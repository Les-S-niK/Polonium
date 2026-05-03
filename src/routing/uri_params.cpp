
#include "polonium/routing/uri_params.hpp"

UriParamTemplate::UriParamTemplate(std::string type, std::string name)
    : type(std::move(type)), name(std::move(name)) {}

UriParamValue::UriParamValue(std::string_view type, std::string value)
    : type(type), value(std::move(value)) {}
UriParamValue::UriParamValue(std::string_view type, int value)
    : type(type), value(std::to_string(value)) {}
