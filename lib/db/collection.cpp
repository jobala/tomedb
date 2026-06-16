module;

#include <string>

export module db:collection;
export import query_processor;

namespace tome
{
struct collection
{
  collection(const std::string &name) : name_(name) {}

  auto explain_insert(tome::document &doc) -> std::string
  {
    tome::insert_statement statement(this->name_, doc);
    return statement.explain();
  }

  auto explain_find(const query &query) -> std::string
  {
    tome::find_statement statement(this->name_, query);
    return statement.explain();
  }

private:
  std::string name_;
};
} // namespace tome
