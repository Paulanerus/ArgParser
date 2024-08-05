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
#include <ostream>
#include <vector>
#include <ranges>
#include <format>
#include <string>

namespace psap // Paul's Simple Argument Parser
{
    namespace color
    {
        namespace state
        {
            enum class color_hue
            {
                reset = 39,
                black = 30,
                red = 31,
                green = 32,
                yellow = 33,
                blue = 34,
                magenta = 35,
                cyan = 36,
                gray = 37,
                light_black = 90,
                light_red = 91,
                light_green = 92,
                light_yellow = 93,
                light_blue = 94,
                light_magenta = 95,
                light_cyan = 96,
                light_gray = 97,
            };

            class color_mode
            {
            public:
                void enable() noexcept
                {
                    m_ColorMode = true;
                }

                void disable() noexcept
                {
                    m_ColorMode = false;
                }

                bool state() const noexcept
                {
                    return m_ColorMode;
                }

            private:
                bool m_ColorMode;
            };

            inline color_mode &global_color() noexcept
            {
                static color_mode mode;

                return mode;
            }

            inline std::string generic_coloring(const std::string &text, state::color_hue hue) noexcept
            {
                if (!global_color().state())
                    return text;

                return std::format("\033[{}m{}\033[{}m", static_cast<std::underlying_type_t<state::color_hue>>(hue), text, static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::reset));
            }
        }

        inline void enableColor() noexcept
        {
            state::global_color().enable();
        }

        inline void disableColor() noexcept
        {
            state::global_color().disable();
        }

        inline bool stateColor() noexcept
        {
            return state::global_color().state();
        }

        inline std::string black(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::black);
        }

        inline std::string red(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::red);
        }

        inline std::string green(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::green);
        }

        inline std::string yellow(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::yellow);
        }

        inline std::string blue(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::blue);
        }

        inline std::string magenta(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::magenta);
        }

        inline std::string cyan(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::cyan);
        }

        inline std::string gray(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::gray);
        }

        inline std::string light_black(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_black);
        }

        inline std::string light_red(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_red);
        }

        inline std::string light_green(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_green);
        }

        inline std::string light_yellow(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_yellow);
        }

        inline std::string light_blue(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_blue);
        }

        inline std::string light_magenta(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_magenta);
        }

        inline std::string light_cyan(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_cyan);
        }

        inline std::string light_gray(const std::string &text) noexcept
        {
            return state::generic_coloring(text, state::color_hue::light_gray);
        }

        inline std::ostream &reset(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::reset) << "m";
            return os;
        }

        inline std::ostream &black(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::black) << "m";
            return os;
        }

        inline std::ostream &red(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::red) << "m";
            return os;
        }

        inline std::ostream &green(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::green) << "m";
            return os;
        }

        inline std::ostream &yellow(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::yellow) << "m";
            return os;
        }

        inline std::ostream &blue(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::blue) << "m";
            return os;
        }

        inline std::ostream &magenta(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::magenta) << "m";
            return os;
        }

        inline std::ostream &cyan(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::cyan) << "m";
            return os;
        }

        inline std::ostream &gray(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::gray) << "m";
            return os;
        }

        inline std::ostream &light_black(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_black) << "m";
            return os;
        }

        inline std::ostream &light_red(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_red) << "m";
            return os;
        }

        inline std::ostream &light_green(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_green) << "m";
            return os;
        }

        inline std::ostream &light_yellow(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_yellow) << "m";
            return os;
        }

        inline std::ostream &light_blue(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_blue) << "m";
            return os;
        }

        inline std::ostream &light_magenta(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_magenta) << "m";
            return os;
        }

        inline std::ostream &light_cyan(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_cyan) << "m";
            return os;
        }

        inline std::ostream &light_gray(std::ostream &os)
        {
            if (stateColor())
                os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_gray) << "m";
            return os;
        }
    }

    namespace string
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
    }

    namespace internal
    {
        template <typename T>
        auto try_catch(T &&t) noexcept -> std::optional<decltype(t())>
        {
            try
            {
                return t();
            }
            catch ([[maybe_unused]] const std::exception &)
            {
                return std::nullopt;
            }
        }
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

        Command &fallback()
        {
            m_Fallback = true;
            return *this;
        }

        bool has(std::string_view option_id) const noexcept
        {
            return std::ranges::any_of(m_Options, [option_id](const Option &opt)
                                       { return opt.active && std::ranges::any_of(opt.identifier, [option_id](const std::string &id)
                                                                                  { return option_id == id; }); });
        }

        bool operator[](std::convertible_to<std::string_view> auto option_id) const noexcept
        {
            return has(option_id);
        }

        template <typename T>
        std::optional<T> get(std::string_view option) const
        {
            auto it = std::ranges::find_if(m_Options, [&option](const Option &opt)
                                           { return std::ranges::any_of(opt.identifier, [&option](const std::string &id)
                                                                        { return option == id; }); });

            if (it == m_Options.end() || it->value.empty())
                return std::nullopt;

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
                return internal::try_catch([&it]
                                           { return static_cast<int16_t>(std::stoi(it->value)); });
            else if constexpr (std::is_same_v<T, uint16_t>)
                return internal::try_catch([&it]
                                           { return static_cast<uint16_t>(std::stoul(it->value)); });
            else if constexpr (std::is_same_v<T, int32_t>)
                return internal::try_catch([&it]
                                           { return std::stoi(it->value); });
            else if constexpr (std::is_same_v<T, uint32_t>)
                return internal::try_catch([&it]
                                           { return static_cast<uint32_t>(std::stoul(it->value)); });
            else if constexpr (std::is_same_v<T, int64_t>)
                return internal::try_catch([&it]
                                           { return std::stol(it->value); });
            else if constexpr (std::is_same_v<T, uint64_t>)
                return internal::try_catch([&it]
                                           { return std::stoul(it->value); });
            else if constexpr (std::is_same_v<T, float>)
                return internal::try_catch([&it]
                                           { return std::stof(it->value); });
            else if constexpr (std::is_same_v<T, double>)
                return internal::try_catch([&it]
                                           { return std::stod(it->value); });
            else if constexpr (std::is_same_v<T, long double>)
                return internal::try_catch([&it]
                                           { return std::stold(it->value); });
            // else if constexpr (std::is_constructible_v<T, std::string>)
            //     return T(it->value);
            else
                return std::nullopt;
        }

        friend ArgParser;

    private:
        std::vector<std::string> m_Identifier;

        std::string m_Help;

        bool m_Fallback;

        std::vector<Option> m_Options;

        std::function<void(const ArgParser &parser, const Command &command)> m_Func;

        void execute(const ArgParser &parser)
        {
            if (m_Func == nullptr)
                return;

            m_Func(parser, *this);
        }

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
        ArgParser(std::string &&app_name, bool color_output = true) noexcept : m_AppName(app_name)
        {
            if (color_output)
                color::enableColor();
        }

        template <typename... Args>
            requires(std::convertible_to<Args, std::string> && ...)
        Command &command(Args &&...args) noexcept
        {
            std::vector<std::string> identifier{std::forward<Args>(args)...};

            return m_Commands.emplace_back(std::move(identifier));
        }

        ArgParser &option(Option &&option)
        {
            m_Options.emplace_back(std::move(option));
            return *this;
        }

        void parse(char *argv[], int argc, std::size_t start = 1)
        {
            if (argc - start == 0)
            {
                auto it = std::ranges::find(m_Commands, true, &Command::m_Fallback);

                if (it != m_Commands.end())
                    it->execute(*this);

                return;
            }

            m_Args.assign(argv + start, argv + argc);

            std::ranges::for_each(m_Args, [](std::string &arg)
                                  { std::transform(arg.begin(), arg.end(), arg.begin(), [](auto c)
                                                   { return std::tolower(c); }); });

            std::size_t command_idx{};
            std::unordered_set<std::size_t> indieces;

            for (std::size_t i{}; i < m_Args.size(); i++)
            {
                if (isCommand(m_Args[i]))
                {
                    indieces.insert(i);
                    command_idx = i;
                    break;
                }

                auto [has, flag] = hasOption(m_Options, m_Args[i]);

                indieces.insert(i);

                if (!has)
                {
                    command_idx = i;
                    break;
                }

                if (flag || i + 1 >= m_Args.size())
                    continue;

                auto [n_has, _] = hasOption(m_Options, m_Args[i + 1]);

                if (!n_has)
                    updateValue(m_Args[i], m_Args[i + 1]);

                indieces.insert(i + 1);
                i++;
            }

            auto command_opt = getCommandBy(m_Args[command_idx]);

            if (!command_opt.has_value())
            {
                std::cout << std::format("Unknown command '{}'\n", color::light_red(m_Args[command_idx]));

                auto similar = getSimilar(m_Args[command_idx]);

                if (!similar.empty())
                    std::cout << std::format("Did you mean: '{}'?", color::green(similar)) << std::endl;

                return;
            }

            auto command = command_opt.value();

            for (std::size_t i = command_idx + 1; i < m_Args.size(); i++)
            {
                auto [c_has, c_flag] = hasOption(command.m_Options, m_Args[i]);

                if (!c_has)
                    continue;

                if (c_flag || i + 1 >= m_Args.size())
                {
                    indieces.insert(i);
                    continue;
                }

                auto [n_has, _] = hasOption(command.m_Options, m_Args[i + 1]);

                if (!n_has)
                    command[m_Args[i]] = m_Args[i + 1];

                indieces.insert({i, i + (n_has ? 0 : 1)});
                i++;
            }

            auto it = std::ranges::remove_if(m_Args, [&](const std::string &arg)
                                             { return indieces.contains(&arg - &m_Args[0]); });

            m_Args.erase(it.begin(), it.end());

            command.execute(*this);
        }

        void print(std::string_view identifier) const noexcept
        {
            if (identifier.empty())
            {
                std::cout << color::yellow("Usage: ") << m_AppName << color::green(" [Options]") << color::cyan(" [Command]") << "\n\n";

                std::cout << color::green("Options:\n");
                for (const auto &opt : m_Options)
                    std::cout << "    " << string::Join(opt.identifier) << std::setw(27 - (std::size_t)opt) << " " << opt.help << "\n";

                std::cout << "\n";

                std::cout << color::cyan("Commands:\n");

                for (const auto &cmd : m_Commands)
                    std::cout << "    " << string::Join(cmd.m_Identifier) << std::setw(27 - (std::size_t)cmd) << " " << cmd.m_Help << "\n";

                std::cout << "\n";

                std::cout
                    << "See '"
                    << color::yellow
                    << m_AppName
                    << " help <command>"
                    << color::reset
                    << "' for more information on a specific command."
                    << std::endl;

                return;
            }

            auto command_result = getCommandBy(identifier);

            if (!command_result)
            {
                std::cout
                    << "Command '"
                    << color::light_red
                    << identifier
                    << color::reset
                    << "' not found!"
                    << std::endl;
                return;
            }

            auto command = command_result.value();

            std::cout
                << color::yellow("Usage: ")
                << m_AppName
                << " "
                << color::cyan(command.m_Identifier[0])
                << (command.m_Options.size() > 0 ? color::green(" [Options]") : "")
                << color::light_red(" [Args]")
                << "\n\n";

            std::cout << color::green("Options:\n");

            for (const auto &arg : command.m_Options)
                std::cout
                    << "    "
                    << string::Join(arg.identifier)
                    << (arg.flag ? "" : " <value> ")
                    << std::setw(17 - (std::size_t)arg)
                    << " "
                    << arg.help
                    << "\n"
                    << std::endl;
        }

        void operator()(std::string_view identifier) const noexcept
        {
            print(identifier);
        }

        const std::vector<std::string> &args() const noexcept
        {
            return m_Args;
        }

        std::string argAt(std::size_t idx) const noexcept
        {
            if (idx < m_Args.size())
                return m_Args[idx];

            return "";
        }

        std::string operator[](std::size_t idx) const noexcept
        {
            return argAt(idx);
        }

        bool has(std::string_view option_id) const noexcept
        {
            return std::ranges::any_of(m_Options, [option_id](const Option &opt)
                                       { return opt.active && std::ranges::any_of(opt.identifier, [option_id](const std::string &id)
                                                                                  { return option_id == id; }); });
        }

        bool operator[](std::convertible_to<std::string_view> auto option_id) const noexcept
        {
            return has(option_id);
        }

        template <typename T>
        std::optional<T> get(std::string_view option) const
        {
            auto it = std::ranges::find_if(m_Options, [&option](const Option &opt)
                                           { return std::ranges::any_of(opt.identifier, [&option](const std::string &id)
                                                                        { return option == id; }); });

            if (it == m_Options.end() || it->value.empty())
                return std::nullopt;

            if constexpr (std::is_convertible_v<std::string, T>)
                return static_cast<T>(it->value);
            else if constexpr (std::is_convertible_v<std::wstring, T>)
                return static_cast<T>(it->value);
            else if constexpr (std::is_same_v<T, int8_t>)
                return static_cast<int8_t>(it->value[0]);
            else if constexpr (std::is_same_v<T, uint8_t>)
                return static_cast<uint8_t>(it->value[0]);
            else if constexpr (std::is_same_v<T, int16_t>)
                return internal::try_catch([&it]
                                           { return static_cast<int16_t>(std::stoi(it->value)); });
            else if constexpr (std::is_same_v<T, uint16_t>)
                return internal::try_catch([&it]
                                           { return static_cast<uint16_t>(std::stoul(it->value)); });
            else if constexpr (std::is_same_v<T, int32_t>)
                return internal::try_catch([&it]
                                           { return std::stoi(it->value); });
            else if constexpr (std::is_same_v<T, uint32_t>)
                return internal::try_catch([&it]
                                           { return static_cast<uint32_t>(std::stoul(it->value)); });
            else if constexpr (std::is_same_v<T, int64_t>)
                return internal::try_catch([&it]
                                           { return std::stol(it->value); });
            else if constexpr (std::is_same_v<T, uint64_t>)
                return internal::try_catch([&it]
                                           { return std::stoul(it->value); });
            else if constexpr (std::is_same_v<T, float>)
                return internal::try_catch([&it]
                                           { return std::stof(it->value); });
            else if constexpr (std::is_same_v<T, double>)
                return internal::try_catch([&it]
                                           { return std::stod(it->value); });
            else if constexpr (std::is_same_v<T, long double>)
                return internal::try_catch([&it]
                                           { return std::stold(it->value); });
            else
                return std::nullopt;
        }

    private:
        const std::string m_AppName;

        std::vector<Option> m_Options;

        std::vector<Command> m_Commands;

        std::vector<std::string> m_Args;

        inline void updateValue(std::string_view option, const std::string &value) noexcept
        {
            auto it = std::ranges::find_if(m_Options, [&option](const Option &opt)
                                           { return std::ranges::any_of(opt.identifier, [&option](const std::string &id)
                                                                        { return option == id; }); });
            if (it == m_Options.end())
                return;

            it->value = value;
        }

        std::pair<bool, bool> hasOption(std::span<Option> options, std::string_view option)
        {
            auto contains_option = [&option](const Option &opt)
            { return std::ranges::find(opt.identifier, option) != opt.identifier.end(); };

            if (auto opt = std::ranges::find_if(options, contains_option); opt != options.end())
            {
                opt->active = true;
                return std::make_pair(true, opt->flag);
            }

            return std::make_pair(false, false);
        }

        bool isCommand(std::string_view identifier) const noexcept
        {
            return std::ranges::any_of(m_Commands, [&identifier](const Command &cmd)
                                       { return std::ranges::any_of(cmd.m_Identifier, [&identifier](const auto &id)
                                                                    { return identifier == id; }); });
        }

        std::optional<Command> getCommandBy(std::string_view identifier) const noexcept
        {
            auto has_identifier = [&](const Command &cmd)
            { return std::ranges::any_of(cmd.m_Identifier, [identifier](const auto &id)
                                         { return id == identifier; }); };

            if (auto cmd = std::ranges::find_if(m_Commands, has_identifier); cmd != m_Commands.end())
                return *cmd;

            return std::nullopt;
        }

        std::string getSimilar(std::string_view identifier) const noexcept
        {
            std::size_t length = std::numeric_limits<std::size_t>::max();

            std::string current_best_match{""};

            for (auto &cmd : m_Commands)
            {
                for (auto &id : cmd.m_Identifier)
                {
                    auto distance = string::LevenshteinDistance(identifier, id);

                    if (distance < length)
                    {
                        length = distance;
                        current_best_match = id;
                    }
                }
            }

            return current_best_match;
        }
    };
}