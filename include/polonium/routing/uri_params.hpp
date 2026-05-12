
#pragma once

#include <array>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace polonium {
namespace uri_param_types {
inline constexpr std::string_view str_type = "str";
inline constexpr std::string_view int_type = "int";
inline constexpr std::array<std::string_view, 2> allowed_types = {int_type,
                                                                  str_type};
}  // namespace uri_param_types

using uri_path_params = std::vector<std::pair<std::string, std::string>>;

struct UriParamTemplate {
    UriParamTemplate(std::string type, std::string name);

    std::string type;
    std::string name;
};

struct UriParamValue {
    UriParamValue(std::string_view type, std::string value);
    UriParamValue(std::string_view type, int value);

    std::string_view type;
    std::variant<std::string, int> value;
};
}  // namespace polonium
