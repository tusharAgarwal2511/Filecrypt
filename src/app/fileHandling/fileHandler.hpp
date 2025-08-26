#ifndef FILE_HANDLER
#define FILE_HANDLER

#include <fstream>
#include <string>
#include <iostream>

class FileHandler{
public:
    FileHandler(const std::string &filePath);
    ~FileHandler();
    std::fstream getFileStream();

private:
    std::fstream fileStream;
};

#endif