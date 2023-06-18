#include "pch.h"
#include "Obfuscator.h"

#include "JsonObfuscator/Utils.h"

namespace JsonObfuscator
{
    Obfuscator::Obfuscator()
        :m_ObfuscationFailed(false)
        ,m_FileObfuscated(false)
    {

    }

    Obfuscator Obfuscator::Create()
    {
        return Obfuscator();
    }

    void Obfuscator::Obfuscate(const std::string& filePath)
    {
        m_ObfuscationFailed = false;
        m_FileObfuscated = false;

        m_ObfuscatedJson.clear();
        m_ReplacementMap.clear();

        json parsedData;
        if (!ParseJsonFromFile(filePath, parsedData))
        {
            m_ObfuscationFailed = true;
            return;
        }

        m_ObfuscatedJson = IterateObject(parsedData);

        m_FileObfuscated = true;
    }

    bool Obfuscator::ObfuscationSucceeded()
    {
        return !m_ObfuscationFailed && m_FileObfuscated;
    }

    const json& Obfuscator::GetObfuscatedJson()
    {
        assert(ObfuscationSucceeded());
        return m_ObfuscatedJson;
    }

    const json& Obfuscator::GetReplacementMap()
    {
        assert(ObfuscationSucceeded());
        return m_ReplacementMap;
    }

    std::string Obfuscator::ObfuscateString(const std::string& input)
    {
        auto obj = m_ReplacementMap.find(input);
        // check if this replacement entry already exists
        if (obj == m_ReplacementMap.end())
        {
            // creating new replacement entry
            m_ReplacementMap[input] = Utils::ConvertToHex(input);
        }

        return Utils::ConvertToUnicode(input);
    }

    json Obfuscator::IterateObject(const json& data)
    {
        json output;
        for (auto& [key, value] : data.items())
        {
            const std::string unicodeKey = ObfuscateString(key);
            SetKeyValuePair(output, unicodeKey, value);
        }
        return output;
    }

    json Obfuscator::IterateArray(const json& data)
    {
        json output;
        for (auto& [key, value] : data.items())
        {
            int index = std::stoi(key);
            SetKeyValuePair(output, index, value);
        }
        return output;
    }

    bool Obfuscator::ParseJsonFromFile(const std::string& filePath, json& data)
    {
        std::ifstream targetFile(filePath);
        if (!targetFile.is_open())
        {
            std::cout << "File " << filePath << " couldn't be found.\n";
            return false;
        }

        data = json::parse(targetFile, nullptr, false);
        if (data.is_discarded())
        {
            std::cout << "File " << filePath << " is not a valid json.\n";
            return false;
        }

        targetFile.close();
    }
}