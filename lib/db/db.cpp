module;

#include <string>

export module db;

export import :collection;
export import types;
import :storage;

export namespace tome
{
struct db
{
  db(const std::string &db_path) : store_(storage(db_path)) {}

  auto collection(const std::string &collection_name) -> collection
  {
    struct collection coll(collection_name, store_);
    return coll;
  };

private:
  storage store_;
};
} // namespace tome
