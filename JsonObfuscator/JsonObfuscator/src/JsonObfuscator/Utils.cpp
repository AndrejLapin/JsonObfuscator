#include "pch.h"
#include "Utils.h"

namespace JsonObfuscator::Utils
{
    bool Compare(const std::string& input, const std::string& search, unsigned int pos)
    {
        unsigned int currentIndex = 0;
        while (currentIndex < search.length())
        {
            if (input[pos + currentIndex] != search[currentIndex]) { return false; }
            ++currentIndex;
        }
        return true;
    }

    void ReplaceAll(std::string& input, const std::string& search, const std::string& replace)
    {
        for (unsigned int index = 0; index < input.length(); ++index)
        {
            if (Compare(input, search, index))
            {
                input.replace(index, search.length(), replace);
                index += replace.length();
            }
        }
    }

    void PrintJson(const std::string& filePath, json data, std::streamsize padding)
    {
        std::string outputString = GetFormatedJsonString(data, padding);
        // when nlohmann::json gets printed in its entirety it adds additional escape sequence before '\'
        // so we need to eliminate them here
        JsonObfuscator::Utils::ReplaceAll(outputString, "\\\\u", "\\u");

        std::ofstream outputFile(filePath);
        outputFile << outputString;
        outputFile.close();
    }

    void PrintJsonByLine(const std::string& filePath, json data, std::streamsize padding)
    {
        std::stringstream outputStream;
        outputStream << std::setw(padding) << data;

        std::ofstream outputFile(filePath);
        std::string line;
        while (getline(outputStream, line))
        {
            // when nlohmann::json gets printed in its entirety it adds additional escape sequence before '\'
            // so we need to eliminate them here
            JsonObfuscator::Utils::ReplaceAll(line, "\\\\u", "\\u");
            outputFile << line << "\n";
        }
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

    std::string GetFormatedJsonString(json data, std::streamsize padding)
    {
        std::stringstream outputStream;
        outputStream << std::setw(padding) << data;
        return outputStream.str();
    }

    std::string ConvertToUnicode(const std::string& input)
    {
        std::stringstream output;
        for (auto character : input)
        {
            unsigned int unicode = static_cast<unsigned int>(character);
            output << "\\u" << std::setfill('0') << std::setw(4) << std::hex << unicode;
        }
        return output.str();
    }

    std::string ConvertToHex(const std::string& input)
    {
        std::stringstream output;
        for (auto character : input)
        {
            unsigned int hex = static_cast<unsigned int>(character);
            output << "\\x" << std::hex << hex;
        }
        return output.str();
    }
}