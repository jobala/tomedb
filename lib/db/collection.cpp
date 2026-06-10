module;

#include <any>
#include <string>
#include <unordered_map>

export module db:collection;
import query_processor;

namespace tome
{
export using document = std::unordered_map<std::string, std::any>;

struct collection
{
  collection(const std::string &name) : name(name) {}

  auto explain_insert(document doc) -> std::string
  {
    tomedb::insert_statement state{this->name, doc};
    state.build_plan();
    return this->explain();
  }

  std::string name;

private:
  auto explain() -> std::string { return ""; }
  std::string root_page_;
};
} // namespace tome
