
/* This header provides interface to access third-party json parsers
 * functionality. */
#pragma once

#include <expected>
#include <glaze/core/context.hpp>
#include <glaze/json/generic.hpp>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace polonium::json_actions {
using json = std::unordered_map<std::string, glz::generic>;

template <typename T>
concept is_trivially_aggregate =
    std::is_aggregate_v<T> and std::is_default_constructible_v<T>;

/**
 * @brief Parse string value to json type.
 *
 * @param str: string to be parsed.
 */
[[nodiscard]] auto parseStringJson(std::string_view str) -> json;

template <is_trivially_aggregate T>
[[nodiscard]] auto convertAggregateJsonString(T&& object) -> std::string {
    std::expected<std::string, glz::error_ctx> parsed_str =
        glz::write_json<T>(std::move(object));
    if (not parsed_str.has_value()) { /* Return an empty string if aggregate
                                         can't be dumped to string. */
        return {};
    }
    return parsed_str.value();
}

/**
 * @brief Convert T aggregate to json object.
 *
 * @param T&& object
 * @return converted object in json format.
 */
template <is_trivially_aggregate T>
[[nodiscard]] auto convertAggregateJson(T&& object) -> json {
    return parseStringJson(
        std::move(convertAggregateJsonString(std::move(object))));
}

/**
 * @brief Dump parsed Json to a string.
 *
 * @param parsed_json: parsed json to dump.
 */
[[nodiscard]] auto dumpJsonString(const json& parsed_json) -> std::string;
[[nodiscard]] auto dumpJsonString(json&& parsed_json) -> std::string;

/**
 * @brief Convert Json string to an aggregate object.
 *
 * @param parsed_json: unparsed json to convert.
 * @return std::optional<T>: converted aggregate object.
 */
template <is_trivially_aggregate T>
[[nodiscard]] auto convertJsonStringAggregate(std::string&& json_string)
    -> std::optional<T> {
    std::expected<T, glz::error_ctx> dumped_object =
        glz::read_json<T>(std::move(json_string));
    if (not dumped_object.has_value()) { /* Return the std::nullopt if json can
                                            not be converted. */
        return std::nullopt;
    }
    return dumped_object.value();
}

/**
 * @brief Convert parsed Json to an aggregate object.
 *
 * @param parsed_json: parsed json to convert.
 * @return std::optional<T>: converted aggregate object.
 */
template <is_trivially_aggregate T>
[[nodiscard]] auto convertJsonAggregate(json&& parsed_json)
    -> std::optional<T> {
    std::string json_string = dumpJsonString(std::move(parsed_json));
    return convertJsonStringAggregate<T>(json_string);
}

}  // namespace polonium::json_actions
