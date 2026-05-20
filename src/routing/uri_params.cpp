
#include "polonium/routing/uri_params.hpp"

#include <cstdint>

polonium::UriParamTemplate::UriParamTemplate(std::string type, std::string name)
    : type(std::move(type)), name(std::move(name)) {}

polonium::UriParamValue::UriParamValue(std::string_view type, std::string value)
    : type(type), value(std::move(value)) {}
polonium::UriParamValue::UriParamValue(std::string_view type, int64_t value)
    : type(type), value(value) {}
polonium::UriParamValue::UriParamValue(std::string_view type, uint64_t value)
    : type(type), value(value) {}
polonium::UriParamValue::UriParamValue(std::string_view type, double value)
    : type(type), value(value) {}
