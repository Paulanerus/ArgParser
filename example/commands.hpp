#pragma once

#include "arg_parser.hpp"

inline auto HELP_ACTION = [](const psap::ArgParser &parser, [[maybe_unused]] const auto &_)
{
    parser(parser[0]);
};

inline auto NEW_ACTION = []([[maybe_unused]] const auto &_, [[maybe_unused]] const auto &__)
{
    std::cout << "New" << std::endl;
};

inline auto ADD_ACTION = []([[maybe_unused]] const auto &_, const psap::Command &cmd)
{
    auto branch_opt = cmd.get<std::string>("--branch");

    if (cmd["--link"])
        std::cout << std::quoted("--link") << " was specified\n";

    std::cout << std::format("Selected branch: {}", branch_opt.value()) << std::endl;
};

inline auto REMOVE_ACTION = []([[maybe_unused]] const auto &_, [[maybe_unused]] const auto &__)
{
    std::cout << "Remove" << std::endl;
};

inline auto BUILD_ACTION = []([[maybe_unused]] const auto &_, [[maybe_unused]] const auto &__)
{
    std::cout << "Build" << std::endl;
};

inline auto RUN_ACTION = [](const psap::ArgParser &parser, const psap::Command &cmd)
{
    auto val = parser.get<int>("--value").value_or(32);

    std::cout << "Global value: " << val << std::endl;

    if (parser.has("-t"))
        std::cout << "Global test was set" << std::endl;

    if (cmd["--debug"])
        std::cout << "Debug was specified" << std::endl;
};