module;
#include <string>
#include <variant>
export module db:collection;
export import query_processor;

import :storage;
import types;
import result_types;

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

  auto execute() -> json
  {
    auto [next, record] = std::visit(executor_, plan_);

    return std::visit(overloaded{
                          [](const insert_result &res) { return json{{"id", res.inserted_id}}; },
                          [](const update_result &res) {
                            return json{{"matched_count", res.matched_count}, {"modified_count", res.modified_count}};
                          },
                          [](const delete_result &res) { return json{{"deleted_count", res.deleted_count}}; },
                          [next, this](const get_result &res) {
                            auto records = json::array();
                            records.push_back(res.doc);

                            while (next)
                            {
                              execute();
                            }

                            return records;
                          },
                      },
                      record);
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
