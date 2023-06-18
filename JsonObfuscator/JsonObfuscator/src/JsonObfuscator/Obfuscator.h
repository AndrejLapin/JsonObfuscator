#pragma once

#include "JsonObfuscator/Include.h"

namespace JsonObfuscator
{
    class OutputFileNames;

    class Obfuscator
    {
    public:
        /*
        * Creates obfuscator on the stack
        */
        static Obfuscator Create();

        /*
        * Obfuscates json file at filePath
        * and creates obfuscated json and replacement map json
        */
        void Obfuscate(const std::string& filePath);

        bool ObfuscationSucceeded();

        /*
        * If ObfuscationSucceeded returns obfuscated json
        */
        const json& GetObfuscatedJson();
        /*
        * If ObfuscationSucceeded returns replacement map json
        */
        const json& GetReplacementMap();

    private:
        Obfuscator(); // constuctor is private so we dont create it on the heap

        template <typename KeyType>
        void SetKeyValuePair(json& object, KeyType key, const json& value);

        /*
        * converts input string to a sting of unicode escape sequences
        * and fills replacement map with new obfuscation entries
        */
        std::string ObfuscateString(const std::string& input);

        json IterateObject(const json& data);
        json IterateArray(const json& data);

        /*
        * Parses json at filePath to data
        * if succeeds returns true, if not - false
        */
        bool ParseJsonFromFile(const std::string& filePath, json& data);

    private:
        bool m_ObfuscationFailed;
        bool m_FileObfuscated;

        json m_ObfuscatedJson;
        json m_ReplacementMap;
    };

    template<typename KeyType>
    inline void Obfuscator::SetKeyValuePair(json& object, KeyType key, const json& value)
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
            object[key] = ObfuscateString(value);
        }
        else
        {
            object[key] = value;
        }
    }
}
