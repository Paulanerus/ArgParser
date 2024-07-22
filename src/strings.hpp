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

size_t LevenshteinDistance(std::convertible_to<std::string_view> auto src, std::convertible_to<std::string_view> auto target)
{
    std::string_view src_view = src;
    std::string_view target_view = target;

    if (src_view == target_view)
        return 0;

    if (src_view.empty() || target_view.empty())
        return std::max(src_view.length(), target_view.length());

    std::vector<size_t> distance(target_view.length() + 1);

    for (size_t i{}; i < src_view.length(); i++)
    {
        distance[0] = i + 1;

        auto corner = i;

        for (size_t j{}; j < target_view.length(); j++)
        {
            auto upper = distance[j + 1];

            distance[j + 1] = src_view[i] == target_view[j] ? corner : std::min({upper, corner, distance[j]}) + 1;
            corner = upper;
        }
    }

    return distance[target_view.length()];
}