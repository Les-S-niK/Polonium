
#include "http/http.hpp"
#include "routing/uri_parser.hpp"


std::unordered_map<std::string, std::string>
UriParser::getUriParamsByTemplate(std::unordered_map<unsigned, UriParamTemplate> params_template) const {
    if(params_template.empty()) return {};
    
    std::vector<std::string> splitted_uri = splitUri(uri_);
    std::vector<std::string> splitted_uri_template = splitUri(uri_template_);
    if(splitted_uri.size() != splitted_uri_template.size()) return {};

    std::unordered_map<std::string, std::string> parsed_values;
    
    for(size_t section = 0; section < splitted_uri.size(); section++) {
        auto found_template_it = params_template.find(section);
        
        if(found_template_it != params_template.end()) {
            const UriParamTemplate& param_template = (found_template_it->second);

            if(param_template.type == uri_param_types::int_type) {
                if(!tryConvertToInt(splitted_uri[section])) return {};
                parsed_values.emplace(param_template.name, splitted_uri[section]);
            }
            else if(param_template.type == uri_param_types::str_type)
                parsed_values.emplace(param_template.name, splitted_uri[section]);
        } 
        else if(splitted_uri[section] != splitted_uri_template[section]) return {};
    }
    return parsed_values;
}


std::unordered_map<unsigned, UriParamTemplate> UriTemplateParser::getUriParamsTemplate() const {
    // {unsigned section: UtiParamTemplate(string type, string name)}
    std::unordered_map<unsigned, UriParamTemplate> path_params;    
    std::vector<std::string> splitted_uri = splitUri(uri_template_);
    // TODO: Make better exception throwing.
    if(!validateUriTemplateFirst()) throw std::runtime_error("Incorrect URI template!");

    for(size_t section = 0; section < splitted_uri.size(); section++) {
        std::string_view current_str = splitted_uri.at(section);

        if(current_str.front() == '{') {
            auto optional_type_and_name = validateUriTemplateSecond(current_str); 
            // TODO: Make better exception throwing.
            if(!optional_type_and_name) throw std::runtime_error("Incorrect URI template!");
            path_params.emplace(section, UriParamTemplate(
                optional_type_and_name->first,
                optional_type_and_name->second
            ));
        }
    }
    return path_params;
}

bool UriTemplateParser::validateUriTemplateFirst() const noexcept {
    if(uri_template_.empty()) return false;
    if(uri_template_.front() != '/') return false;

    size_t brace_amount = 0;
    bool inside_brackets = false;
    
    for(size_t index = 0; index < uri_template_.size(); index++) {
        const char& char_ = uri_template_.at(index);
        switch (char_) {
            case '{':
                if(index > 0 && uri_template_.at(index - 1) != '/') return false;
                if(inside_brackets) return false;
                inside_brackets = true;
                brace_amount++; 
                break;
            case '}':
                if(index + 1 < uri_template_.size() && uri_template_.at(index + 1) != '/') return false;
                if(!inside_brackets) return false;
                inside_brackets = false;
                brace_amount--;
                break;
            case ' ':
                if(!inside_brackets) return false;
                break;
            case '/':
                if(inside_brackets) return false;
                break;
        }
    }
    if(brace_amount != 0) return false;
    return true;
}

std::optional<std::pair<std::string, std::string>> UriTemplateParser::validateUriTemplateSecond(std::string_view uri_param_template) const {
    using namespace uri_param_types;
    const auto match = ctre::search<pattern>(uri_param_template);
    const std::optional<std::string> type = match.get<1>().to_optional_string();
    const std::optional<std::string> name = match.get<2>().to_optional_string();

    auto found_type = std::find(allowed_types.cbegin(), allowed_types.cend(), type);
    // TODO: Make better exception throwing.
    if(found_type == std::end(allowed_types)) throw std::runtime_error("Incorrect template param type!");
    if(!type || !name) return std::nullopt;

    return std::make_pair(type.value(), name.value());
}
