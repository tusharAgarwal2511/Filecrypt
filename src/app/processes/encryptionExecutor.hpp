
#ifndef ENCRYPTION_EXECUTOR
#define ENCRYPTION_EXECUTOR

#include <iostream>
#include <string>
#include <queue>
#include <memory>
#include "encryptionTask.hpp"
#include <mutex>
#include <vector>
#include <thread>

class ThreadPool{
public:
    ThreadPool(std::string mode);
    ~ThreadPool();
    bool submitToQueue(std::unique_ptr<EncryptionTask> task);
    void waitForCompletion(); 
    void executeTasks(const std::string &taskStr);

private:
    // std::mutex queueLock;
    std::vector<std::thread> threads;  
};

#endif
