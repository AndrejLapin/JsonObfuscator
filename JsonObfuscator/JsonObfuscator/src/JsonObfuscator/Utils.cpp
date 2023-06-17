#include "pch.h"
#include "Utils.h"

namespace JsonObfuscator::Utils
{
    void ReplaceAll(std::string& str, const std::string& search, const std::string& replace)
    {
        size_t pos;
        while ((pos = str.find(search)) != std::string::npos)
        {
            str.replace(pos, search.length(), replace);
        }
    }

    void PrintJson(const std::string& filePath, json data, std::streamsize padding)
    {
        std::stringstream outputStream;
        outputStream << std::setw(padding) << data;
        std::string outputString = outputStream.str();
        JsonObfuscator::Utils::ReplaceAll(outputString, "\\\\u", "\\u");

        std::ofstream outputFile(filePath);
        outputFile << outputString;
        outputFile.close();
    }

    void PrintReplacementMap(const std::string& filePath, json data)
    {
        std::stringstream outputStream;
        for (auto [key, value] : data.items())
        {
            outputStream << key << " -> " << static_cast<std::string>(value) << "\n";
        }
        std::ofstream outputFile(filePath);
        outputFile << outputStream.str();
        outputFile.close();
    }
}