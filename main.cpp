#include <iostream>
#include <fstream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include "api.h"

int main(){

    std::system("clear");

    std::string logo = R"""(
============================================================
        H   H   EEEEE   CCCC   K   K   EEEEE   RRRR
        H   H   E      C       K  K    E       R   R
        HHHHH   EEEE   C       KKK     EEEE    RRRR
        H   H   E      C       K  K    E       R  R
        H   H   EEEEE   CCCC   K   K   EEEEE   R   R
============================================================

    Author: @fakehecker
    Channel: @heckerdrops
    Version: Stripe CPP 0.2 (Revised Edition)

    Starting...
    )""";
    std::cout << logo << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::string code;

    std::string cmd = "https://surefootedgroundedbrackets.junaidrahman2.repl.co/";
    std::string del = "https://surefootedgroundedbrackets.junaidrahman2.repl.co/del.php";
    std::string pass = exec(cmd.c_str());
    std::string delnow = exec(del.c_str());

    if (delnow == "true"){
        std::system("clear");
        std::cout << "Umm, It's time for a good bye. Take care!" << std::endl;
        #if OS_Windows
        std::remove("main.exe");
        #else
        std::remove("main");
        #endif
        std::exit(1);
    }
    
    while (true) {
        std::cout << "Enter Access Code: ";
        std::getline(std::cin, code);

        if (code != pass) {
            std::cout << "Wrong Code!! Try again" << std::endl;
        }
        else {
            break;
        }
    }

    std::cout << "Code Accepted" << std::endl;

    std::cout << "Enter CC File Path: ";
    std::string fp;
    std::getline(std::cin, fp);

    std::cout << "Enter Threads Number: ";
    int numThreads = 1;
    std::cin >> numThreads;

    std::ifstream file(fp);

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    std::system("clear");

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&lines]() {
            while (true) {
                std::string line;
                {
                    if (lines.empty()) {
                        break;
                    }
                    line = lines.back();
                    lines.pop_back();
                }
                checker(line);
            }
        });
    }

    for (std::thread& t : threads) {
        t.join();
    }

    return 0;
}