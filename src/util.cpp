
/**
 * @file
 *
 * @brief Utility functions
 *
 * @copyright Unassemblize is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            3 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#include "util.h"
#include <codecvt>

namespace util
{
std::string to_utf8(const wchar_t *utf16)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.to_bytes(utf16);
}

std::string to_utf8(const std::wstring &utf16)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.to_bytes(utf16);
}

std::wstring to_utf16(const char *utf8)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.from_bytes(utf8);
}

std::wstring to_utf16(const std::string &utf8)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.from_bytes(utf8);
}

void strip_inplace(std::string &str, std::string_view chars)
{
    auto pred = [&chars](const char &c) { return chars.find(c) != std::string::npos; };
    str.erase(std::remove_if(str.begin(), str.end(), pred), str.end());
}

std::string strip(const std::string &str, std::string_view chars)
{
    std::string s(str);
    strip_inplace(s, chars);
    return s;
}

std::string get_file_ext(const std::string &file_name)
{
    const size_t pos = file_name.find_last_of(".");
    if (pos != std::string::npos)
    {
        return file_name.substr(pos + 1);
    }
    return {};
}

} // namespace util