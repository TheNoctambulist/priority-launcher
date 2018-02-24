// Copyright (c) 2018 TheNoctambulist
// Distributed under the MIT license.

#pragma once

namespace ArgParser
{

    // Fairly basic upper case implementation, but sufficient for parsing arguments
    void ToUpper(std::wstring& arg)
    {
        for (auto &c : arg)
        {
            c = towupper(c);
        }
    }

    class SwitchArgParser
    {
    public:
        SwitchArgParser(std::wstring switchArg)
            : switchArg(switchArg)
        {
            ToUpper(this->switchArg);
        }
        ~SwitchArgParser()
        {
        }

        bool IsSwitch(std::wstring arg)
        {
            ToUpper(arg);
            return arg == switchArg;
        }

    private:
        std::wstring switchArg;
    };

    template<typename Enum>
    class EnumArgParser
    {
    public:
        EnumArgParser(std::wstring switchArg)
            : switchArg(switchArg)
        {
            ToUpper(this->switchArg);
        }
        ~EnumArgParser()
        {
        }

        void AddValue(Enum value, std::wstring text)
        {
            ToUpper(text);
            enumMap.emplace(text, value);
        }

        bool IsSwitch(std::wstring arg)
        {
            ToUpper(arg);
            return arg == switchArg;
        }

        bool TryParse(std::wstring text, Enum &value)
        {
            ToUpper(text);
            auto argMatch = enumMap.find(text);
            if (argMatch == enumMap.end())
            {
                return false;
            }
            else
            {
                value = argMatch->second;
                return true;
            }
        }

    private:
        std::wstring switchArg;
        std::unordered_map<std::wstring, Enum> enumMap;
    };


}
