#pragma once

#include "JsonObfuscator/Include.h"

namespace JsonObfuscator
{
    class OutputFileNames;

    class Obfuscator
    {
    public:
        enum class State
        {
            NONE,
            OBFUSCATION_STARTED,
            FILE_NOT_FOUND,
            INVALID_JSON,
            OBFUSCATED_SUCCESSFULLY
        };
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
        void Obfuscate(const json& data);

        inline State GetObfuscationState() { return m_State; }

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

        void ObfuscateAndSetValue(json::reference& object, const json& value);

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
        State m_State;

        json m_ObfuscatedJson;
        json m_ReplacementMap;
    };
}
