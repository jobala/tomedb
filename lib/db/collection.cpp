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
    return std::make_unique<tome::insert_statement>(name_, doc);
  }

  auto find(const query &query) -> std::unique_ptr<tome::statement>
  {
    return std::make_unique<tome::find_statement>(name_, query);
  }

  auto update(const json &query, const json &data) -> std::unique_ptr<tome::statement>
  {
    return std::make_unique<tome::update_statement>(name_, query, data);
  }

  auto delete_doc(const json &query) -> std::unique_ptr<tome::statement>
  {
    return std::make_unique<tome::delete_statement>(name_, query);
  }

private:
  std::string name_;
};
} // namespace tome
