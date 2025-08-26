#include "utilities.hpp"
#include <termios.h>
#include <unistd.h>
#include <iostream>

void moveCursorUp(int n)
{
    std::cout << "\033[" << n << "A";
}

char getKey()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

int showMenu(const std::vector<std::string> &options)
{
    int selected = 0;

    for (int i = 0; i < options.size(); ++i)
    {
        if (i == selected)
            std::cout << "> " << YELLOW << options[i] << RESET << "\n";
        else
            std::cout << "  " << CYAN << options[i] << RESET << "\n";
    }
    std::cout << std::flush;

    while (true)
    {
        char c = getKey();
        if (c == '\033')
        {
            getKey();
            char arrow = getKey();
            int prevSelected = selected;

            if (arrow == 'A')
                selected = (selected - 1 + options.size()) % options.size();
            if (arrow == 'B')
                selected = (selected + 1) % options.size();

            if (prevSelected != selected)
            {
                moveCursorUp(options.size());
                for (int i = 0; i < options.size(); ++i)
                {
                    if (i == selected)
                        std::cout << "> " << YELLOW << options[i] << RESET << "  \n";
                    else
                        std::cout << "  " << CYAN << options[i] << RESET << "  \n";
                }
                std::cout << std::flush;
            }
        }
        else if (c == '\n' || c == '\r')
        {
            return selected;
        }
    }
}

void printHeader()
{
    std::cout << CYAN;
    std::cout << "=====================================\n";
    std::cout << "        FILECRYPT - Secure Encryptor \n";
    std::cout << "=====================================\n\n";
    std::cout << RESET;
}

void showProgressBar(size_t current, size_t total)
{
    const int barWidth = 40;
    float progress = static_cast<float>(current) / total;
    int pos = barWidth * progress;

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

void printSummary(size_t files, double mbProcessed, double duration, double speed)
{
    std::cout << "\n\n"
              << GREEN << "Processing Complete!" << RESET << "\n\n";
    std::cout << CYAN << std::left << std::setw(20) << "Files processed:" << RESET << files << "\n";
    std::cout << CYAN << std::setw(20) << "Total size:" << RESET << mbProcessed << " MB\n";
    std::cout << CYAN << std::setw(20) << "Time taken:" << RESET << duration << " sec\n";
    std::cout << CYAN << std::setw(20) << "Speed:" << RESET << speed << " MB/sec\n\n";
}
