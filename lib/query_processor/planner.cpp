module;

#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

export module query_processor:planner;

export import types;
import :logical_plan;
import :expr;

namespace tome
{
export struct planner
{
  template <typename T>
  auto plan(const statement_repl &statement) -> logical_plan
  {
    T t{};
    return t.plan(statement);
  }
};

export struct statement
{
  virtual std::string explain() = 0;
  virtual void execute() = 0;
  virtual ~statement() = default;
};

export struct insert_statement
{
  [[nodiscard]]
  auto plan(const statement_repl &statement) const -> logical_plan
  {
    return insert{.collection = statement.collection, .doc = statement.doc};
  }
};

export struct find_statement
{
  [[nodiscard]]
  auto plan(const statement_repl &statement) const -> logical_plan
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
  auto plan(const statement_repl &statement) const -> logical_plan
  {
    logical_plan plan = scan{.collection = statement.collection};
    plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)), .predicate = parse_(statement.filter)};
    return plan;
  }

  predicate_parser parse_{};
};

export struct delete_statement
{

  auto plan(const statement_repl &statement) const -> logical_plan
  {
    logical_plan plan = scan{.collection = statement.collection};
    plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)), .predicate = parse_(statement.filter)};
    return plan;
  }

  predicate_parser parse_;
};
} // namespace tome
