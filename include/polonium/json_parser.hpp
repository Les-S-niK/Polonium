
/* This header provides interface to access third-party json parsers
 * functionality. */
#pragma once

#include <string_view>
#include <unordered_map>

using json = std::unordered_map<std::string, std::string>;
namespace json_actions {
/**
 * @brief Parse string value to json type.
 *
 * @param str: string to be parsed.
 */
[[nodiscard]] auto parseStringJson(std::string_view str) -> json;

/**
 * @brief Dump parsed Json to a string.
 *
 * @param parsed_json: parsed json to dump.
 */
[[nodiscard]] auto dumpJsonString(const json& parsed_json) -> std::string;
[[nodiscard]] auto dumpJsonString(json&& parsed_json) -> std::string;
}  // namespace json_actions
