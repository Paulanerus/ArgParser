#pragma once

#include <string_view>
#include <algorithm>
#include <concepts>
#include <iterator>
#include <sstream>
#include <cstdint>
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

std::size_t LevenshteinDistance(std::convertible_to<std::string_view> auto src, std::convertible_to<std::string_view> auto target)
{
    if (src == target)
        return 0;

    if (src.empty() || target.empty())
        return std::max(src.length(), target.length());

    std::vector<std::size_t> distance(target.length() + 1);

    for (std::size_t i{}; i < src.length(); i++)
    {
        distance[0] = i + 1;

        auto corner = i;

        for (size_t j{}; j < target.length(); j++)
        {
            auto upper = distance[j + 1];

            distance[j + 1] = src[i] == target[j] ? corner : std::min({upper, corner, distance[j]}) + 1;
            corner = upper;
        }
    }

    return distance[target.length()];
}