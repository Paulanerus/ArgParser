#pragma once

#include <algorithm>
#include <iostream>
#include <charconv>
#include <string>
#include <vector>

class ArgParser
{
public:
    ArgParser(int argc, char *argv[]) : m_Args(argv + 1, argv + argc)
    {
    }

private:
    std::vector<std::string> m_Args;
};