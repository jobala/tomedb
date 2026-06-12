module;

#include <format>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

export module query_processor:statement;

namespace tome
{
using json = nlohmann::json;
export using document = std::unordered_map<std::string, json>;

struct logical_expr
{
};

struct statement
{
  virtual std::string explain() = 0;
  virtual void execute() = 0;
};

export struct insert_statement : public statement
{
  insert_statement(const std::string &collection, document &doc) : collection_(collection), document_(doc) {}

  auto explain() -> std::string override
  {
    return std::format("collection({})\n\tdocument: {}", collection_, document_.dump());
  }

  auto execute() -> void override {}

private:
  std::string collection_;
  json document_;
};
} // namespace tome
