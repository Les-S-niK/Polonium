
#include <stdexcept>
#include <format>

#include "dispatcher.hpp"
#include "ctre.hpp"


void Dispatcher::registerMethod(std::string&& method, std::string&& uri, endpoint_handler&& handler) {
    logger_.trace(__func__);

    if (routes_.find(method) == routes_.end()) {
        routes_.emplace(method, std::unordered_map<std::string, endpoint_handler>());
    }
    routes_.at(method).emplace(uri, handler);
    logger_.info(std::format("Registered new method.\nMethod: {}\nUri: {}", method, uri));
}

std::optional<endpoint_handler> Dispatcher::checkRoute(const std::string& method, const std::string& uri) const {
    logger_.trace(__func__);
    logger_.debug(std::format("Check router.\nMethod: {}\nUri: {}", method, uri));
    
    /*
    TODO: Добавить следующую логику:
        Диспетчер держит не шаблон URI, а, в зависимости от наличия параметров пути,
        уже распарсенный путь. Если путь без параметров - всё уже работает.
        При наличии параметров пути, благодаря функциям, мы вычленяем из пути все параметры, 
        "перемалываем" их своим парсером в регулярки.
        Т.е. /users/{int id}/ => /users/\d+/
        При проверке придётся реализовать логику сверки URI с паттерном, причём так,
        чтобы гарантировать верные срабатывания.
        
        Реализовать функцию, возвращающую bool при сверки uri с шаблоном с регуляркой.
        При true вычленить параметры пути из URI.
        
        Вопрос открытый про валидацию типов. Вероятно, ближе к делу я примерно пойму момент, когда
        она должна будет выполняться.
    
    */ 
    // uri_path_params path_params = findAllPathParams(uri);

    try {
        endpoint_handler route = routes_.at(method).at(uri);
        logger_.debug(std::format("Endpoint was found.\nMethod: {}\nUri: {}", method, uri));
        return route;
    }
    catch(std::out_of_range& err) {
        logger_.debug(std::format("Endpoint not found.\nMethod: {}\nUri: {}", method, uri));
        return std::nullopt;
    }
}

uri_path_params Dispatcher::findAllPathParams(std::string_view uri) const {
    logger_.trace(__func__);
    static constexpr auto pattern = ctll::fixed_string{"\\w+\\s+\\w+"};
    uri_path_params path_params;

    for(const auto& match : ctre::multiline_search_all<pattern>(uri)) {
        std::optional<std::string_view> found_view = match.to_optional_view();
        if(!found_view) continue;

        if(size_t found_index = found_view->find(' '); found_index != std::string::npos) {
            logger_.debug(std::format("Found path param: {}", found_view.value()));
            path_params.push_back({
                std::string(found_view->begin() + found_index + 1, found_view->end()),
                std::string(found_view->begin(), found_view->begin() + found_index)
            });
        }
    }
    return path_params;
}
