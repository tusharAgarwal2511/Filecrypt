
#include "encryptionExecutor.hpp"
#include "encryptionTask.hpp"
#include <iostream>
#include <cstring>
#include "../encryptDecrypt/cryption.hpp"
#include <thread>

ThreadPool::ThreadPool(std::string mode){
   
}

bool ThreadPool::submitToQueue(std::unique_ptr<EncryptionTask> task){
    std::string taskString = task->toString();
    threads.emplace_back([this, taskString]()
                         { this->executeTasks(taskString); });
    return true;
}

void ThreadPool::executeTasks(const std::string &taskStr){
    executeCryption(taskStr);
}

ThreadPool::~ThreadPool(){
}

void ThreadPool::waitForCompletion(){
    for (auto &t : threads){
        if (t.joinable())
            t.join();
    }
    threads.clear();  
}
