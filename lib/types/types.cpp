module;

#include <nlohmann/json.hpp>
#include <unordered_map>

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

template <typename T>
concept store = requires(T t) {
  t.put(std::string_view{}, std::string_view{});
  t.get(std::string_view{});
  t.erase(std::string_view{});
};

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
}; // namespace tome
