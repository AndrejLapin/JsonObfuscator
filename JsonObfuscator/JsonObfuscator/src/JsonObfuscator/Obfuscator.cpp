#include "pch.h"
#include "Obfuscator.h"

namespace JsonObfuscator
{
    Obfuscator::Obfuscator()
        :m_ObfuscationFailed(false)
        ,m_FileObfuscated(false)
    {

    }

    Obfuscator Obfuscator::Get()
    {
        return Obfuscator();
    }

    void Obfuscator::Obfuscate(const std::string& filePath)
    {
        m_ObfuscationFailed = false;
        m_FileObfuscated = false;

        m_ObfuscatedJson.clear();
        m_ReplacementMap.clear();

        m_FilePath = filePath;

        json parsedData;
        if (!ParseJsonFromFile(m_FilePath, parsedData))
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

    json Obfuscator::IterateObject(const json& data)
    {
        json output;
        for (auto& [key, value] : data.items())
        {
            const std::string unicodeKey = ConvertToUnicode(key);
            ParseKeyValuePair(output, unicodeKey, value);
        }
        return output;
    }

    json Obfuscator::IterateArray(const json& data)
    {
        json output;
        for (auto& [key, value] : data.items())
        {
            int index = std::stoi(key);
            ParseKeyValuePair(output, index, value);
        }
        return output;
    }

    std::string Obfuscator::ConvertToUnicode(const std::string& input)
    {
        auto obj = m_ReplacementMap.find(input);
        if (obj == m_ReplacementMap.end()) // new replacement key
        {
            m_ReplacementMap[input] = ConvertToHex(input);
        }

        std::stringstream output;
        for (auto character : input)
        {
            unsigned int unicode = static_cast<unsigned int>(character);
            output << "\\u" << std::setfill('0') << std::setw(4) << std::hex << unicode;
        }
        return output.str();
    }

    std::string Obfuscator::ConvertToHex(const std::string& input)
    {
        std::stringstream output;
        for (auto character : input)
        {
            unsigned int hex = static_cast<unsigned int>(character);
            output << "\\x" << std::hex << hex;
        }
        return output.str();
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