#include<fstream>
#include<string>
#include<iostream>
int fileread()
{
    std::ifstream file("scores.txt"); // ���ļ��Խ��ж�ȡ  

    if (!file) { // ����ļ��Ƿ�ɹ���  
        std::cerr << "Unable to open file";
        exit(-1);
    }

    std::string line;
    while (std::getline(file, line)) { // ���ļ��ж�ȡһ��  
        std::cout << line << std::endl; // �������  
    }
}