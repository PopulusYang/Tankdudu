#include<fstream>
#include<string>
#include<iostream>
int fileread()
{
    std::ifstream file("scores.txt"); // 打开文件以进行读取  

    if (!file) { // 检查文件是否成功打开  
        std::cerr << "Unable to open file";
        exit(-1);
    }

    std::string line;
    while (std::getline(file, line)) { // 从文件中读取一行  
        std::cout << line << std::endl; // 输出该行  
    }
}