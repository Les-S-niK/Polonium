
#pragma once

#include <array>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "ctre.hpp"
#include "http/http.hpp"


using parsed_templates = std::unordered_map<unsigned, UriParamTemplate>;

namespace uri_param_types {
    inline constexpr std::string_view str_type = "str";
    inline constexpr std::string_view int_type = "int";
    inline constexpr std::array<std::string_view, 2> allowed_types = {int_type, str_type};
}


inline std::vector<std::string> splitUri(std::string_view uri, const char separator = '/') {
    std::vector<std::string> splitted_uri;
    splitted_uri.reserve(5);
    std::string token, uri_str(uri);
    std::istringstream token_stream(uri_str);
    
    while(std::getline(token_stream, token, separator)) {
        if(!token.empty()) 
            splitted_uri.push_back(token);
    }
    return splitted_uri;
}


class UriParser
{
    public:
        UriParser(std::string_view uri, std::string_view uri_template)
         :
          uri_(uri),
          uri_template_(uri_template)
        {}
        UriParser(const UriParser&) noexcept = default;
        UriParser(const UriParser&&) noexcept = delete;
        UriParser& operator=(const UriParser&) noexcept = default;
        UriParser& operator=(const UriParser&&) noexcept = delete;
        
        std::unordered_map<std::string, UriParamValue> 
        getUriParamsByTemplate(parsed_templates params_template) const;
    
    private:
        std::string_view uri_;
        std::string_view uri_template_;

        bool tryConvertToInt(const std::string& value) const {
            try { std::stoi(value); } 
            catch(const std::invalid_argument&) { return false; }
            catch(const std::out_of_range&) { return false; }
            return true;
        }
};


class UriTemplateParser
{
    public:
        UriTemplateParser(std::string_view uri_template) : uri_template_(uri_template) {}
        UriTemplateParser(const UriTemplateParser&) noexcept = default;
        UriTemplateParser(const UriTemplateParser&&) noexcept = delete;
        UriTemplateParser& operator=(const UriTemplateParser&) noexcept = default;
        UriTemplateParser& operator=(const UriTemplateParser&&) noexcept = delete;
        
        parsed_templates getUriParamsTemplate() const;

    private:
        static constexpr auto pattern = ctll::fixed_string{"(?<type>\\w+)\\s+(?<name>\\w+)"};
        std::string_view uri_template_;

        bool validateUriTemplateFirst() const noexcept;
        std::optional<std::pair<std::string, std::string>> validateUriTemplateSecond(std::string_view uri_param_template) const;
};
