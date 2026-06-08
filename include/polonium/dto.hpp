#pragma once

#include <algorithm>

#include "polonium/json_parser.hpp"

namespace polonium::dto {
using json_actions::json;

template <typename T>
concept is_trivially_aggregate =
    std::is_aggregate_v<T> and std::is_default_constructible_v<T>;

/**
 * @brief Serialize aggregate to a json structured string.
 *
 * @param object: default constructible aggregate.
 * @return std::string: json structured string.
 */
template <is_trivially_aggregate T>
[[nodiscard]] auto toJsonString(T&& object) -> std::string {
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
[[nodiscard]] auto toJson(T&& object) -> json {
    return json_actions::stringToJson(
        std::move(toJsonString(std::move(object))));
}

/**
 * @brief Convert Json string to an aggregate object.
 *
 * @param parsed_json: unparsed json to convert.
 * @return std::optional<T>: converted aggregate object.
 */
template <is_trivially_aggregate T>
[[nodiscard]] auto toAggregate(std::string&& json_string) -> std::optional<T> {
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
[[nodiscard]] auto toAggregate(json&& parsed_json) -> std::optional<T> {
    std::string json_string =
        json_actions::jsonToString(std::move(parsed_json));
    return toAggregate<T>(json_string);
}

}  // namespace polonium::dto
