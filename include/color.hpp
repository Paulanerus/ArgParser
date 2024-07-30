#pragma once

#include <type_traits>
#include <ostream>
#include <format>
#include <string>

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