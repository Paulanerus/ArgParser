#pragma once

#include "strings.hpp"
#include "levenshtein.hpp"

#include <initializer_list>
#include <type_traits>
#include <functional>
#include <algorithm>
#include <iostream>
#include <optional>
#include <cstdint>
#include <iomanip>
#include <string>
#include <vector>
#include <any>

struct Option
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

    static Option Flag(std::initializer_list<std::string> &&identifier, std::string &&help_txt)
    {
        return Option{
            .identifier = std::move(identifier),
            .help = std::move(help_txt),
            .flag = true,
            .required = false,
            .value = {}};
    }

    template <typename T>
    static Option Default(std::initializer_list<std::string> &&identifier, std::string &&help_txt, T &&any)
    {
        return Option{
            .identifier = std::move(identifier),
            .help = std::move(help_txt),
            .flag = false,
            .required = false,
            .value = std::move(any)};
    }

    static Option Required(std::initializer_list<std::string> &&identifier, std::string &&help_txt)
    {
        return Option{
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

    Command &option(Option &&arg) noexcept
    {
        m_Options.push_back(std::move(arg));

        return *this;
    }

    Command &action(std::function<void(const Command &command)> &&func)
    {
        m_Func = std::move(func);

        return *this;
    }

    void operator()() noexcept
    {
        m_Func(*this);
    }

    bool operator[](std::string_view option_id) const noexcept
    {
        for (auto &option : m_Options)
        {
            for (auto &identifier : option.identifier)
            {
                if (option_id == identifier)
                    return true;
            }
        }

        return false;
    }

    std::string_view help() const noexcept
    {
        return m_Help;
    }

    const std::vector<Option> &options() const noexcept
    {
        return m_Options;
    }

    const std::vector<std::string> &identifier() const noexcept
    {
        return m_Identifier;
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

private:
    std::vector<std::string> m_Identifier;

    std::string m_Help;

    std::vector<Option> m_Options;

    std::function<void(const Command &command)> m_Func;
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

    void parse(char *argv[], int argc, size_t start = 1)
    {
        if (argc - start == 0)
            return;

        std::vector<std::string> args{argv + start, argv + argc};

        std::for_each(args.begin(), args.end(), [](std::string &arg)
                      { std::transform(arg.begin(), arg.end(), arg.begin(), [](uint8_t c)
                                       { return std::tolower(c); }); });

        auto command_opt = getCommandBy(args[0]);

        if (!command_opt.has_value())
        {
            std::cout << "Unknown command '" << args[0] << "'\n";
            std::cout << "Did you mean '" << getSimilar(args[0]) << "'?" << std::endl;
            return;
        }

        std::cout << "Command execute: " << args[0] << std::endl;
        // command_opt.value()();
    }

    void operator()(std::string_view identifier)
    {
        if (identifier.empty())
        {
            std::cout << "Usage: tram [Command] [Options]\n\n";

            std::cout << "Commands:\n";

            for (auto &cmd : m_Commands)
                std::cout << "    " << Join(cmd.identifier()) << std::setw(27 - cmd.totalIdentifierLength()) << " " << cmd.help() << "\n";

            std::cout << "\n";

            std::cout << "See 'tram help <command>' for more information on a specific command." << std::endl;

            return;
        }

        auto command_result = getCommandBy(identifier);

        if (!command_result)
        {
            std::cout << "Command '" << identifier << "' not found!" << std::endl;
            return;
        }

        auto command = command_result.value();

        std::cout << "Usage: tram " << command.identifier()[0] << (command.options().size() > 0 ? " [Options]" : "") << " [Args] \n\n";

        std::cout << "Options:\n";

        for (auto &arg : command.options())
            std::cout << "    " << Join(arg.identifier) << (arg.isFlag() ? "" : " <value> ") << std::setw(17 - arg.totalIdentifierLength()) << " " << arg.help << "\n";

        std::cout << std::endl;
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

    std::string getSimilar(std::string_view identifier)
    {
        size_t current_dist = 100;
        std::string most_similar;
        for (auto &cmd : m_Commands)
        {
            auto dist = calculateDistance(identifier, cmd.identifier()[0]);

            if (dist < current_dist)
            {
                current_dist = dist;
                most_similar = cmd.identifier()[0];
            }
        }

        return most_similar;
    }
};
