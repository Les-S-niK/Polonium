
/* This header provides interface to access third-party json parsers
 * functionality. */
#pragma once

#include <glaze/core/context.hpp>
#include <glaze/json/generic.hpp>
#include <string_view>
#include <unordered_map>

namespace polonium::json_actions {
using json = std::unordered_map<std::string, glz::generic>;

/**
 * @brief Parse string value to json type.
 *
 * @param str: string to be parsed.
 */
[[nodiscard]] auto stringToJson(std::string_view str) -> json;

/**
 * @brief Dump parsed Json to a string.
 *
 * @param parsed_json: parsed json to dump.
 */
[[nodiscard]] auto jsonToString(const json& parsed_json) -> std::string;
[[nodiscard]] auto jsonToString(json&& parsed_json) -> std::string;

}  // namespace polonium::json_actions
