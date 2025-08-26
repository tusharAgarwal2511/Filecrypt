#ifndef ENCRYPTION_TASK
#define ENCRYPTION_TASK

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../fileHandling/fileHandler.hpp"

enum class Action{
    ENCRYPT,
    DECRYPT
};

struct EncryptionTask{
    std::string filePath;
    std::fstream fStream;
    Action action;

    EncryptionTask(std::fstream &&stream, Action act, std::string filePath) : fStream(std::move(stream)), action(act), filePath(filePath){
    }
    
    std::string toString(){
        std::ostringstream oss;
        oss << filePath << "," << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
        return oss.str();
    }

    static EncryptionTask fromString(const std::string &taskData){
        std::istringstream iss(taskData);
        std::string filePath;
        std::string actionStr;
        if (std::getline(iss, filePath, ',') && std::getline(iss, actionStr)){
            Action action = (actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);
            FileHandler fileHandler(filePath);
            std::fstream fStream = std::move(fileHandler.getFileStream());
            if (fStream.is_open()){
                return EncryptionTask(std::move(fStream), action, filePath);
            }
            else{
                throw std::runtime_error("Failed to open file: " + filePath);
            }
        }
        else{
            throw std::runtime_error("Invalid task data format");
        }
    }
};

#endif
