#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <chrono>
#include <memory>

#include "src/app/utils/utilities.hpp"
#include "src/app/processes/encryptionExecutor.hpp"
#include "src/app/processes/encryptionTask.hpp"
#include "src/app/encryptDecrypt/cryption.hpp"
#include "src/app/fileHandling/fileHandler.hpp"

namespace fs = std::filesystem;

int main()
{
    printHeader();

    std::string directory;
    std::cout << "Enter the directory path: ";
    std::getline(std::cin, directory);

    if (!(fs::exists(directory) && fs::is_directory(directory)))
    {
        std::cout << RED << "Invalid directory path!" << RESET << std::endl;
        return 1;
    }

    std::vector<std::string> options = {"Encrypt files", "Decrypt files"};
    std::vector<std::string> modes = {"Singlethreaded", "Multithreading"};

    int choice = showMenu(options);
    int modeChoice = showMenu(modes);

    Action action = (choice == 0) ? Action::ENCRYPT : Action::DECRYPT;
    std::string modeFlag = (modeChoice == 0) ? "Single" : "Multithreading";

    std::cout << GREEN << "Execution Mode: " << modeFlag << RESET << "\n\n";

    size_t totalFiles = 0;
    size_t totalBytes = 0;
    for (const auto &entry : fs::recursive_directory_iterator(directory))
        if (entry.is_regular_file())
        {
            totalBytes += fs::file_size(entry);
            totalFiles++;
        }

    if (totalFiles == 0)
    {
        std::cout << RED << "No files found in the directory!" << RESET << std::endl;
        return 0;
    }

    ThreadPool threadPool(modeFlag);
    size_t processed = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    for (const auto &entry : fs::recursive_directory_iterator(directory))
    {
        if (!entry.is_regular_file())
            continue;

        processed++;
        showProgressBar(processed, totalFiles);

        std::string filePath = entry.path().string();
        FileHandler fileHandler(filePath);
        std::fstream fStream = std::move(fileHandler.getFileStream());
        if (!fStream.is_open())
            continue;

        auto task = std::make_unique<EncryptionTask>(std::move(fStream), action, filePath);
        if (modeFlag == "Single")
            executeCryption(task->toString());
        else
            threadPool.submitToQueue(std::move(task));
    }

    if (modeFlag == "Multithreading")
        threadPool.waitForCompletion();

    auto endTime = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(endTime - startTime).count();
    double mbProcessed = totalBytes / (1024.0 * 1024.0);
    double speed = mbProcessed / duration;

    printSummary(totalFiles, mbProcessed, duration, speed);
    return 0;
}
