#include <iostream>
#include <fstream>
#include "fileHandler.hpp"

FileHandler::FileHandler(const std::string &filePath){
    fileStream.open(filePath, std::ios::in | std::ios::out | std::ios::binary);
    // if (!fileStream.is_open()){
    // }
}

std::fstream FileHandler::getFileStream(){
    return std::move(fileStream);
}

FileHandler::~FileHandler(){ 
    if (fileStream.is_open()){
        fileStream.close();
    }
}