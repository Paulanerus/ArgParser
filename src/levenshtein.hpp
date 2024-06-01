#pragma once

#include <string_view>
#include <algorithm>
#include <cstdint>
#include <vector>

size_t calculateDistance(std::string_view src, std::string_view target)
{
    if (src == target)
        return 0;

    if (src.empty() || target.empty())
        return std::max(src.length(), target.length());

    std::vector<size_t> distance(target.length() + 1);

    for (size_t i{}; i < src.length(); i++)
    {
        distance[0] = i + 1;

        auto corner = i;

        for (size_t j{}; j < target.length(); j++)
        {
            auto upper = distance[j + 1];

            distance[j + 1] = src[i] == target[j] ? corner : std::min(std::min(upper, corner), distance[j]) + 1;
            corner = upper;
        }
    }

    return distance[target.length()];
}