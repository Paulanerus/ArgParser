#pragma once

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>

std::string Join(const std::vector<std::string> &vec, const char *delimiter = ", ")
{
    std::ostringstream stream;

    auto begin = vec.begin(), end = vec.end();

    if (begin != end)
    {
        std::copy(begin, std::prev(end), std::ostream_iterator<std::string>(stream, delimiter));
        begin = prev(end);
    }

    if (begin != end)
        stream << *begin;

    return stream.str();
}