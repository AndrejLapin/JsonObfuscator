#pragma once

#include "JsonObfuscator/Include.h"

namespace JsonObfuscator
{
    class OutputFileNames;

    class Obfuscator
    {
    public:
        static Obfuscator Get();

        void Obfuscate(const std::string& filePath);

        bool ObfuscationSucceeded();

        const json& GetObfuscatedJson();
        const json& GetReplacementMap();

    private:
        Obfuscator();

        template <typename KeyType>
        void ParseKeyValuePair(json& object, KeyType key, const json& value);

        json IterateObject(const json& data);
        json IterateArray(const json& data);

        std::string ConvertToUnicode(const std::string& input);
        std::string ConvertToHex(const std::string& input);

        bool ParseJsonFromFile(const std::string& filePath, json& data);

    private:
        std::string m_FilePath;

        bool m_ObfuscationFailed;
        bool m_FileObfuscated;

        json m_ObfuscatedJson;
        json m_ReplacementMap;
    };

    template<typename KeyType>
    inline void Obfuscator::ParseKeyValuePair(json& object, KeyType key, const json& value)
    {
        if (value.is_object())
        {
            object[key] = IterateObject(value);
        }
        else if (value.is_array())
        {
            object[key] = IterateArray(value);
        }
        else if (value.is_string())
        {
            object[key] = ConvertToUnicode(value);
        }
        else
        {
            object[key] = value;
        }
    }
}
