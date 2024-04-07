#pragma once

#include "strings.hpp"

#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <any>

struct Argument
{
    std::vector<std::string> identifier;

    std::string help;

    bool flag;

    bool required;

    std::any value;

    bool isFlag() const noexcept
    {
        return flag && !required;
    }

    bool isDefault() const noexcept
    {
        return !flag && !required;
    }

    bool isRequired() const noexcept
    {
        return !flag && required;
    }

    static Argument Flag(std::initializer_list<std::string> &&identifier, std::string &&help_txt)
    {
        return Argument{
            .identifier = std::move(identifier),
            .help = std::move(help_txt),
            .flag = true,
            .required = false,
            .value = {}};
    }

    template <typename T>
    static Argument Default(std::initializer_list<std::string> &&identifier, std::string &&help_txt, T &&any)
    {
        return Argument{
            .identifier = std::move(identifier),
            .help = std::move(help_txt),
            .flag = false,
            .required = false,
            .value = std::move(any)};
    }

    static Argument Required(std::initializer_list<std::string> &&identifier, std::string &&help_txt)
    {
        return Argument{
            .identifier = std::move(identifier),
            .help = std::move(help_txt),
            .flag = false,
            .required = true,
            .value = {}};
    }
};

class Command
{
public:
    Command(std::vector<std::string> &&identifier) noexcept : m_Identifier(std::move(identifier)) {}

    Command &help(std::string &&help) noexcept
    {
        m_Help = std::move(help);

        return *this;
    }

    Command &argument(Argument &&arg) noexcept
    {
        m_Arguments.push_back(std::move(arg));

        return *this;
    }

    std::string_view help() const noexcept
    {
        return m_Help;
    }

    const std::vector<Argument> &arguments() const noexcept
    {
        return m_Arguments;
    }

    friend std::ostream &operator<<(std::ostream &os, const Command &command)
    {
        return os << Join(command.m_Identifier);
    }

private:
    std::vector<std::string> m_Identifier;

    std::string m_Help;

    std::vector<Argument> m_Arguments;
};

class ArgParser
{
public:
    ArgParser() noexcept = default;

    template <typename... Args, typename = std::enable_if_t<std::conjunction_v<std::is_convertible<Args, std::string>...>>>
    Command &command(Args &&...args) noexcept
    {
        std::vector<std::string> identifier{std::forward<Args>(args)...};

        return m_Commands.emplace_back(std::move(identifier));
    }

    std::vector<std::string> parse(const char *argv[], int argc, size_t start = 1)
    {
        std::vector<std::string> args{argv + start, argv + argc};

        std::for_each(args.begin(), args.end(), [](std::string &arg)
                      { std::transform(arg.begin(), arg.end(), arg.begin(), [](uint8_t c)
                                       { return std::tolower(c); }); });

        // Actual parsing

        return args;
    }

    friend std::ostream &operator<<(std::ostream &os, const ArgParser &arg_parser)
    {
        os << "Commands:\n";

        // TODO (paul) copy cargo help layout

        for (auto &cmd : arg_parser.m_Commands)
        {
            os << "    " << cmd << "\n";
        }

        return os;
    }

private:
    std::vector<Command> m_Commands;
};
