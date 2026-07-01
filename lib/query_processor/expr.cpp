module;

#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

export module query_processor:expr;

import types;

namespace tome
{
struct binary_expr;
struct logical_expr;
struct field_expr;
struct literal_expr;

export using expr = std::variant<binary_expr, field_expr, literal_expr, logical_expr>;
export using literal = std::variant<int, std::string>;

struct field_expr
{
  std::string field;
};

struct literal_expr
{
  json value;
};

struct binary_expr
{
  std::string op;
  std::unique_ptr<expr> left;
  std::unique_ptr<expr> right;
};

struct logical_expr
{
  std::string op;
  std::vector<std::unique_ptr<expr>> children;
};

struct predicate_parser
{
  auto operator()(const json &doc) const -> expr
  {
    auto doc_iter = doc.begin();
    const auto &key = doc_iter.key();
    auto value = doc_iter.value();

    if (value.is_object())
    {
      auto value_iter = value.begin();
      field_expr left{.field = key};
      literal_expr right{.value = value_iter.value()};

      return binary_expr{
          .op = value_iter.key(), .left = std::make_unique<expr>(left), .right = std::make_unique<expr>(right)};
    }

    if (value.is_array())
    {
      const auto &op = key;
      std::vector<std::unique_ptr<expr>> children{};
      for (const auto &child : value)
      {
        auto child_expr = this->operator()(child);
        children.push_back(std::make_unique<expr>(std::move(child_expr)));
      }
      return logical_expr{.op = op, .children = std::move(children)};
    }

    field_expr json_field{.field = key};
    literal_expr json_value{.value = value};

    return binary_expr{.op = "$eq",
                       .left = std::make_unique<expr>(std::move(json_field)),
                       .right = std::make_unique<expr>(std::move(json_value))};
  }
};

export struct expr_printer
{
  auto operator()(const binary_expr &binary_expr) const -> std::string
  {
    std::string op;
    auto left = std::visit(*this, *binary_expr.left);
    auto right = std::visit(*this, *binary_expr.right);
    op = binary_expr.op;

    auto it = symbol_map.find(op);
    if (it != symbol_map.end())
    {
      op = it->second;
    }
    return std::format("{}{}{}", left, op, right);
  }

  auto operator()(const logical_expr &logical_expr) const -> std::string
  {
    std::string op;
    if (logical_expr.op == "$and")
    {
      op = "and";
    }
    else if (logical_expr.op == "$or")
    {
      op = "or";
    }
    else
    {
      throw std::runtime_error("unsupported operator");
    }

    std::string res;
    for (const auto &child : logical_expr.children)
    {
      auto printed_child = std::visit(*this, *child);
      res += std::format(" {} ({})", op, printed_child);
    }

    if (res.empty())
    {
      return "";
    }

    // remove trailing operator
    return res.substr(op.length() + 2);
  }

  auto operator()(const literal_expr &literal_expr) const -> std::string { return literal_expr.value.dump(); }
  auto operator()(const field_expr &field_expr) const -> std::string { return field_expr.field; }

private:
  std::unordered_map<std::string, std::string> symbol_map{{"$eq", "="},   {"$ne", "!="}, {"$gt", ">"},
                                                          {"$gte", ">="}, {"$lt", "<"},  {"$lte", "<="}};
};
} // namespace tome
