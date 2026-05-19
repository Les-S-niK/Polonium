#include <concepts>
#include <cstdint>
#include <optional>
#include <string>

namespace polonium::params_parser {
template <typename T, typename Func>
concept is_valid_params_parser =
    requires(const std::string& value, size_t pos, const Func& function) {
        { function(value, &pos) } -> std::same_as<T>;
    };

template <typename T, typename Func>
    requires is_valid_params_parser<T, Func>
[[nodiscard]] auto convertStrToNumber(const std::string& value,
                                      const Func& converter)
    -> std::optional<T> {
    try {
        size_t pos{};
        const T converted = converter(value, &pos);
        return value.size() == pos ? std::optional<T>(converted) : std::nullopt;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

[[nodiscard]] auto convertStrToInt64(const std::string& value)
    -> std::optional<int64_t>;
[[nodiscard]] auto convertStrToUint64(const std::string& value)
    -> std::optional<uint64_t>;
[[nodiscard]] auto convertStrToDouble(const std::string& value)
    -> std::optional<double>;
}  // namespace polonium::params_parser
