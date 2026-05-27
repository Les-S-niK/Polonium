
#include "users_router.hpp"

#include <ctre/functions.hpp>
#include <glaze/json/generic.hpp>
#include <memory>
#include <polonium/json_parser.hpp>
#include <string>
#include <unordered_map>
#include <utility>

// TODO: lessnik - Implement JsonResponse method that takes body
// from converted ODT.

namespace {

auto getHttp422Error() -> std::shared_ptr<polonium::JsonResponse> {
    static auto http422error = std::make_shared<polonium::JsonResponse>(
        polonium::json_actions::json{{"Error", "Unprocessable Entity 422"}},
        polonium::status_codes::client_errors_4xx::unprocessable_entity_422);

    return http422error;
}
}  // namespace

users_router::UsersRouter::UsersRouter() : polonium::PoloniumRouter("/users") {
    get("/{uint id}/{str name}/", getUserData);
    get("/{str name}/", greetUser);
    post("/{str name}/", signUpUser);
}

auto users_router::UsersRouter::signUpUser(polonium::HttpRequest&& request)
    -> std::shared_ptr<JsonResponse> {
    using polonium::json_actions::convertJsonStringAggregate;

    std::string username =
        std::get<std::string>(request.path_params.at("name").value);
    std::optional<SignUpUserModel> sign_up_model =
        convertJsonStringAggregate<SignUpUserModel>(std::move(request.body));
    if (not sign_up_model.has_value()) {
        return getHttp422Error();
    }
    JsonResponse response{};
    SignUpUserResponseModel response_model{
        .greetings = std::format("Hello, {}", std::move(username)),
        .age = sign_up_model->age};
    response.appendContent(std::move(response_model));
    return std::make_shared<JsonResponse>(response);
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

    polonium::json_actions::json parsed_body =
        polonium::json_actions::parseStringJson(std::move(request.body));
    if (parsed_body.empty() or not parsed_body.contains("message")) {
        return getHttp422Error();
    }
    std::string message =
        std::move(parsed_body.at("message").get<std::string>());

    JsonResponse response{};
    response.appendContent("Greetings",
                           std::format("Hello, {}", std::move(name)));
    response.appendContent("Message", std::move(message));
    return std::make_shared<JsonResponse>(response);
}
