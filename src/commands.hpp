#pragma once

#include "arg_parser.hpp"
#include "strings.hpp"

inline auto HELP_ACTION = [](const ArgParser &parser, [[maybe_unused]] const auto &_)
{
    parser(parser.argAt(0));
};

inline auto NEW_ACTION = []([[maybe_unused]] const ArgParser &parser, [[maybe_unused]] const Command &cmd)
{
    std::cout << "New" << std::endl;
};

inline auto ADD_ACTION = []([[maybe_unused]] const ArgParser &parser, [[maybe_unused]] const Command &cmd)
{
    std::cout << "Add" << std::endl;
};

inline auto REMOVE_ACTION = []([[maybe_unused]] const ArgParser &parser, [[maybe_unused]] const Command &cmd)
{
    std::cout << "Remove" << std::endl;
};

inline auto BUILD_ACTION = []([[maybe_unused]] const ArgParser &parser, [[maybe_unused]] const Command &cmd)
{
    std::cout << "Build" << std::endl;
};

inline auto RUN_ACTION = []([[maybe_unused]] const ArgParser &parser, [[maybe_unused]] const Command &cmd)
{
    std::cout << "Run" << std::endl;
};