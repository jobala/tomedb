module;

#include <nlohmann/json.hpp>
#include <string>

export module db;

export import :collection;

export namespace tome
{
using document = std::unordered_map<std::string, nlohmann::json>;

struct db
{
  auto collection(const std::string &collection_name) -> collection
  {
    struct collection coll(collection_name);
    return coll;
  };
};
} // namespace tome
