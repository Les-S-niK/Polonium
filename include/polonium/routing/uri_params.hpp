
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace polonium {
namespace uri_param_types {
inline constexpr std::string_view str_type = "str";
inline constexpr std::string_view int_type = "int";
inline constexpr std::string_view uint_type = "uint";
inline constexpr std::string_view double_type = "double";

inline constexpr std::array<std::string_view, 4> allowed_types = {
    str_type, int_type, uint_type, double_type};
}  // namespace uri_param_types

using uri_path_params = std::vector<std::pair<std::string, std::string>>;

struct UriParamTemplate {
    UriParamTemplate(std::string type, std::string name);

    std::string type;
    std::string name;
};

struct UriParamValue {
    UriParamValue(std::string_view type, std::string value);
    UriParamValue(std::string_view type, int64_t value);
    UriParamValue(std::string_view type, uint64_t value);
    UriParamValue(std::string_view type, double value);

    std::string_view type;
    std::variant<std::string, int64_t, uint64_t, double> value;
};
}  // namespace polonium
