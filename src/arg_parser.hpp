#pragma once

#include "strings.hpp"

#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <optional>
#include <cstdint>
#include <iomanip>
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

    size_t totalIdentifierLength() const noexcept
    {
        size_t size{};
        for (auto &id : identifier)
            size += id.length() + 2;

        return size - 2;
    }

    friend std::ostream &operator<<(std::ostream &os, const Argument &argument)
    {
        return os << Join(argument.identifier);
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

    Command &allowsArguments() noexcept
    {
        m_AllowArguments = true;

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

    void printHelp() const noexcept
    {
        std::cout << "Usage: tram " << m_Identifier[0] << (m_Arguments.size() > 0 ? " [Options]" : "") << (m_AllowArguments ? " [Args] " : "") << " \n\n";

        std::cout << "Options:\n";

        for (auto &arg : m_Arguments)
            std::cout << "    " << arg << (arg.isFlag() ? "" : " <value> ") << std::setw(17 - arg.totalIdentifierLength()) << " " << arg.help << "\n";

        std::cout << std::endl;
    }

    bool hasIdentifier(std::string_view identifier) const noexcept
    {
        for (auto &id : m_Identifier)
        {
            if (id == identifier)
                return true;
        }

        return false;
    }

    size_t totalIdentifierLength() const noexcept
    {
        size_t size{};
        for (auto &id : m_Identifier)
            size += id.length() + 2;

        return size - 2;
    }

    friend std::ostream &operator<<(std::ostream &os, const Command &command)
    {
        return os << Join(command.m_Identifier);
    }

private:
    std::vector<std::string> m_Identifier;

    std::string m_Help;

    std::vector<Argument> m_Arguments;

    bool m_AllowArguments;
};

class ArgParser
{
public:
    ArgParser() noexcept
    {
        command("help", "h")
            .help("Shows help.");
    }

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

    void operator()(std::string_view identifier)
    {
        if (identifier.empty())
            std::cout << *this << std::endl;

        auto command = getCommandBy(identifier);

        if (!command.has_value())
        {
            std::cout << "Command '" << identifier << "' not found!" << std::endl;
            return;
        }

        command.value().printHelp();
    }

    friend std::ostream &operator<<(std::ostream &os, const ArgParser &arg_parser)
    {
        os << "Usage: tram [Command] [Options]\n\n";

        os << "Commands:\n";

        for (auto &cmd : arg_parser.m_Commands)
            os << "    " << cmd << std::setw(27 - cmd.totalIdentifierLength()) << " " << cmd.help() << "\n";

        os << "\n";

        os << "See 'tram help <command>' for more information on a specific command.";

        return os;
    }

private:
    std::vector<Command> m_Commands;

    std::optional<Command> getCommandBy(std::string_view identifier)
    {
        for (auto &cmd : m_Commands)
        {
            if (cmd.hasIdentifier(identifier))
                return cmd;
        }

        return std::nullopt;
    }
};
