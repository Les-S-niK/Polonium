
#include <memory>
#include <polonium/app/polonium.hpp>

namespace users_router {
using polonium::JsonResponse;
using polonium::status_codes::client_errors_4xx::unprocessable_entity_422;

class UsersRouter : public polonium::PoloniumRouter {
   public:
    UsersRouter();

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
