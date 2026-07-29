module;

#include <cstdint>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

export module types;

export namespace tome
{
using json = nlohmann::json;
using document = std::unordered_map<std::string, json>;

struct query
{
  std::optional<document> filter;
  std::optional<json> projection;
};

struct statement
{
  std::string collection;
  query query;
  json filter;
  json doc;
  json data;
};

enum class expr_op : std::int8_t { EQ, NE, GT, LT, AND, OR, GTE, LTE };

template <typename T>
concept store = requires(T t) {
  t.put(std::string_view{}, std::vector<std::uint8_t>{});
  t.get(std::string_view{});
  t.erase(std::string_view{});
  t.next(std::string{});
  t.reset(std::string{});
};

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
}; // namespace tome
