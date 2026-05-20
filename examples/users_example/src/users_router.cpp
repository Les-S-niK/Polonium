
#include "users_router.hpp"

users_router::UsersRouter::UsersRouter() : polonium::PoloniumRouter("/users") {
    get("/{uint id}/{str name}/", getUserData);
    get("/{str name}/", greetUser);
}

auto users_router::UsersRouter::getUserData(polonium::HttpRequest&& request)
    -> std::shared_ptr<JsonResponse> {
    const uint64_t user_id =
        std::get<uint64_t>(request.path_params.at("id").value);
    std::string user_name =
        std::get<std::string>(std::move(request.path_params.at("name").value));

    polonium::JsonResponse response{};
    response.appendContent("id", std::to_string(user_id));
    response.appendContent("name", std::move(user_name));
    return std::make_shared<polonium::JsonResponse>(response);
}

auto users_router::UsersRouter::greetUser(polonium::HttpRequest&& request)
    -> std::shared_ptr<JsonResponse> {
    std::string name =
        std::get<std::string>(request.path_params.at("name").value);

    json parsed_body = json_actions::parseStringJson(std::move(request.body));
    if (parsed_body.empty() or not parsed_body.contains("message")) {
        JsonResponse response{unprocessable_entity_422};
        response.appendContent("error", "unprocessable_entity_422");
        return std::make_shared<JsonResponse>(response);
    }
    std::string message = std::move(parsed_body.at("message"));

    JsonResponse response{};
    response.appendContent("Greetings",
                           std::format("Hello, {}", std::move(name)));
    response.appendContent("Message", std::move(message));
    return std::make_shared<JsonResponse>(response);
}
