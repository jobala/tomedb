module;

#include <string>
#include <variant>

export module db:collection;
export import query_processor;

import :storage;
import types;

namespace tome
{
struct collection
{
  collection(const std::string &name, storage &store) : name_(name), store_(store) {}

  auto insert(const tome::document &doc) -> collection *
  {
    statement_repl statement{.collection = name_, .query = doc};
    plan_ = planner_.plan<insert_statement>(statement);
    return this;
  }

  //
  // auto find(const query &query) -> std::unique_ptr<tome::statement>
  // {
  //   return std::make_unique<tome::find_statement>(name_, query);
  // }
  //
  // auto update(const json &query, const json &data) -> std::unique_ptr<tome::statement>
  // {
  //   return std::make_unique<tome::update_statement>(name_, query, data);
  // }
  //
  // auto delete_doc(const json &query) -> std::unique_ptr<tome::statement>
  // {
  //   return std::make_unique<tome::delete_statement>(name_, query);
  // }

  auto explain() -> std::string { return std::visit(explainer_, plan_); }

  auto execute() -> void { store_.put("hello", "world"); }

private:
  std::string name_;
  std::string op_;
  logical_plan plan_;
  storage &store_;
  planner planner_{};
  explainer explainer_{};
};
} // namespace tome
