module;

#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

export module query_processor:statement;

export import types;
import :logical_plan;
import :expr;

namespace tome
{

export struct statement
{
  virtual std::string explain() = 0;
  virtual void execute() = 0;
  virtual ~statement() = default;
};

export struct insert_statement : public statement
{
  insert_statement(const std::string &collection, document &doc) : collection_(collection), document_(doc) {}

  auto explain() -> std::string override
  {
    return std::format("collection({})\n\tdocument: {}", collection_, document_.dump());
  }

  auto execute() -> void override { throw std::runtime_error("not implemented"); }

private:
  std::string collection_;
  json document_;
};

export struct find_statement : public statement
{
  find_statement(std::string &collection, const query &query) : collection_(collection), query_(query) {}

  auto explain() -> std::string override
  {
    auto plan = build_plan();
    return std::visit(explainer_, plan);
  }

  auto execute() -> void override { throw std::runtime_error("not implemented"); }

private:
  auto build_plan() -> logical_plan
  {
    logical_plan plan = scan{.collection = collection_};

    if (query_.filter.has_value())
    {
      plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)), .predicate = parse_(query_.filter)};
    }

    if (query_.projection.has_value())
    {
      std::vector<std::string> fields{};

      for (const auto &field : query_.projection.value())
      {
        fields.push_back(field);
      }

      plan = projection{.fields = std::move(fields), .child = std::make_unique<logical_plan>(std::move(plan))};
    }
    return plan;
  }

  std::string collection_;
  query query_;
  explainer explainer_;
  predicate_parser parse_;
};

export struct update_statement : public statement
{
  update_statement(const std::string &name, const json &query, const json &data)
      : collection_(name), query_(query), data_(data)
  {
  }

  auto explain() -> std::string override
  {
    auto plan = build_plan();
    auto explanation = std::visit(explainer_, plan);
    return std::format("update({})\n\t{}", collection_, explanation);
  }

  auto execute() -> void override { throw std::runtime_error("not implemented"); }

private:
  auto build_plan() -> logical_plan
  {
    logical_plan plan = scan{.collection = collection_};
    plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)), .predicate = parse_(query_)};
    return plan;
  }

  std::string collection_;
  json query_;
  json data_;
  explainer explainer_;
  predicate_parser parse_;
};

export struct delete_statement : public statement
{
  delete_statement(const std::string &name, const json &query) : collection_(name), query_(query) {}

  auto explain() -> std::string override
  {
    auto plan = build_plan();
    auto explanation = std::visit(explainer_, plan);
    return std::format("delete({})\n\t{}", collection_, explanation);
  }

  auto execute() -> void override { throw std::runtime_error("not implemented"); }

private:
  auto build_plan() -> logical_plan
  {
    logical_plan plan = scan{.collection = collection_};
    plan = selection{.child = std::make_unique<logical_plan>(std::move(plan)), .predicate = parse_(query_)};
    return plan;
  }

  std::string collection_;
  json query_;
  explainer explainer_;
  predicate_parser parse_;
};
} // namespace tome
