
#include "polonium/json_parser.hpp"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <unordered_map>

auto json_actions::parseStringJson(std::string_view str) -> json {
    if (!nlohmann::json::accept(
            str)) { /* Return an empty json if body can't be parsed. */
        return {};
    }

    return nlohmann::json::parse(str)
        .get<std::unordered_map<std::string, std::string>>();
}
auto json_actions::dumpJsonString(const json& parsed_json) -> std::string {
    nlohmann::json json = nlohmann::json::object();
    json.update(parsed_json);
    return json.dump();
}
auto json_actions::dumpJsonString(json&& parsed_json) -> std::string {
    nlohmann::json json = nlohmann::json::object();
    json.update(std::move(parsed_json));
    return json.dump();
}
