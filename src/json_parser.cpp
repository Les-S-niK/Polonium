
#include "polonium/json_parser.hpp"

#include <glaze/glaze.hpp>
#include <glaze/json/read.hpp>

auto json_actions::parseStringJson(std::string_view str) -> json {
    json parsed_json{};
    auto error_core = glz::read_json(parsed_json, str);
    if (error_core) { /* Return an empty json if body can't be parsed. */
        return {};
    }

    return parsed_json;
}
auto json_actions::dumpJsonString(const json& parsed_json) -> std::string {
    std::string dumped_json{};
    auto error_core = glz::write_json(parsed_json, dumped_json);
    if (error_core) {
        return {};
    }
    return dumped_json;
}
auto json_actions::dumpJsonString(json&& parsed_json) -> std::string {
    std::string dumped_json{};
    auto error_core = glz::write_json(std::move(parsed_json), dumped_json);
    if (error_core) {
        return {};
    }
    return dumped_json;
}
