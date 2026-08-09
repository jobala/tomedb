module;
#include <memory>
#include <string>
#include <utility>
export module query_processor:planner;

import types;
import :logical_plan;
import :expr;

namespace tome
{
export struct planner
{
  template <typename T>
  auto plan(const statement &statement) -> logical_plan
  {
    T t{};
    return t.plan(statement);
  }
};

export struct insert_statement
{
  [[nodiscard]]
  auto plan(const statement &statement) const -> logical_plan
  {
    return insert{.collection = statement.collection, .doc = statement.doc};
  }
};

export struct find_statement
{
  [[nodiscard]]
  auto plan(const statement &statement) const -> logical_plan
  {
    logical_plan plan = scan{.collection = statement.collection};

    if (statement.query.filter.has_value())
    {
      plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)),
                       .predicate = parse_(statement.query.filter)};
    }

    if (statement.query.projection.has_value())
    {
      std::vector<std::string> fields{};

      for (const auto &field : statement.query.projection.value())
      {
        fields.push_back(field);
      }

      plan = projection{.fields = std::move(fields), .child = std::make_unique<logical_plan>(std::move(plan))};
    }
    return plan;
  }

  predicate_parser parse_{};
};

export struct update_statement
{
  [[nodiscard]]
  auto plan(const statement &statement) const -> logical_plan
  {
    logical_plan plan = scan{.collection = statement.collection};
    plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)), .predicate = parse_(statement.filter)};
    return plan;
  }

  predicate_parser parse_{};
};

export struct delete_statement
{

  [[nodiscard]]
  auto plan(const statement &statement) const -> logical_plan
  {
    logical_plan plan = scan{.collection = statement.collection};
    plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)), .predicate = parse_(statement.filter)};
    return plan;
  }

  predicate_parser parse_;
};
} // namespace tome
