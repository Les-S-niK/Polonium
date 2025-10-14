#pragma once
#include <string>

#include "../third_party/llhttp/llhttp.h"


class HttpRequestsParser
{
    public:
        HttpRequestsParser(const std::string& request);

    private:
        llhttp_t parser;
        llhttp_settings_t settings;
        const std::string request;
        
        void setCallbacks();
        void parse();

        // Callback llhttp methods.
        static int handler_on_message_begin(llhttp_t* parser);
};
