
#pragma once

#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

/*
Since I won't change ctre to another way to use regex in compile-time,
I don't want to make a header with interfaces for this third-party lib.
*/
#include <ctre.hpp>

#include "polonium/routing/uri_params.hpp"

using parsed_templates = std::unordered_map<unsigned, UriParamTemplate>;

inline auto splitUri(std::string_view uri, char separator = '/')
    -> std::vector<std::string>;

class UriParser {
   public:
    UriParser(std::string_view uri, std::string_view uri_template)
        : uri_(uri), uri_template_(uri_template) {}
    UriParser(const UriParser&) noexcept = default;
    UriParser(const UriParser&&) noexcept = delete;
    auto operator=(const UriParser&) noexcept -> UriParser& = default;
    auto operator=(const UriParser&&) noexcept -> UriParser& = delete;

    [[nodiscard]] auto getUriParamsByTemplate(parsed_templates params_template)
        const -> std::unordered_map<std::string, UriParamValue>;

   private:
    std::string_view uri_;
    std::string_view uri_template_;

    [[nodiscard]]
    static auto tryConvertToInt(const std::string& value) -> bool {
        size_t pos{};

        try {
            std::stoi(value, &pos);
        } catch (const std::invalid_argument&) {
            return false;
        } catch (const std::out_of_range&) {
            return false;
        }

        return value.size() == pos;
    }
};

class UriTemplateParser {
   public:
    UriTemplateParser(std::string_view uri_template)
        : uri_template_(uri_template) {}
    UriTemplateParser(const UriTemplateParser&) noexcept = default;
    UriTemplateParser(const UriTemplateParser&&) noexcept = delete;
    auto operator=(const UriTemplateParser&) noexcept
        -> UriTemplateParser& = default;
    auto operator=(const UriTemplateParser&&) noexcept
        -> UriTemplateParser& = delete;

    [[nodiscard]] auto getUriParamsTemplate() const -> parsed_templates;

   private:
    static constexpr auto pattern =
        ctll::fixed_string{R"((?<type>\w+)\s+(?<name>\w+))"};
    std::string_view uri_template_;

    [[nodiscard]] auto validateUriTemplateFirst() const noexcept -> bool;
    [[nodiscard]] static auto validateUriTemplateSecond(
        std::string_view uri_param_template)
        -> std::optional<std::pair<std::string, std::string>>;
};
