
/* This header provides third-party json parser functionallity. */
#pragma once

#include <nlohmann/json.hpp>


using json = nlohmann::json;

namespace json_actions {
    inline json parseStringJson(std::string_view str) {
        if(!json::accept(str))
            /* Return an empty json if body can't be parsed. */
            return json{};
        
        return json::parse(str);
    }    
}
