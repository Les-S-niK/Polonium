
#include "polonium/routing/params_parser.hpp"

#include <cstdint>
#include <optional>
#include <string>

[[nodiscard]] auto polonium::params_parser::convertStrToInt64(
    const std::string& value) -> std::optional<int64_t> {
    auto stol_wrapper = [](const std::string& value, size_t* pos) -> int64_t {
        return std::stol(value, pos);
    };
    return convertStrToNumber<int64_t>(value, stol_wrapper);
}

[[nodiscard]] auto polonium::params_parser::convertStrToUint64(
    const std::string& value) -> std::optional<uint64_t> {
    if (value.starts_with('-')) {
        return std::nullopt;
    }
    auto stoul_wrapper = [](const std::string& value, size_t* pos) -> uint64_t {
        return std::stoul(value, pos);
    };
    return convertStrToNumber<uint64_t>(value, stoul_wrapper);
}

[[nodiscard]] auto polonium::params_parser::convertStrToDouble(
    const std::string& value) -> std::optional<double> {
    auto stod_wrapper = [](const std::string& value, size_t* pos) -> double {
        return std::stod(value, pos);
    };
    return convertStrToNumber<double>(value, stod_wrapper);
}
