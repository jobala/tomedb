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
}; // namespace tome
