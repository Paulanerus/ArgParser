#pragma once

#include "strings.hpp"

#include <initializer_list>
#include <type_traits>
#include <string_view>
#include <functional>
#include <algorithm>
#include <iostream>
#include <optional>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <string>
#include <format>
#include <ranges>
#include <any>

class ArgParser;

struct Option
{
    std::vector<std::string> identifier;

    std::string help;

    bool flag;

    bool active;

    std::any value;

    operator std::size_t() const noexcept
    {
        std::size_t size{};
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
            .active = false,
            .value = {}};
    }

    template <typename T>
    static Option Default(std::initializer_list<std::string> &&identifier, std::string &&help_txt, T &&any)
    {
        return Option{
            .identifier = std::move(identifier),
            .help = std::move(help_txt),
            .flag = false,
            .active = false,
            .value = std::move(any)};
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

    Command &action(std::function<void(const ArgParser &parser, const Command &command)> &&func)
    {
        m_Func = std::move(func);

        return *this;
    }

    void execute(const ArgParser &parser) noexcept
    {
        if (m_Func == nullptr)
            return;

        m_Func(parser, *this);
    }

    template <typename Arg>
        requires(std::convertible_to<Arg, std::string_view>)
    bool operator[](Arg option_id) const noexcept
    {
        return std::ranges::any_of(m_Options, [option_id](const Option &opt)
                                   { return opt.active && std::ranges::any_of(opt.identifier, [option_id](const std::string &id)
                                                                              { return option_id == id; }); });
    }

    bool hasOption(const std::string &opt_str)
    {
        auto contains_option = [&opt_str](const auto &opt)
        { return std::ranges::find(opt.identifier, opt_str) != opt.identifier.end(); };

        if (auto opt = std::ranges::find_if(m_Options, contains_option); opt != m_Options.end())
        {
            opt->active = true;
            return true;
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
        return std::ranges::any_of(m_Identifier, [identifier](const auto &id)
                                   { return id == identifier; });
    }

    operator std::size_t() const noexcept
    {
        std::size_t size{};
        for (auto &id : m_Identifier)
            size += id.length() + 2;

        return size - 2;
    }

private:
    std::vector<std::string> m_Identifier;

    std::string m_Help;

    std::vector<Option> m_Options;

    std::function<void(const ArgParser &parser, const Command &command)> m_Func;
};

class ArgParser
{
public:
    ArgParser(std::string &&app_name) noexcept : m_AppName(app_name) {}

    template <typename... Args>
        requires(std::convertible_to<Args, std::string> && ...)
    Command &command(Args &&...args) noexcept
    {
        std::vector<std::string> identifier{std::forward<Args>(args)...};

        return m_Commands.emplace_back(std::move(identifier));
    }

    void parse(char *argv[], int argc, size_t start = 1)
    {
        if (argc - start == 0)
            return;

        m_Args.assign(argv + start, argv + argc);

        std::for_each(m_Args.begin(), m_Args.end(), [](std::string &arg)
                      { std::transform(arg.begin(), arg.end(), arg.begin(), [](uint8_t c)
                                       { return std::tolower(c); }); });

        auto command_opt = getCommandBy(m_Args[0]);

        if (!command_opt.has_value())
        {
            std::cout << "Unknown command '" << m_Args[0] << "'\n";
            std::cout << "Did you mean: '" << getSimilar(m_Args[0]) << "'?" << std::endl;
            return;
        }

        auto command = command_opt.value();

        auto it = std::ranges::remove_if(m_Args, [&](const std::string &arg)
                                         { return arg == m_Args[0] || command.hasOption(arg); });

        m_Args.erase(it.begin(), it.end());

        command.execute(*this);
    }

    void operator()(std::string_view identifier) const
    {
        if (identifier.empty())
        {
            std::cout << std::format("Usage: {} [Command] [Options]\n\n", m_AppName);

            std::cout << "Commands:\n";

            for (auto &cmd : m_Commands)
                std::cout << "    " << Join(cmd.identifier()) << std::setw(27 - (std::size_t)cmd) << " " << cmd.help() << "\n";

            std::cout << "\n";

            std::cout << std::format("See '{} help <command>' for more information on a specific command.", m_AppName) << std::endl;

            return;
        }

        auto command_result = getCommandBy(identifier);

        if (!command_result)
        {
            std::cout << "Command '" << identifier << "' not found!" << std::endl;
            return;
        }

        auto command = command_result.value();

        std::cout << std::format("Usage: {} {} {} [Args] \n\n", m_AppName, command.identifier()[0], command.options().size() > 0 ? "[Options]" : "");

        std::cout << "Options:\n";

        for (auto &arg : command.options())
            std::cout << "    " << Join(arg.identifier) << (arg.flag ? "" : " <value> ") << std::setw(17 - (std::size_t)arg) << " " << arg.help << "\n";

        std::cout << std::endl;
    }

    const std::vector<std::string> &args() const noexcept
    {
        return m_Args;
    }

    std::string operator[](size_t idx) const noexcept
    {
        if (idx < m_Args.size())
            return m_Args[idx];

        return "";
    }

private:
    const std::string m_AppName;

    std::vector<Command> m_Commands;

    std::vector<std::string> m_Args;

    std::optional<Command> getCommandBy(std::string_view identifier) const
    {
        auto has_identifier = [identifier](const Command &cmd)
        { return cmd.hasIdentifier(identifier); };

        if (auto cmd = std::ranges::find_if(m_Commands, has_identifier); cmd != m_Commands.end())
            return *cmd;

        return std::nullopt;
    }

    std::string getSimilar(std::string_view identifier) const
    {
        auto distance_compare = [identifier](const auto &lhs, const auto &rhs)
        { return LevenshteinDistance(identifier, lhs.identifier()[0]) < LevenshteinDistance(identifier, rhs.identifier()[0]); };

        if (auto min_distance = std::ranges::min_element(m_Commands, distance_compare); min_distance != m_Commands.end())
            return min_distance->identifier()[0];

        return "help";
    }
};
