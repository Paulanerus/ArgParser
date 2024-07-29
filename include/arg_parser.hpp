#pragma once

#include <unordered_set>
#include <string_view>
#include <type_traits>
#include <functional>
#include <algorithm>
#include <optional>
#include <iostream>
#include <concepts>
#include <iomanip>
#include <utility>
#include <sstream>
#include <vector>
#include <ranges>
#include <format>
#include <string>

namespace psap // Paul's Simple Argument Parser
{
    inline std::string Join(const std::vector<std::string> &vec, const char *delimiter = ", ") noexcept
    {
        std::ostringstream stream;

        auto begin = vec.begin(), end = vec.end();

        if (begin != end)
        {
            std::copy(begin, std::prev(end), std::ostream_iterator<std::string>(stream, delimiter));
            begin = prev(end);
        }

        if (begin != end)
            stream << *begin;

        return stream.str();
    }

    inline std::size_t LevenshteinDistance(std::convertible_to<std::string_view> auto src, std::convertible_to<std::string_view> auto target) noexcept
    {
        if (src == target)
            return 0;

        if (src.empty() || target.empty())
            return std::max(src.length(), target.length());

        std::vector<std::size_t> distance(target.length() + 1);

        for (std::size_t i{}; i < src.length(); i++)
        {
            distance[0] = i + 1;

            auto corner = i;

            for (size_t j{}; j < target.length(); j++)
            {
                auto upper = distance[j + 1];

                distance[j + 1] = src[i] == target[j] ? corner : std::min({upper, corner, distance[j]}) + 1;
                corner = upper;
            }
        }

        return distance[target.length()];
    }

    class ArgParser;

    struct Option
    {
        std::vector<std::string> identifier;

        std::string help;

        bool flag;

        bool active;

        std::string value;

        static Option Flag(std::initializer_list<std::string> &&identifier, std::string &&help_txt)
        {
            return Option{
                .identifier = std::move(identifier),
                .help = std::move(help_txt),
                .flag = true,
                .active = false,
                .value = {}};
        }

        static Option Value(std::initializer_list<std::string> &&identifier, std::string &&help_txt, std::string &&val = "")
        {
            return Option{
                .identifier = std::move(identifier),
                .help = std::move(help_txt),
                .flag = false,
                .active = false,
                .value = std::move(val)};
        }

        friend ArgParser;

    private:
        operator std::size_t() const noexcept
        {
            std::size_t size{};
            for (auto &id : identifier)
                size += id.length() + 2;

            return size - 2;
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

        Command &action(std::function<void(const ArgParser &parser, const Command &command)> &&func) noexcept
        {
            m_Func = std::move(func);

            return *this;
        }

        void execute(const ArgParser &parser)
        {
            if (m_Func == nullptr)
                return;

            m_Func(parser, *this);
        }

        bool operator[](std::convertible_to<std::string_view> auto option_id) const noexcept
        {
            return std::ranges::any_of(m_Options, [option_id](const Option &opt)
                                       { return opt.active && std::ranges::any_of(opt.identifier, [option_id](const std::string &id)
                                                                                  { return option_id == id; }); });
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

        template <typename T>
        std::optional<T> get(std::string_view option) const
        {
            auto it = std::ranges::find_if(m_Options, [&option](const Option &opt)
                                           { return std::ranges::any_of(opt.identifier, [&option](const std::string &id)
                                                                        { return option == id; }); });

            if (it == m_Options.end() || it->value.empty())
                return std::nullopt;

            // TODO return default value

            // Doesn't look that good.. ~Paul
            if constexpr (std::is_convertible_v<std::string, T>)
                return static_cast<T>(it->value);
            else if constexpr (std::is_convertible_v<std::wstring, T>)
                return static_cast<T>(it->value);
            else if constexpr (std::is_same_v<T, int8_t>)
                return static_cast<int8_t>(it->value[0]);
            else if constexpr (std::is_same_v<T, uint8_t>)
                return static_cast<uint8_t>(it->value[0]);
            else if constexpr (std::is_same_v<T, int16_t>)
            {
                try
                {
                    return static_cast<int16_t>(std::stoi(it->value));
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, uint16_t>)
            {
                try
                {
                    return static_cast<uint16_t>(std::stoul(it->value));
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, int32_t>)
            {
                try
                {
                    return std::stoi(it->value);
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, uint32_t>)
            {
                try
                {
                    return static_cast<uint32_t>(std::stoul(it->value));
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, int64_t>)
            {
                try
                {
                    return std::stol(it->value);
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, uint64_t>)
            {
                try
                {
                    return std::stoul(it->value);
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                try
                {
                    return std::stof(it->value);
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                try
                {
                    return std::stod(it->value);
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            else if constexpr (std::is_same_v<T, long double>)
            {
                try
                {
                    return std::stold(it->value);
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }
            // else if constexpr (std::is_constructible_v<T, std::string>)
            //     return T(it->value);
            else
                return std::nullopt;
        }

        friend ArgParser;

    private:
        std::vector<std::string> m_Identifier;

        std::string m_Help;

        std::vector<Option> m_Options;

        std::function<void(const ArgParser &parser, const Command &command)> m_Func;

        std::string &operator[](const std::string &option) noexcept
        {
            static std::string fallback;

            auto it = std::ranges::find_if(m_Options, [&option](const Option &opt)
                                           { return std::ranges::any_of(opt.identifier, [&option](const std::string &id)
                                                                        { return option == id; }); });
            if (it == m_Options.end())
                return fallback; // Should never happen...

            return it->value;
        }

        std::pair<bool, bool> hasOption(const std::string &opt_str) noexcept
        {
            auto contains_option = [&opt_str](const auto &opt)
            { return std::ranges::find(opt.identifier, opt_str) != opt.identifier.end(); };

            if (auto opt = std::ranges::find_if(m_Options, contains_option); opt != m_Options.end())
            {
                opt->active = true;
                return std::make_pair(true, opt->flag);
            }

            return std::make_pair(false, false);
        }

        operator std::size_t() const noexcept
        {
            std::size_t size{};
            for (auto &id : m_Identifier)
                size += id.length() + 2;

            return size - 2;
        }
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

        void parse(char *argv[], int argc, std::size_t start = 1)
        {
            if (argc - start == 0)
                return;

            m_Args.assign(argv + start, argv + argc);

            std::for_each(m_Args.begin(), m_Args.end(), [](std::string &arg)
                          { std::transform(arg.begin(), arg.end(), arg.begin(), [](auto c)
                                           { return std::tolower(c); }); });

            auto command_opt = getCommandBy(m_Args[0]);

            if (!command_opt.has_value())
            {
                std::cout << "Unknown command '" << m_Args[0] << "'\n";
                std::cout << "Did you mean: '" << getSimilar(m_Args[0]) << "'?" << std::endl;
                return;
            }

            auto command = command_opt.value();

            std::unordered_set<std::size_t> indieces{0};
            for (std::size_t i{}; i < m_Args.size(); i++)
            {
                auto [c_has, c_flag] = command.hasOption(m_Args[i]);

                if (!c_has)
                    continue;

                if (c_flag || i + 1 >= m_Args.size())
                {
                    indieces.insert(i);
                    continue;
                }

                auto [n_has, _] = command.hasOption(m_Args[i + 1]);

                if (n_has)
                {
                    indieces.insert(i);
                    continue;
                }

                command[m_Args[i]] = m_Args[i + 1];

                indieces.insert({i, i + 1});
            }

            auto it = std::ranges::remove_if(m_Args, [&](const std::string &arg)
                                             { return indieces.contains(&arg - &m_Args[0]); });

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

        std::string operator[](std::size_t idx) const noexcept
        {
            if (idx < m_Args.size())
                return m_Args[idx];

            return "";
        }

    private:
        const std::string m_AppName;

        std::vector<Command> m_Commands;

        std::vector<std::string> m_Args;

        std::optional<Command> getCommandBy(std::string_view identifier) const noexcept
        {
            auto has_identifier = [&](const Command &cmd)
            { return std::ranges::any_of(cmd.identifier(), [identifier](const auto &id)
                                         { return id == identifier; }); };

            if (auto cmd = std::ranges::find_if(m_Commands, has_identifier); cmd != m_Commands.end())
                return *cmd;

            return std::nullopt;
        }

        std::string getSimilar(std::string_view identifier) const noexcept
        {
            auto distance_compare = [identifier](const auto &lhs, const auto &rhs)
            { return LevenshteinDistance(identifier, lhs.identifier()[0]) < LevenshteinDistance(identifier, rhs.identifier()[0]); };

            if (auto min_distance = std::ranges::min_element(m_Commands, distance_compare); min_distance != m_Commands.end())
                return min_distance->identifier()[0];

            return "help";
        }
    };
}