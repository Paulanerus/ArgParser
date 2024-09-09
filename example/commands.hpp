#pragma once

#include "arg_parser.hpp"

#ifdef _MSC_VER
#define AP_MAYBE_UNUSED

#pragma warning(push)
#pragma warning(disable : 4100)

#else
#define AP_MAYBE_UNUSED [[maybe_unused]]
#endif

inline auto HELP_ACTION = [](const psap::ArgParser &parser, AP_MAYBE_UNUSED const auto &_)
{
    parser(parser[0]);
};

inline auto NEW_ACTION = [](AP_MAYBE_UNUSED const auto &_, AP_MAYBE_UNUSED const auto &__)
{
    std::cout << "New" << std::endl;
};

inline auto ADD_ACTION = [](AP_MAYBE_UNUSED const auto &_, AP_MAYBE_UNUSED const psap::Command &cmd)
{
    auto branch = cmd.get<std::string>("--branch").value_or("master");

    if (cmd.has("-t"))
        std::cout << "Tested" << std::endl;

    if (cmd["--link"])
        std::cout << std::quoted("--link") << " was specified\n";

    std::cout << "Selected branch: " << branch << std::endl;
};

inline auto REMOVE_ACTION = [](AP_MAYBE_UNUSED const auto &_, AP_MAYBE_UNUSED const auto &__)
{
    std::cout << "Remove" << std::endl;
};

inline auto BUILD_ACTION = [](AP_MAYBE_UNUSED const auto &_, AP_MAYBE_UNUSED const auto &__)
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

#ifdef _MSC_VER
#pragma warning(pop)
#endif