#pragma once

#include "JsonObfuscator/Include.h"

namespace JsonObfuscator::Utils
{
    void ReplaceAll(std::string& str, const std::string& search, const std::string& replace);
    void PrintJson(const std::string& filePath, json data, std::streamsize padding = 2);
    void PrintReplacementMap(const std::string& filePath, json data);
}