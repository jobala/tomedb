module;

#include <any>
#include <string>
#include <unordered_map>

export module query_processor:statement;

namespace tomedb
{
using document = std::unordered_map<std::string, std::any>;

struct logical_expr
{
};

struct statement
{
  virtual logical_expr build_plan() = 0;
};

export struct insert_statement : public statement
{
  insert_statement(const std::string &collection, document &doc) : collection_(collection), document_(doc) {}

  auto build_plan() -> logical_expr override { return logical_expr{}; }

private:
  std::string collection_;
  std::unordered_map<std::string, std::any> document_;
};

} // namespace tomedb
