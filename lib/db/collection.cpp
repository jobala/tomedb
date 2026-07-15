module;
#include <string>
#include <variant>
#include <vector>
export module db:collection;
export import query_processor;

import :storage;
import types;

namespace tome
{
struct collection
{
  collection(const std::string &name, storage &store) : name_(name), executor_(executor{store}) {}

  auto insert(const tome::document &doc) -> collection *
  {
    statement statement{.collection = name_, .doc = doc};
    plan_ = planner_.plan<insert_statement>(statement);
    return this;
  }

  auto find(const query &query) -> collection *
  {
    statement statement{.collection = name_, .query = query};
    plan_ = planner_.plan<find_statement>(statement);
    return this;
  }

  auto update(const json &filter, const json &data) -> collection *
  {
    statement statement{.collection = name_, .filter = filter, .data = data};
    plan_ = planner_.plan<update_statement>(statement);
    return this;
  }

  auto delete_one(const json &filter) -> collection *
  {
    statement statement{.collection = name_, .filter = filter};
    plan_ = planner_.plan<delete_statement>(statement);
    return this;
  }

  auto explain() -> std::string { return std::visit(explainer_, plan_); }

  auto execute() -> std::vector<std::string>
  {
    std::vector<std::string> res{};
    auto [next, record] = std::visit(executor_, plan_);
    res.push_back(record);

    while (next)
    {
      auto out = std::visit(executor_, plan_);
      next = out.first;

      res.push_back(out.second);
    }

    return res;
  }

private:
  std::string name_;
  std::string op_;
  logical_plan plan_;
  planner planner_{};
  explainer explainer_{};
  executor<storage> executor_;
};
} // namespace tome
