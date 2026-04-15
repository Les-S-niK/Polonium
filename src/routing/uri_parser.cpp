
#include "polonium/routing/uri_parser.hpp"

#include <algorithm>
#include <sstream>

#include "polonium/routing/uri_params.hpp"

auto UriParser::getUriParamsByTemplate(parsed_templates params_template) const
    -> std::unordered_map<std::string, UriParamValue> {
    if (params_template.empty()) {
        return {};
    }

    // Split both uri and template uri by default separator.
    std::vector<std::string> splitted_uri = splitUri(uri_);
    std::vector<std::string> splitted_uri_template = splitUri(uri_template_);
    if (splitted_uri.size() != splitted_uri_template.size()) {
        return {};
    }

    std::unordered_map<std::string, UriParamValue> parsed_values;
    for (size_t section = 0; section < splitted_uri.size(); section++) {
        auto found_template_it = params_template.find(section);

        if (found_template_it != params_template.end()) {
            const UriParamTemplate& param_template =
                (found_template_it->second);

            // Check if param type is valid.
            if (param_template.type == uri_param_types::int_type) {
                if (!tryConvertToInt(splitted_uri[section])) {
                    return {};
                }
                parsed_values.emplace(param_template.name,
                                      UriParamValue(uri_param_types::int_type,
                                                    splitted_uri[section]));
            } else if (param_template.type == uri_param_types::str_type) {
                parsed_values.emplace(param_template.name,
                                      UriParamValue(uri_param_types::str_type,
                                                    splitted_uri[section]));
            }
        } else if (splitted_uri[section] != splitted_uri_template[section]) {
            return {};
        }
    }
    return parsed_values;
}

auto UriTemplateParser::getUriParamsTemplate() const -> parsed_templates {
    // {unsigned section: UtiParamTemplate(string type, string name)}
    parsed_templates path_params;
    std::vector<std::string> splitted_uri = UriParser::splitUri(uri_template_);
    // TODO: lessnik - Make better exception throwing.
    if (!validateUriSyntax()) {
        throw std::runtime_error("Incorrect URI template!");
    }

    for (size_t section = 0; section < splitted_uri.size(); section++) {
        std::string_view current_str = splitted_uri.at(section);

        if (current_str.front() == '{') {
            auto optional_type_and_name = parseUriTemplate(current_str);
            // TODO: lessnik - Make better exception throwing.
            if (!optional_type_and_name) {
                throw std::runtime_error("Incorrect URI template!");
            }
            path_params.emplace(
                section, UriParamTemplate(optional_type_and_name->first,
                                          optional_type_and_name->second));
        }
    }
    return path_params;
}

auto UriTemplateParser::validateUriSyntax() const noexcept -> bool {
    if (uri_template_.empty()) {
        return false;
    }
    // URI in must starts with "/"
    // Example: /users/123/
    if (uri_template_.front() != '/') {
        return false;
    }
    size_t brace_amount = 0;
    bool inside_brackets = false;

    for (size_t index = 0; index < uri_template_.size(); index++) {
        const char& char_ = uri_template_.at(index);
        switch (char_) {
            case '{':
                // Check if the previous char is "/"
                // ( "{" can be only after "/" and before "}" )
                if (index > 0 && uri_template_.at(index - 1) != '/') {
                    return false;
                }
                if (inside_brackets) {
                    return false;
                }
                inside_brackets = true;
                brace_amount++;
                break;
            case '}':
                // Check if the next char is "/"
                // ( "}" can be only after "{" and before "/" )
                if (index + 1 < uri_template_.size() &&
                    uri_template_.at(index + 1) != '/') {
                    return false;
                }
                if (!inside_brackets) {
                    return false;
                }
                inside_brackets = false;
                brace_amount--;
                break;
            case ' ':
                // Space is available only inside brackets.
                if (!inside_brackets) {
                    return false;
                }
                break;
            case '/':
                // "/" is not allowed in dynamic URI templates.
                // ( Inside brackets. )
                if (inside_brackets) {
                    return false;
                }
                break;
        }
    }
    return brace_amount == 0;
}

auto UriTemplateParser::parseUriTemplate(std::string_view uri_param_template)
    -> std::optional<std::pair<std::string, std::string>> {
    using uri_param_types::allowed_types;

    // Search matches with URI param template by pattern: "type name".
    // Example: "/{str username}/"
    const auto match = ctre::search<pattern>(uri_param_template);
    const std::optional<std::string> type = match.get<1>().to_optional_string();
    const std::optional<std::string> name = match.get<2>().to_optional_string();
    if (!type || !name) {
        return std::nullopt;
    }

    const auto* found_type = std::ranges::find(allowed_types, type.value());
    // TODO: lessnik - Make better exception throwing.
    if (found_type == std::end(allowed_types)) {
        throw std::runtime_error("Incorrect template param type!");
    }
    return std::make_pair(type.value(), name.value());
}

auto UriParser::splitUri(std::string_view uri, char separator)
    -> std::vector<std::string> {
    std::vector<std::string> splitted_uri;
    std::string token;
    std::string uri_str(uri);
    std::istringstream token_stream(uri_str);

    constexpr auto default_splits_amount = 5;
    splitted_uri.reserve(default_splits_amount);

    while (std::getline(token_stream, token, separator)) {
        if (!token.empty()) {
            splitted_uri.push_back(token);
        }
    }
    return splitted_uri;
}
