module;

#include <iostream>
#include <string>

export module db;

export import :collection;
import :storage;
export import types;

export namespace tome
{
struct db
{
  db(const std::string &db_path) { std::cout << db_path; }

  auto collection(const std::string &collection_name) -> collection
  {
    struct collection coll(collection_name);
    return coll;
  };
};
} // namespace tome
