
#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/*
Since I won't change ctre to another way to use regex in compile-time,
I don't want to make a header with interfaces for this third-party lib.
*/
#include <ctre.hpp>

#include "polonium/routing/uri_params.hpp"

namespace polonium {
using parsed_templates = std::unordered_map<unsigned, UriParamTemplate>;

class UriParser {
   public:
    UriParser(std::string_view uri, std::string_view uri_template);
    UriParser(const UriParser&) noexcept = default;
    UriParser(const UriParser&&) noexcept = delete;
    auto operator=(const UriParser&) noexcept -> UriParser& = default;
    auto operator=(const UriParser&&) noexcept -> UriParser& = delete;
    ~UriParser() = default;

    /**
     * Get all URI params ( type and value ) by parsed templates parameters.
     * @param params_template: parsed templates parameters.
     *  ( std::unordered_map<section number, ( type, name )> )
     *
     * @return std::unordered_map<std::string, UriParamValue>:
     *  { name: ( type, value ) }
     */
    [[nodiscard]] auto getUriParamsByTemplate(parsed_templates params_template)
        const -> std::unordered_map<std::string, UriParamValue>;
    /**
     * Split URI by separator. ( default = '/')
     * @param uri: URI to split.
     * @param separator: char that uses to separate URL. Default = '/'.
     *
     * @return std::vector<std::string>: splitted by separator URI.
     */
    [[nodiscard]] static auto splitUri(std::string_view uri,
                                       char separator = '/')
        -> std::vector<std::string>;

   private:
    template <typename T>
    static auto appendParsedNumberValue(
        std::unordered_map<std::string, UriParamValue>& parsed_templates,
        const std::optional<T>& parsed_value,
        const UriParamTemplate& param_template) -> void {
        parsed_templates.emplace(
            param_template.name,
            UriParamValue(param_template.type, parsed_value.value()));
    }
    std::string_view uri_;
    std::string_view uri_template_;
};

class UriTemplateParser {
   public:
    enum class ParserErrors : uint8_t {
        IncorrectUriTemplate,
        IncorrectTemplateParamType
    };
    explicit UriTemplateParser(std::string_view uri_template);
    UriTemplateParser(const UriTemplateParser&) noexcept = default;
    UriTemplateParser(const UriTemplateParser&&) noexcept = delete;
    auto operator=(const UriTemplateParser&) noexcept
        -> UriTemplateParser& = default;
    auto operator=(const UriTemplateParser&&) noexcept
        -> UriTemplateParser& = delete;
    ~UriTemplateParser() noexcept = default;

    [[nodiscard]] auto getUriParamsTemplate() const
        -> std::expected<parsed_templates, ParserErrors>;

   private:
    static constexpr auto pattern =
        ctll::fixed_string{R"((?<type>\w+)\s+(?<name>\w+))"};

    /**
     * Validate URI with templates syntax corretness.
     * Check brackets amount, slashes, spaces, etc.
     *
     * @return bool: True if the URI is correct. Else false.
     */
    [[nodiscard]] auto validateUriSyntax() const noexcept -> bool;

    /**
     * Validate URI template corretness by regex.
     * Check if necessary fields ( name and type ) exist in the URI template.
     *
     * @param uri_param_value: URI template param to validate.
     * @return std::optional<std::pair<std::string, std::string>>:
     *  Pair of type and name if the template is correct. Else std::nullopt.
     */
    [[nodiscard]] static auto parseUriTemplate(
        std::string_view uri_param_template)
        -> std::optional<std::pair<std::string, std::string>>;

    std::string_view uri_template_;
};
}  // namespace polonium
