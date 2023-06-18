#pragma once

#include "JsonObfuscator/Include.h"

namespace JsonObfuscator::Utils
{
    /*
    * Replaces all substrings with a replacement string
    */
    void ReplaceAll(std::string& input, const std::string& search, const std::string& replace);
    /*
    * Prints json to a file with correct unicode escape sequences
    */
    void PrintJson(const std::string& filePath, json data, std::streamsize padding = 2);

    void PrintJsonByLine(const std::string& filePath, json data, std::streamsize padding = 2);
    /*
    * Prints out json root in a "<key> -> <value>" format
    */
    void PrintReplacementMap(const std::string& filePath, json data);

    std::string GetFormatedJsonString(json data, std::streamsize padding = 2);
    /*
    * Converts input string to a string of unicode escape sequences
    */
    std::string ConvertToUnicode(const std::string& input);
    /*
    * Converts input string to a string of hex escape sequences
    */
    std::string ConvertToHex(const std::string& input);
}