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
}

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

        // Doesn't look that good.. ~Paul
        if constexpr (std::is_convertible_v<std::string, T>)
            return static_cast<T>(it->value);
        else if constexpr (std::is_convertible_v<std::wstring, T>)
            return static_cast<T>(it->value);
        else if constexpr (std::is_same_v<T, std::int8_t>)
            return static_cast<int8_t>(it->value[0]);
        else if constexpr (std::is_same_v<T, std::uint8_t>)
            return static_cast<uint8_t>(it->value[0]);
        else if constexpr (std::is_same_v<T, std::int16_t>)
            return internal::try_catch([&it] { return static_cast<std::int16_t>(std::stoi(it->value)); });
        else if constexpr (std::is_same_v<T, std::uint16_t>)
            return internal::try_catch([&it] { return static_cast<std::uint16_t>(std::stoul(it->value)); });
        else if constexpr (std::is_same_v<T, std::int32_t>)
            return internal::try_catch([&it] { return std::stoi(it->value); });
        else if constexpr (std::is_same_v<T, std::uint32_t>)
            return internal::try_catch([&it] { return static_cast<std::uint32_t>(std::stoul(it->value)); });
        else if constexpr (std::is_same_v<T, std::int64_t>)
            return internal::try_catch([&it] { return std::stol(it->value); });
        else if constexpr (std::is_same_v<T, std::uint64_t>)
            return internal::try_catch([&it] { return std::stoul(it->value); });
        else if constexpr (std::is_same_v<T, float>)
            return internal::try_catch([&it] { return std::stof(it->value); });
        else if constexpr (std::is_same_v<T, double>)
            return internal::try_catch([&it] { return std::stod(it->value); });
        else if constexpr (std::is_same_v<T, long double>)
            return internal::try_catch([&it] { return std::stold(it->value); });
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

        update_length(cmd.length());

        return cmd;
    }

    ArgParser& option(Option&& option)
    {
        update_length(option.length());

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

        std::for_each(m_Args.begin(), m_Args.end(), [](std::string& arg) { std::transform(arg.begin(), arg.end(), arg.begin(), [](auto c) { return std::tolower(c); }); });

        std::size_t command_idx {};
        std::unordered_set<std::size_t> indieces;
        for (std::size_t i {}; i < m_Args.size(); i++) {
            if (is_command(m_Args[i])) {
                indieces.insert(i);
                command_idx = i;
                break;
            }

            auto [has, flag, contains_val] = is_option(m_Options, m_Args[i]);

            indieces.insert(i);

            if (!has) {
                command_idx = i;
                break;
            }

            if (flag)
                continue;

            if (contains_val) {
                auto val = m_Args[i].substr(m_Args[i].find_first_of('=') + 1);

                update_value(m_Args[i], val);
            } else {
                if (i + 1 >= m_Args.size())
                    continue;

                auto [n_has, _, __] = is_option(m_Options, m_Args[i + 1]);

                if (!n_has)
                    update_value(m_Args[i], m_Args[i + 1]);

                indieces.insert(i + 1);
                i++;
            }
        }

        auto command_opt = command_by_id(m_Args[command_idx]);

        if (!command_opt.has_value()) {
            std::cout << "Unknown command '" << color::light_red(m_Args[command_idx]) << "'\n";

            auto similar = get_similar(m_Args[command_idx]);

            if (!similar.empty())
                std::cout << "Did you mean: '" << color::green(similar) << "'?" << std::endl;

            return;
        }

        auto command = command_opt.value();

        for (std::size_t i = command_idx + 1; i < m_Args.size(); i++) {
            auto [c_has, c_flag, c_contains_val] = is_option(command.m_Options, m_Args[i]);

            if (!c_has)
                continue;

            if (c_flag) {
                indieces.insert(i);
                continue;
            }

            if (c_contains_val) {
                auto val = m_Args[i].substr(m_Args[i].find_first_of('=') + 1);

                command[m_Args[i]] = val;

                indieces.insert(i);
            } else {
                if (i + 1 >= m_Args.size()) {
                    indieces.insert(i);
                    continue;
                }

                auto [n_has, _, __] = is_option(command.m_Options, m_Args[i + 1]);

                if (!n_has)
                    command[m_Args[i]] = m_Args[i + 1];

                indieces.insert({ i, i + (n_has ? 0 : 1) });
                i++;
            }
        }

        auto it = std::remove_if(m_Args.begin(), m_Args.end(), [&](const std::string& arg) { return indieces.find(&arg - &m_Args[0]) != indieces.end(); });

        m_Args.erase(it);

        command.execute(*this);
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

        auto command_result = command_by_id(identifier);

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

        if constexpr (std::is_convertible_v<std::string, T>)
            return static_cast<T>(it->value);
        else if constexpr (std::is_convertible_v<std::wstring, T>)
            return static_cast<T>(it->value);
        else if constexpr (std::is_same_v<T, std::int8_t>)
            return static_cast<std::int8_t>(it->value[0]);
        else if constexpr (std::is_same_v<T, std::uint8_t>)
            return static_cast<std::uint8_t>(it->value[0]);
        else if constexpr (std::is_same_v<T, std::int16_t>)
            return internal::try_catch([&it] { return static_cast<std::int16_t>(std::stoi(it->value)); });
        else if constexpr (std::is_same_v<T, std::uint16_t>)
            return internal::try_catch([&it] { return static_cast<std::uint16_t>(std::stoul(it->value)); });
        else if constexpr (std::is_same_v<T, std::int32_t>)
            return internal::try_catch([&it] { return std::stoi(it->value); });
        else if constexpr (std::is_same_v<T, std::uint32_t>)
            return internal::try_catch([&it] { return static_cast<std::uint32_t>(std::stoul(it->value)); });
        else if constexpr (std::is_same_v<T, std::int64_t>)
            return internal::try_catch([&it] { return std::stol(it->value); });
        else if constexpr (std::is_same_v<T, std::uint64_t>)
            return internal::try_catch([&it] { return std::stoul(it->value); });
        else if constexpr (std::is_same_v<T, float>)
            return internal::try_catch([&it] { return std::stof(it->value); });
        else if constexpr (std::is_same_v<T, double>)
            return internal::try_catch([&it] { return std::stod(it->value); });
        else if constexpr (std::is_same_v<T, long double>)
            return internal::try_catch([&it] { return std::stold(it->value); });
        else
            return std::nullopt;
    }

private:
    const ParserConf m_Conf;

    std::vector<Option> m_Options;

    std::vector<Command> m_Commands;

    std::vector<std::string> m_Args;

    std::size_t m_MaxLength {};

    void update_length(std::size_t length) noexcept
    {
        if (length > m_MaxLength)
            m_MaxLength = length;
    }

    void update_value(const std::string& option, const std::string& value) noexcept
    {
        auto it = std::find_if(m_Options.begin(), m_Options.end(), [&option](const Option& opt) { return std::any_of(opt.identifier.begin(), opt.identifier.end(), [&option](const std::string& id) { return string::starts_with(option, id); }); });
        if (it == m_Options.end())
            return;

        it->value = value;
    }

    std::tuple<bool, bool, bool> is_option(std::vector<Option>& options, const std::string& option)
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

    bool is_command(std::string_view identifier) const noexcept
    {
        for (auto& cmd : m_Commands) {
            for (auto& id : cmd.m_Identifier) {
                if (identifier == id)
                    return true;
            }
        }

        return false;
    }

    std::optional<Command> command_by_id(std::string_view identifier) const noexcept
    {
        auto has_identifier = [&](const Command& cmd) { return std::any_of(cmd.m_Identifier.begin(), cmd.m_Identifier.end(), [identifier](const auto& id) { return id == identifier; }); };

        if (auto cmd = std::find_if(m_Commands.begin(), m_Commands.end(), has_identifier); cmd != m_Commands.end())
            return *cmd;

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
