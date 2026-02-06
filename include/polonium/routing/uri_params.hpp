
#pragma once

#include <string_view>
#include <array>
#include <string>
#include <vector>


namespace uri_param_types {
    inline constexpr std::string_view str_type = "str";
    inline constexpr std::string_view int_type = "int";
    inline constexpr std::array<std::string_view, 2> allowed_types = {int_type, str_type};
}

using uri_path_params = std::vector<std::pair<std::string, std::string>>;


struct UriParamTemplate
{
    UriParamTemplate(std::string type, std::string name)
     :
      type(type), name(name) 
        {} 
    std::string type;
    std::string name;
};


struct UriParamValue
{
    UriParamValue(std::string_view type, std::string value) : type(type), value(value) {}
    UriParamValue(std::string_view type, int value) : type(type), value(std::to_string(value)) {}
    
    std::string_view type;
    std::string value;
};
