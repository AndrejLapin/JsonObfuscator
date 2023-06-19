#include "pch.h"
#include "Obfuscator.h"

#include "JsonObfuscator/Utils.h"

namespace JsonObfuscator
{
    Obfuscator::Obfuscator()
        :m_State(State::NONE)
        ,m_ObfuscationFailed(false)
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

        m_State = State::OBFUSCATION_STARTED;

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

        m_State = State::OBFUSCATED_SUCCESSFULLY;
    }

    Obfuscator::State Obfuscator::GetObfuscationState()
    {
        return m_State;
    }

    const json& Obfuscator::GetObfuscatedJson()
    {
        assert(GetObfuscationState() == State::OBFUSCATED_SUCCESSFULLY);
        return m_ObfuscatedJson;
    }

    const json& Obfuscator::GetReplacementMap()
    {
        assert(GetObfuscationState() == State::OBFUSCATED_SUCCESSFULLY);
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
            m_State = State::FILE_NOT_FOUND;
            return false;
        }

        data = json::parse(targetFile, nullptr, false);
        if (data.is_discarded())
        {
            m_State = State::INVALID_JSON;
            return false;
        }

        targetFile.close();
    }
}