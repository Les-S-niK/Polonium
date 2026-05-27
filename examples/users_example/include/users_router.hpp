
#include <memory>
#include <polonium/app/polonium.hpp>
#include <polonium/http/http_actions.hpp>

namespace users_router {
using polonium::JsonResponse;
using polonium::status_codes::client_errors_4xx::unprocessable_entity_422;

struct SignUpUserModel {
    std::string password;
    uint age;
};

struct SignUpUserResponseModel {
    std::string greetings;
    uint age;
};

class UsersRouter : public polonium::PoloniumRouter {
   public:
    UsersRouter();

    /**
     * @brief Handler gets user data from params and body and
     * make "sign-up" ( validate user data and return it as one json. )
     *
     * This is a test example of simple new DTO functional provided by glaze.
     *
     * Expects "name" in path params.
     * Expects "password" and "age" fields in body ( json ).
     *
     * @param request
     */
    static auto signUpUser(polonium::HttpRequest&& request)
        -> std::shared_ptr<JsonResponse>;

    /**
     * @brief Handler gets user data from path params and
     * return it in json response body.
     * Expects "id" in path params.
     * Expects "name" in path params.
     *
     * @param request
     */
    static auto getUserData(polonium::HttpRequest&& request)
        -> std::shared_ptr<JsonResponse>;

    /**
     * @brief Handler greets user using provided data.
     * Expects "name" in path params.
     * Expects "message" field in the JSON body.
     *
     * @param request
     */
    static auto greetUser(polonium::HttpRequest&& request)
        -> std::shared_ptr<JsonResponse>;
};
}  // namespace users_router
