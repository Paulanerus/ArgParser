#pragma once

#include <initializer_list>
#include <string_view>
#include <functional>
#include <algorithm>
#include <vector>
#include <string>
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
        return std::ranges::any_of(m_Options, [option_id](const auto &option)
                                   { return std::ranges::any_of(option.identifier, [option_id](const auto &id)
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

    bool hasIdentifier(std::string_view identifier) const noexcept
    {
        return std::ranges::any_of(m_Identifier, [identifier](const auto &id)
                                   { return id == identifier; });
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