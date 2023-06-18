// JsonObfuscator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "src/JsonObfuscator/Obfuscator.h"
#include "src/JsonObfuscator/Utils.h"

// argv[1] should be our target json path
int main(int argc, char* argv[])
{
    // check if file argument was provided
    if (argc < 2)
    {
        std::cout << "Provide a json file as an argument.\n";
        return 1;
    }

    const std::string filePath(argv[1]);
    const std::string jsonExtension = ".json";

    const size_t formatBeggining = filePath.find(jsonExtension);
    if (formatBeggining == std::string::npos)
    {
        std::cout << "Incorrect file format. Fromat should be '.json'.\n";
        return 1;
    }
    
    const std::string nameNoExtension = filePath.substr(0, formatBeggining);
    const std::string outputFileName = nameNoExtension + "-obfuscated" + jsonExtension;
    const std::string outputMapFileName = nameNoExtension + "-replacement_map";

    JsonObfuscator::Obfuscator obfuscator = JsonObfuscator::Obfuscator::Create();
    obfuscator.Obfuscate(filePath);
    if (!obfuscator.ObfuscationSucceeded())
    {
        return 1;
    }
    json obfuscatedJson = obfuscator.GetObfuscatedJson();
    json replacementMap = obfuscator.GetReplacementMap();

    std::future<void> printJsonTask = std::async(std::launch::async, JsonObfuscator::Utils::PrintJson, outputFileName, obfuscatedJson, 2);
    std::future<void> printMapTask = std::async(std::launch::async, JsonObfuscator::Utils::PrintReplacementMap, outputMapFileName, replacementMap);

    std::cout << "Succesufully obfuscated json file.\n";
    return 0;
}
