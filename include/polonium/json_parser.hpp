
/* This header provides third-party json parser functionallity. */
#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace json_actions {
inline auto parseStringJson(std::string_view str) -> json {
    if (!json::accept(
            str)) { /* Return an empty json if body can't be parsed. */
        return json{};
    }

    return json::parse(str);
}
}  // namespace json_actions
