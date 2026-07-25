module;
#include <expected>
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

export using expr = std::variant<binary_expr, logical_expr>;
export using literal = std::variant<int, std::string>;

struct binary_expr
{
  expr_op op;
  std::string left;
  json right;
};

struct logical_expr
{
  expr_op op;
  std::vector<std::unique_ptr<expr>> children;
};

struct predicate_parser
{
  expr operator()(const json &doc) const
  {
    auto doc_iter = doc.begin();
    const auto &key = doc_iter.key();
    auto value = doc_iter.value();

    if (value.is_object())
    {
      auto value_iter = value.begin();
      auto op = get_operator(value_iter.key()).value();
      return binary_expr{.op = op, .left = key, .right = value_iter.value()};
    }

    if (value.is_array())
    {
      const auto &op = get_operator(key).value();
      std::vector<std::unique_ptr<expr>> children{};
      for (const auto &child : value)
      {
        auto child_expr = this->operator()(child);
        children.push_back(std::make_unique<expr>(std::move(child_expr)));
      }
      return logical_expr{.op = op, .children = std::move(children)};
    }
    return binary_expr{.op = expr_op::EQ, .left = key, .right = value};
  }

private:
  std::expected<expr_op, std::string> get_operator(const std::string &op) const
  {
    auto iter = symbol_map.find(op);
    if (iter != symbol_map.end())
    {
      return iter->second;
    }

    return std::unexpected("operator not found");
  }

  std::unordered_map<std::string, expr_op> symbol_map{
      {"$eq", expr_op::EQ}, {"$ne", expr_op::NE},   {"$gt", expr_op::GT}, {"$gte", expr_op::GTE},
      {"$lt", expr_op::LT}, {"$and", expr_op::AND}, {"$or", expr_op::OR}, {"$lte", expr_op::LTE},
  };
};

export struct expr_printer
{
  auto operator()(const binary_expr &binary_expr) const -> std::string
  {
    std::string print_op;
    auto it = symbol_map.find(binary_expr.op);
    if (it != symbol_map.end())
    {
      print_op = it->second;
    }

    return std::format("{}{}{}", binary_expr.left, print_op, binary_expr.right.dump());
  }

  auto operator()(const logical_expr &logical_expr) const -> std::string
  {
    std::string op;
    if (logical_expr.op == expr_op::AND)
    {
      op = "and";
    }
    else if (logical_expr.op == expr_op::OR)
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

private:
  std::unordered_map<expr_op, std::string> symbol_map{{expr_op::EQ, "="},   {expr_op::NE, "!="}, {expr_op::GT, ">"},
                                                      {expr_op::GTE, ">="}, {expr_op::LT, "<"},  {expr_op::LTE, "<="}};
};
} // namespace tome
