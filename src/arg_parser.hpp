#pragma once

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <charconv>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class Command : std::enable_shared_from_this<Command>
{
public:
    Command() noexcept = default;

    bool operator[](std::string_view strIn) const noexcept
    {
        return false;
    }

private:
};

class ArgParser
{
public:
    ArgParser() noexcept = default;

    template <typename... Args, typename = std::enable_if_t<std::conjunction_v<std::is_same<Args, std::string>...>>>
    std::unique_ptr<Command> command(Args &&...) noexcept
    {
        return std::make_unique<Command>(std::forward<Args>(args)...);
    }

    std::vector<std::string> parse(const char *argv[], int argc, size_t start = 1)
    {
        std::vector<std::string> args{argv + start, argv + argc};

        std::for_each(args.begin(), args.end(), [](std::string &arg)
                      { std::transform(arg.begin(), arg.end(), arg.begin(), [](uint8_t c)
                                       { return std::tolower(c); }); });

        for (auto &arg : args)
            std::cout << arg << std::endl;

        return args;
    }

private:
};