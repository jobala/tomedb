module;

#include <string>

export module db:collection;
import query_processor;

namespace tome
{

struct collection
{
  collection(const std::string &name) : name(name) {}

  auto explain_insert(tome::document doc) -> std::string
  {
    tome::insert_statement statement(this->name, doc);
    return statement.explain();
  }

  std::string name;

private:
  std::string root_page_;
};
} // namespace tome
