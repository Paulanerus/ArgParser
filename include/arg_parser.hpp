#pragma once

#ifndef AP_CXX17
#    define AP_CXX17 201703L
#endif

#ifndef AP_CXX20
#    define AP_CXX20 202002L
#endif

#if defined(_MSVC_LANG) && defined(_MSC_VER) && 190024210 <= _MSC_FULL_VER
#    define AP_CXX_STD_VER _MSVC_LANG
#else
#    define AP_CXX_STD_VER __cplusplus
#endif

#if AP_CXX_STD_VER < AP_CXX17
#    error "ArgParser requires C++17."
#endif

#if !defined(__has_include)
#    define __has_include(x) 0
#endif

#if AP_CXX_STD_VER >= AP_CXX20
#    if __has_include(<format>)
#        define AP_HAS_FORMAT
#    endif
#endif

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

#if defined(AP_HAS_FORMAT)
#    include <format>
#endif

namespace psap // Paul's Simple Argument Parser
{
namespace color {
    namespace state {
        enum class color_hue {
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

        class color_mode {
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

        inline color_mode& global_color() noexcept
        {
            static color_mode mode;

            return mode;
        }

        inline std::string generic_coloring(const std::string& text, state::color_hue hue) noexcept
        {
            if (!global_color().state())
                return text;

#ifdef AP_HAS_FORMAT
            return std::format("\033[{}m{}\033[{}m", static_cast<std::underlying_type_t<state::color_hue>>(hue), text, static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::reset));
#else
            std::ostringstream stream;
            stream << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(hue) << "m";
            stream << text;
            stream << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::reset) << "m";

            return stream.str();
#endif // AP_HAS_FORMAT
        }
    }

    inline void enable_color() noexcept
    {
        state::global_color().enable();
    }

    inline void disable_color() noexcept
    {
        state::global_color().disable();
    }

    inline bool color_state() noexcept
    {
        return state::global_color().state();
    }

    inline std::string black(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::black);
    }

    inline std::string red(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::red);
    }

    inline std::string green(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::green);
    }

    inline std::string yellow(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::yellow);
    }

    inline std::string blue(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::blue);
    }

    inline std::string magenta(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::magenta);
    }

    inline std::string cyan(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::cyan);
    }

    inline std::string gray(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::gray);
    }

    inline std::string light_black(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_black);
    }

    inline std::string light_red(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_red);
    }

    inline std::string light_green(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_green);
    }

    inline std::string light_yellow(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_yellow);
    }

    inline std::string light_blue(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_blue);
    }

    inline std::string light_magenta(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_magenta);
    }

    inline std::string light_cyan(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_cyan);
    }

    inline std::string light_gray(const std::string& text) noexcept
    {
        return state::generic_coloring(text, state::color_hue::light_gray);
    }

    inline std::ostream& reset(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::reset) << "m";
        return os;
    }

    inline std::ostream& black(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::black) << "m";
        return os;
    }

    inline std::ostream& red(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::red) << "m";
        return os;
    }

    inline std::ostream& green(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::green) << "m";
        return os;
    }

    inline std::ostream& yellow(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::yellow) << "m";
        return os;
    }

    inline std::ostream& blue(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::blue) << "m";
        return os;
    }

    inline std::ostream& magenta(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::magenta) << "m";
        return os;
    }

    inline std::ostream& cyan(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::cyan) << "m";
        return os;
    }

    inline std::ostream& gray(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::gray) << "m";
        return os;
    }

    inline std::ostream& light_black(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_black) << "m";
        return os;
    }

    inline std::ostream& light_red(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_red) << "m";
        return os;
    }

    inline std::ostream& light_green(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_green) << "m";
        return os;
    }

    inline std::ostream& light_yellow(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_yellow) << "m";
        return os;
    }

    inline std::ostream& light_blue(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_blue) << "m";
        return os;
    }

    inline std::ostream& light_magenta(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_magenta) << "m";
        return os;
    }

    inline std::ostream& light_cyan(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_cyan) << "m";
        return os;
    }

    inline std::ostream& light_gray(std::ostream& os)
    {
        if (color_state())
            os << "\033[" << static_cast<std::underlying_type_t<state::color_hue>>(state::color_hue::light_gray) << "m";
        return os;
    }
}

namespace string {
    inline std::string join_strings(const std::vector<std::string>& vec, const char* delimiter = ", ") noexcept
    {
        std::ostringstream stream;

        auto begin = vec.begin(), end = vec.end();

        if (begin != end) {
            std::copy(begin, std::prev(end), std::ostream_iterator<std::string>(stream, delimiter));
            begin = prev(end);
        }

        if (begin != end)
            stream << *begin;

        return stream.str();
    }

    inline std::size_t calc_levenshtein_distance(std::string_view src, std::string_view target) noexcept
    {
        if (src == target)
            return 0;

        if (src.empty() || target.empty())
            return std::max(src.length(), target.length());

        std::vector<std::size_t> distance(target.length() + 1);

        for (std::size_t i {}; i < src.length(); i++) {
            distance[0] = i + 1;

            auto corner = i;

            for (std::size_t j {}; j < target.length(); j++) {
                auto upper = distance[j + 1];

                distance[j + 1] = src[i] == target[j] ? corner : std::min({ upper, corner, distance[j] }) + 1;
                corner = upper;
            }
        }

        return distance[target.length()];
    }

    inline bool starts_with(std::string_view str, std::string_view start) noexcept
    {
#if AP_CXX_STD_VER >= AP_CXX20
        return str.starts_with(start);
#else
        return str.size() >= start.size() && std::equal(start.begin(), start.end(), str.begin());
#endif // AP_CXX_STD_VER >= AP_CXX20
    }

    inline void convert_str_to_lower(std::string& str) noexcept
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    }

    inline void convert_str_to_lower_partly(std::string& str, std::size_t start = 0, std::size_t end = std::numeric_limits<std::size_t>::max()) noexcept
    {
        std::transform(str.begin() + (start >= end || start >= str.length() ? 0 : start), (end >= str.length() ? str.end() : str.begin() + end), str.begin() + (start >= end || start >= str.length() ? 0 : start), [](unsigned char c) { return std::tolower(c); });
    }

    inline std::string copy_str_to_lower(std::string str) noexcept
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });

        return str;
    }
}

namespace internal {
    template<typename T>
    inline auto try_catch(T&& t) noexcept -> std::optional<decltype(t())>
    {
        try {
            return t();
        } catch ([[maybe_unused]] const std::exception&) {
            return std::nullopt;
        }
    }

    template<typename T>
    inline std::optional<T> convert_value(const std::string& value) noexcept
    {
        if constexpr (std::is_convertible_v<std::string, T>)
            return static_cast<T>(value);
        else if constexpr (std::is_convertible_v<std::wstring, T>)
            return static_cast<T>(value);
        else if constexpr (std::is_same_v<T, std::int8_t>)
            return static_cast<int8_t>(value[0]);
        else if constexpr (std::is_same_v<T, std::uint8_t>)
            return static_cast<uint8_t>(value[0]);
        else if constexpr (std::is_same_v<T, std::int16_t>)
            return internal::try_catch([&value] { return static_cast<std::int16_t>(std::stoi(value)); });
        else if constexpr (std::is_same_v<T, std::uint16_t>)
            return internal::try_catch([&value] { return static_cast<std::uint16_t>(std::stoul(value)); });
        else if constexpr (std::is_same_v<T, std::int32_t>)
            return internal::try_catch([&value] { return std::stoi(value); });
        else if constexpr (std::is_same_v<T, std::uint32_t>)
            return internal::try_catch([&value] { return static_cast<std::uint32_t>(std::stoul(value)); });
        else if constexpr (std::is_same_v<T, std::int64_t>)
            return internal::try_catch([&value] { return std::stol(value); });
        else if constexpr (std::is_same_v<T, std::uint64_t>)
            return internal::try_catch([&value] { return std::stoul(value); });
        else if constexpr (std::is_same_v<T, float>)
            return internal::try_catch([&value] { return std::stof(value); });
        else if constexpr (std::is_same_v<T, double>)
            return internal::try_catch([&value] { return std::stod(value); });
        else if constexpr (std::is_same_v<T, long double>)
            return internal::try_catch([&value] { return std::stold(value); });
        else
            return std::nullopt;
    }
}

enum class UnknownOptionPolicy {
    Ignore,
    ReportIgnore,
    Remove,
    ReportRemove,
};

enum class ValueStyle {
    Space,
    EqualSign,
    Both,
};

struct ParserConf {
    std::string name;
    std::size_t padding { 4 };
    bool color_output { true };
    ValueStyle value_style { ValueStyle::Both };
    UnknownOptionPolicy unknown_option_policy { UnknownOptionPolicy::Ignore };
};

class ArgParser;

class Command;

struct Option {
    std::vector<std::string> identifier;

    std::string help;

    bool flag;

    bool active;

    std::string value;

    static Option Flag(std::initializer_list<std::string>&& identifier, std::string&& help_txt)
    {
        return Option {
            std::move(identifier), std::move(help_txt), true, false, ""
        };
    }

    static Option Value(std::initializer_list<std::string>&& identifier, std::string&& help_txt)
    {
        return Option {
            std::move(identifier), std::move(help_txt), false, false, ""
        };
    }

    friend ArgParser;

    friend Command;

private:
    std::size_t length() const noexcept
    {
        std::size_t size {};
        for (auto& id : identifier)
            size += id.length() + 2;

        return size - (flag ? 2 : -6);
    }
};

class Command {
public:
    Command(std::vector<std::string>&& identifier) noexcept
        : m_Identifier(std::move(identifier))
    {
    }

    Command& help(std::string&& help) noexcept
    {
        m_Help = std::move(help);
        return *this;
    }

    Command& option(Option&& arg) noexcept
    {
        std::size_t length = arg.length();

        if (length > m_MaxLength)
            m_MaxLength = length;

        m_Options.push_back(std::move(arg));
        return *this;
    }

    Command& action(std::function<void(const ArgParser& parser, const Command& command)>&& func) noexcept
    {
        m_Func = std::move(func);
        return *this;
    }

    Command& fallback()
    {
        m_Fallback = true;
        return *this;
    }

    bool has(std::string_view option_id) const noexcept
    {
        for (auto& opt : m_Options) {
            if (!opt.active)
                continue;

            for (auto& id : opt.identifier) {
                if (option_id == id)
                    return true;
            }
        }

        return false;
    }

    template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string_view>::value>>
    bool operator[](T option_id) const noexcept
    {
        return has(option_id);
    }

    template<typename T>
    std::optional<T> get(std::string_view option) const
    {
        auto it = std::find_if(m_Options.begin(), m_Options.end(), [&option](const Option& opt) { return std::any_of(opt.identifier.begin(), opt.identifier.end(), [&option](const std::string& id) { return option == id; }); });

        if (it == m_Options.end() || it->value.empty())
            return std::nullopt;

        return internal::convert_value<T>(it->value);
    }

    friend ArgParser;

private:
    std::vector<std::string> m_Identifier;

    std::string m_Help;

    bool m_Fallback;

    std::vector<Option> m_Options;

    std::size_t m_MaxLength {};

    std::function<void(const ArgParser& parser, const Command& command)> m_Func;

    void execute(const ArgParser& parser)
    {
        if (m_Func == nullptr)
            return;

        m_Func(parser, *this);
    }

    std::string& operator[](const std::string& option) noexcept
    {
        static std::string fallback;

        auto it = std::find_if(m_Options.begin(), m_Options.end(), [&option](const Option& opt) { return std::any_of(opt.identifier.begin(), opt.identifier.end(), [&option](const std::string& id) { return string::starts_with(option, id); }); });
        if (it == m_Options.end())
            return fallback; // Should never happen...

        return it->value;
    }

    std::size_t length() const noexcept
    {
        std::size_t size {};
        for (auto& id : m_Identifier)
            size += id.length() + 2;

        return size - 2;
    }
};

class ArgParser {
public:
    ArgParser(ParserConf&& conf) noexcept
        : m_Conf(std::move(conf))
    {
        if (m_Conf.color_output)
            color::enable_color();
    }

    template<typename... Args, typename = std::enable_if_t<(std::is_convertible_v<Args, std::string> && ...)>>
    Command& command(Args&&... args) noexcept
    {
        std::vector<std::string> identifier { std::forward<Args>(args)... };

        Command& cmd = m_Commands.emplace_back(std::move(identifier));

        auto length = cmd.length();
        if (length > m_MaxLength)
            m_MaxLength = length;

        return cmd;
    }

    ArgParser& option(Option&& option)
    {
        auto length = option.length();
        if (length > m_MaxLength)
            m_MaxLength = length;

        m_Options.emplace_back(std::move(option));
        return *this;
    }

    void parse(char* argv[], int argc, std::size_t start = 1)
    {
        if (argc - start == 0) {
            auto it = std::find_if(m_Commands.begin(), m_Commands.end(), [](const Command& cmd) { return cmd.m_Fallback == true; });

            if (it != m_Commands.end())
                it->execute(*this);

            return;
        }

        m_Args.assign(argv + start, argv + argc);

        std::optional<Command> command;
        std::unordered_set<std::size_t> indices;
        std::size_t first_non_flag_idx = m_Args.size();

        for (std::size_t i {}; i < m_Args.size(); i++) {

            if (string::starts_with(m_Args[i], "-")) {

                if (m_Conf.value_style == ValueStyle::Space)
                    string::convert_str_to_lower(m_Args[i]);
                else
                    string::convert_str_to_lower_partly(m_Args[i], 0, m_Args[i].find_first_of("="));

                auto [option_found, is_flag, has_value] = is_option(command.has_value() ? command->m_Options : m_Options, m_Args[i]);

                if (!option_found) {
                    if (first_non_flag_idx != m_Args.size() && !command.has_value())
                        continue;

                    switch (m_Conf.unknown_option_policy) {
                    case UnknownOptionPolicy::Remove:
                        indices.insert(i);

                        continue;
                    case UnknownOptionPolicy::ReportIgnore:
                        std::cout << color::light_yellow("Warning:") << " The Option: '" << m_Args[i] << "' is not available." << std::endl;

                        continue;
                    case UnknownOptionPolicy::ReportRemove:
                        std::cout << color::light_yellow("Warning:") << " The Option: '" << m_Args[i] << "' is not available." << std::endl;
                        indices.insert(i);

                        continue;
                    default:
                        continue;
                    }
                }

                indices.insert(i);

                if (is_flag)
                    continue;

                if (has_value) {
                    
                    auto val = m_Args[i].substr(m_Args[i].find_first_of('=') + 1);

                    if (command.has_value())
                        (*command)[m_Args[i]] = val;
                    else
                        update_value(m_Args[i], val);
                } else {

                    if (i + 1 >= m_Args.size())
                        continue; // FIXME: report on missing value.

                    auto [next_option_found, _, __] = is_option(command.has_value() ? command->m_Options : m_Options, m_Args[i + 1]);

                    if (next_option_found)
                        continue; // FIXME: report on missing value.

                    if (command.has_value())
                        (*command)[m_Args[i]] = m_Args[i + 1];
                    else
                        update_value(m_Args[i], m_Args[i + 1]);

                    indices.insert(i + 1);
                    i++;
                }
            } else {

                if (!command.has_value()) {
                    command = find_command_by_id(m_Args[i]);

                    if (command.has_value()) {
                        indices.insert(i);
                        continue;
                    }
                }

                if (i < first_non_flag_idx)
                    first_non_flag_idx = i;
            }
        }

        if (!command.has_value()) {
            std::cout << color::light_red("Error:") << " Unknown command '" << color::light_red(m_Args[first_non_flag_idx]) << "'\n";

            auto similar = get_similar(m_Args[first_non_flag_idx]);

            if (!similar.empty())
                std::cout << "Did you mean: '" << color::green(similar) << "'?" << std::endl;

            return;
        }

        auto it = std::remove_if(m_Args.begin(), m_Args.end(), [&](const std::string& arg) { return indices.find(&arg - &m_Args[0]) != indices.end(); });

        m_Args.erase(it, m_Args.end());

        command->execute(*this);
    }

    void print(std::string_view identifier) const noexcept
    {
        if (identifier.empty()) {
            std::cout << color::yellow("Usage: ") << m_Conf.name << (m_Options.empty() ? "" : color::green(" [Options]")) << color::cyan(" [Command]") << "\n\n";

            if (!m_Options.empty()) {
                std::cout << color::green("Options:\n");
                for (const auto& opt : m_Options)
                    std::cout << "    " << string::join_strings(opt.identifier) << (!opt.flag ? (m_Conf.value_style == ValueStyle::EqualSign ? "=<value>" : " <value>") : "") << std::setw((m_MaxLength + 1 + m_Conf.padding) - opt.length()) << " " << opt.help << "\n";

                std::cout << "\n";
            }

            std::cout << color::cyan("Commands:\n");

            for (const auto& cmd : m_Commands)
                std::cout << "    " << string::join_strings(cmd.m_Identifier) << std::setw((m_MaxLength + 1 + m_Conf.padding) - cmd.length()) << " " << cmd.m_Help << "\n";

            std::cout << "\n";

            std::cout
                << "See '"
                << color::yellow
                << m_Conf.name
                << " help <command>"
                << color::reset
                << "' for more information on a specific command."
                << std::endl;

            return;
        }

        auto command_result = find_command_by_id(identifier);

        if (!command_result) {
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
            << m_Conf.name
            << " "
            << color::cyan(command.m_Identifier[0])
            << (command.m_Options.size() > 0 ? color::green(" [Options]") : "")
            << color::light_red(" [Args]")
            << "\n\n";

        if (command.m_Options.empty())
            std::cout << color::yellow("No options available.") << std::endl;
        else {
            std::cout << color::green("Options:\n");

            for (const auto& opt : command.m_Options)
                std::cout
                    << "    "
                    << string::join_strings(opt.identifier)
                    << (opt.flag ? "" : (m_Conf.value_style == ValueStyle::Space ? "=<value>" : " <value>"))
                    << std::setw((command.m_MaxLength + 1 + m_Conf.padding) - opt.length())
                    << " "
                    << opt.help
                    << "\n"
                    << std::endl;
        }
    }

    void operator()(std::string_view identifier) const noexcept
    {
        print(identifier);
    }

    const std::vector<std::string>& args() const noexcept
    {
        return m_Args;
    }

    std::string arg_at(std::size_t idx) const noexcept
    {
        if (idx < m_Args.size())
            return m_Args[idx];

        return "";
    }

    std::string operator[](std::size_t idx) const noexcept
    {
        return arg_at(idx);
    }

    bool has(std::string_view option_id) const noexcept
    {
        for (auto& opt : m_Options) {
            if (!opt.active)
                continue;

            for (auto& id : opt.identifier) {
                if (option_id == id)
                    return true;
            }
        }

        return false;
    }

    template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string_view>::value>>
    bool operator[](T option_id) const noexcept
    {
        return has(option_id);
    }

    template<typename T>
    std::optional<T> get(std::string_view option) const
    {
        auto it = std::find_if(m_Options.begin(), m_Options.end(), [&option](const Option& opt) { return std::any_of(opt.identifier.begin(), opt.identifier.end(), [&option](const std::string& id) { return option == id; }); });

        if (it == m_Options.end() || it->value.empty())
            return std::nullopt;

        return internal::convert_value<T>(it->value);
    }

private:
    const ParserConf m_Conf;

    std::vector<Option> m_Options;

    std::vector<Command> m_Commands;

    std::vector<std::string> m_Args;

    std::size_t m_MaxLength {};

    void update_value(const std::string& option, const std::string& value) noexcept
    {
        auto it = std::find_if(m_Options.begin(), m_Options.end(), [&option](const Option& opt) { return std::any_of(opt.identifier.begin(), opt.identifier.end(), [&option](const std::string& id) { return string::starts_with(option, id); }); });
        if (it == m_Options.end())
            return;

        it->value = value;
    }

    std::tuple<bool, bool, bool> is_option(std::vector<Option>& options, std::string_view option)
    {
        const auto& style = m_Conf.value_style;

        for (auto& opt : options) {
            for (auto& id : opt.identifier) {
                if (!string::starts_with(option, id))
                    continue;

                if (opt.flag) {
                    opt.active = true;
                    return std::make_tuple(true, true, false);
                }

                if (id.length() == option.length()) {
                    if (style == ValueStyle::EqualSign)
                        continue;

                    opt.active = true;

                    return std::make_tuple(true, false, false);
                } else {
                    if (style == ValueStyle::Space)
                        continue;

                    if (option.at(id.length()) != '=')
                        continue;

                    if (option.length() - id.length() < 2)
                        continue;

                    opt.active = true;

                    return std::make_tuple(true, false, true);
                }
            }
        }

        return std::make_tuple(false, false, false);
    }

    std::optional<Command> find_command_by_id(std::string_view identifier) const noexcept
    {
        for (auto& cmd : m_Commands) {
            for (auto& id : cmd.m_Identifier) {
                if (identifier == id)
                    return cmd;
            }
        }

        return std::nullopt;
    }

    std::string get_similar(std::string_view identifier) const noexcept
    {
        std::size_t length = std::numeric_limits<std::size_t>::max();

        std::string current_best_match { "" };

        for (auto& cmd : m_Commands) {
            for (auto& id : cmd.m_Identifier) {
                auto distance = string::calc_levenshtein_distance(identifier, id);

                if (distance < length) {
                    length = distance;
                    current_best_match = id;
                }
            }
        }
        return current_best_match;
    }
};
}
