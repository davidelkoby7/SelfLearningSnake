#pragma once

#include <stdlib.h>
#include <chrono>
#include <fstream>
#include <math.h> // pow, sqrt

#include "DynamicArray.h"

namespace GeneralFunctions
{
    double Power(const double& num, const double& power)
    {
        return pow(num, power);
    }

    double Square(const double& num)
    {
        return Power(num, 2);
    }

    double SquareRoot(const double& num)
    {
        return sqrt(num);
    }

    char* ReadBinaryFile(const char* path)
    {
        std::streampos size;
        char * memblock;

        std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
        if(file.is_open())
        {
            size = file.tellg();
            memblock = new char [size];
            file.seekg (0,std::ios::beg);
            file.read (memblock, size);
            file.close();
            return memblock;
        }
        std::cout << "Unable to open file\n";
    }

    Utils::DynamicArray<std::string> SplitString(const std::string& str, const std::string& delim=",")
    {
        Utils::DynamicArray<std::string> tokens;
        size_t prev = 0, pos = 0;
        do
        {
            pos = str.find(delim, prev);
            if (pos == std::string::npos)
                pos = str.length();

            std::string token = str.substr(prev, pos-prev);

            if (!token.empty())
                tokens.AddItem(token);

            prev = pos + delim.length();
        }
        while (pos < str.length() && prev < str.length());

        return tokens;
    }

    bool RemoveFile(const char* path)
    {
        return remove(path) == 0;
    }

    std::string ReadFile(const char* path)
    {
        std::ifstream fileStream(path);
        std::string content((std::istreambuf_iterator<char>(fileStream)),(std::istreambuf_iterator<char>()));
        content = content.substr(0, content.size() - 1); // Removing the added newline
        return content;
    }

    void WriteToFile(const char* path, const char* data)
    {
        std::ofstream outfile;
        outfile.open(path);
        outfile << data << "\n";
        outfile.close();
    }

    void AppendToFile(const char* path, const char* data)
    {
        std::ofstream outfile;
        outfile.open(path, std::ios::app);
        outfile << data << "\n";
        outfile.close();
    }

    size_t StringLength(const char* string)
    {
        size_t length = 0;
        while (string[length] != '\x00')
            length++;
        return length;
    }

    int RandomInt(const int& minValue, const int& maxValue)
    {
        // Getting current time in microsecends
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        auto randSeed = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

        // Initializing random's seed
        srand(randSeed);

        // Getting a rand number in the given range
        int randNum = rand() % (maxValue - minValue);
        randNum += minValue;
        return randNum;
    }

    double RandomDouble(double minValue, double maxValue)
    {
       const int precision = 1000000;
       const int randInt = RandomInt(0, precision);
       const double randFactor = static_cast<double>(randInt) / static_cast<double>(precision);
       const double randDouble = minValue + randFactor * (maxValue - minValue);
       return randDouble;
    }
}
