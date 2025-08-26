#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "cryption.hpp"
#include <sstream>
#include "../processes/encryptionTask.hpp"
#include <cstring>
#include <bits/std_thread.h>
#include <filesystem>
#include <fileHandler.hpp>

bool aesEncryptFile(const std::string &inputFile, const std::string &outputFile, const std::string &keyStr){
    unsigned char key[32] = {0}; 
    unsigned char iv[16];       
    memcpy(key, keyStr.data(), std::min(keyStr.size(), sizeof(key)));
    if (!RAND_bytes(iv, sizeof(iv)))
        return false; 

    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);
    if (!in || !out)
        return false;

        out.write((char *)iv, sizeof(iv));

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    std::vector<unsigned char> buffer(4096), cipher(4100);
    int outLen;
    while (in.read((char *)buffer.data(), buffer.size()) || in.gcount())
    {
        EVP_EncryptUpdate(ctx, cipher.data(), &outLen, buffer.data(), in.gcount());
        out.write((char *)cipher.data(), outLen);
    }
    EVP_EncryptFinal_ex(ctx, cipher.data(), &outLen);
    out.write((char *)cipher.data(), outLen);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool aesDecryptFile(const std::string &inputFile, const std::string &outputFile, const std::string &keyStr){
    unsigned char key[32] = {0};
    memcpy(key, keyStr.data(), std::min(keyStr.size(), sizeof(key)));

    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);
    if (!in || !out)
        return false;

    unsigned char iv[16];
    in.read((char *)iv, sizeof(iv)); 

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    std::vector<unsigned char> buffer(4096), plain(4100);
    int outLen;
    while (in.read((char *)buffer.data(), buffer.size()) || in.gcount())
    {
        EVP_DecryptUpdate(ctx, plain.data(), &outLen, buffer.data(), in.gcount());
        out.write((char *)plain.data(), outLen);
    }
    EVP_DecryptFinal_ex(ctx, plain.data(), &outLen);
    out.write((char *)plain.data(), outLen);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

std::string getEnvKey(){
    std::string envPath = ".env";
    FileHandler FileHandler(envPath);
    std::fstream fStream = FileHandler.getFileStream();
    std::stringstream buffer;
    buffer << fStream.rdbuf();
    std::string content = buffer.str();
    return content;
}

int executeCryption(const std::string &taskData)
{
    EncryptionTask task = EncryptionTask::fromString(taskData);
    std::string envKey = getEnvKey();            
    std::string tempFile = task.filePath + ".tmp"; 

    if (task.action == Action::ENCRYPT)
    {
        aesEncryptFile(task.filePath, tempFile, envKey);
    }
    else
    {
        aesDecryptFile(task.filePath, tempFile, envKey);
    }

    // Replace original file with processed file
    std::remove(task.filePath.c_str());
    std::rename(tempFile.c_str(), task.filePath.c_str());

    return 0;
}
