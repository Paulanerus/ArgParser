#pragma once

#include "strings.hpp"
#include "command.hpp"

#include <type_traits>
#include <iostream>
#include <optional>
#include <cstdint>
#include <iomanip>
#include <ranges>

class ArgParser
{
public:
    ArgParser() noexcept = default;

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
        {
            getCommandBy("help").value()();
            return;
        }

        std::vector<std::string> args{argv + start, argv + argc};

        std::for_each(args.begin(), args.end(), [](std::string &arg)
                      { std::transform(arg.begin(), arg.end(), arg.begin(), [](uint8_t c)
                                       { return std::tolower(c); }); });

        auto command_opt = getCommandBy(args[0]);

        if (!command_opt.has_value())
        {
            std::cout << "Unknown command '" << args[0] << "'\n";
            std::cout << "Did you mean: '" << getSimilar(args[0]) << "'?" << std::endl;
            return;
        }

        command_opt.value()();
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
