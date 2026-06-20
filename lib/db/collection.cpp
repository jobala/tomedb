module;

#include <memory>
#include <string>

export module db:collection;
export import query_processor;

namespace tome
{
struct collection
{
  collection(const std::string &name) : name_(name) {}

  auto insert(tome::document &doc) -> std::unique_ptr<tome::statement>
  {
    return std::make_unique<tome::insert_statement>(this->name_, doc);
  }

  auto find(const query &query) -> std::unique_ptr<tome::statement>
  {
    return std::make_unique<tome::find_statement>(this->name_, query);
  }

private:
  std::string name_;
};
} // namespace tome
