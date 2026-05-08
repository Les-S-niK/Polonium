#pragma once

#include <llhttp.h>

#include <cstddef>
#include <cstdint>
#include <string>

#include "polonium/app/polonium_logger.hpp"
#include "polonium/http/http.hpp"

enum class HttpRequestParserStatus : uint8_t {
    Complete = 0,
    NeedMore = 1,
    Error = 2
};

/**
 * @brief This parser class uses to parse raw HTTP requests
 * and serialize it into HttpRequest object.
 */
class HttpRequestParser {
   public:
    HttpRequestParser();
    HttpRequestParser(const HttpRequestParser&) = delete;
    HttpRequestParser(HttpRequestParser&&) = delete;
    auto operator=(const HttpRequestParser&) -> HttpRequestParser = delete;
    auto operator=(HttpRequestParser&&) -> HttpRequestParser = delete;
    ~HttpRequestParser();

    auto feed(std::string_view to_accumulate) -> HttpRequestParserStatus;
    auto getRequest() const -> const HttpRequest&;
    auto hasRemainingData() const -> bool;
    auto isComplete() const -> bool;
    auto isKeepAlive() const -> bool;
    auto removeParsed() -> void;
    auto reset() -> void;

   private:
    llhttp_t parser_{};
    llhttp_settings_t settings_{};
    HttpRequest request_;
    std::string raw_request_;
    bool is_complete_ = false;
    bool is_keep_alive_ = false;
    size_t parsed_bytes_ = 0;
    polonium::PoloniumLogger* logger_;
    /**
     * @brief Temporary pair represents <field: value> in the request headers.
     * The first element sets in the <handler_on_header_field>
     * The second element sets in the <handler_on_header_value>
     * The pair will be added in the <headers> in the
     * <handler_on_header_value_complete>.
     */
    std::pair<std::string, std::string> temporary_pair_;

    auto parseAccumulated() -> HttpRequestParserStatus;
    auto setCallbacks() -> void;

    static auto handlerOnMessageBegin(llhttp_t* parser) -> int;
    /**
     * @brief Handler calls when llhttp finds HTTP method:
     * [GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT]
     */
    static auto handlerOnMethod(llhttp_t* parser, const char* chunk_ptr,
                                size_t length) -> int;
    static auto handlerOnUri(llhttp_t* parser, const char* chunk_ptr,
                             size_t length) -> int;
    /**
     * @brief Handler calls when llhttp finds protocol type [HTTP for example]
     */
    static auto handlerOnProtocol(llhttp_t* parser, const char* chunk_ptr,
                                  size_t length) -> int;
    /**
     * @brief Handler calls when llhttp finds protocol version. [1.1; 2; etc...
     * for example]
     */
    static auto handlerOnProtocolVersion(llhttp_t* parser,
                                         const char* chunk_ptr, size_t length)
        -> int;
    /**
     * @brief Handler calls when llhttp finds header field [<field>: value];
     */
    static auto handlerOnHeaderField(llhttp_t* parser, const char* chunk_ptr,
                                     size_t length) -> int;
    static auto handlerOnHeaderValue(llhttp_t* parser, const char* chunk_ptr,
                                     size_t length) -> int;
    static auto handlerOnHeaderValueComplete(llhttp_t* parser) -> int;

    static auto handlerOnBody(llhttp_t* parser, const char* chunk_ptr,
                              size_t length) -> int;
    static auto handlerOnMessageComplete(llhttp_t* parser) -> int;
};
