
#pragma once

#include <cstddef>
#include <cstdint>

using socket_fd = int;

namespace socket_options {
// Max backlog size for listen function. 10 is for tests. 128 or 4096 for
// production.
constexpr inline uint8_t max_backlog_size = 10;
constexpr inline size_t max_buffer_size = 8192;
}  // namespace socket_options
