
#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

/*
Since I won't change ctre to another way to use regex in compile-time,
I don't want to make a header with interfaces for this third-party lib.
*/ 
#include "ctre.hpp"
#include "routing/uri_params.hpp"


using parsed_templates = std::unordered_map<unsigned, UriParamTemplate>;


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
            size_t pos{}; 
            
            try { std::stoi(value, &pos); } 
            catch(const std::invalid_argument&) { return false; }
            catch(const std::out_of_range&) { return false; }
            
            if(value.size() != pos) return false;
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
