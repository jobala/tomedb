module;

#include <string>

export module db;

export import :collection;
export import types;

export namespace tome
{

struct db
{
  auto collection(const std::string &collection_name) -> collection
  {
    struct collection coll(collection_name);
    return coll;
  };
};
} // namespace tome
