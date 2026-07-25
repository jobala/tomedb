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
  operators op;
  std::string left;
  json right;
};

struct logical_expr
{
  operators op;
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
    return binary_expr{.op = operators::EQ, .left = key, .right = value};
  }

private:
  std::expected<operators, std::string> get_operator(const std::string &op) const
  {
    auto iter = symbol_map.find(op);
    if (iter != symbol_map.end())
    {
      return iter->second;
    }

    return std::unexpected("operator not found");
  }

  std::unordered_map<std::string, operators> symbol_map{
      {"$eq", operators::EQ}, {"$ne", operators::NE},   {"$gt", operators::GT}, {"$gte", operators::GTE},
      {"$lt", operators::LT}, {"$and", operators::AND}, {"$or", operators::OR}, {"$lte", operators::LTE},
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
    if (logical_expr.op == operators::AND)
    {
      op = "and";
    }
    else if (logical_expr.op == operators::OR)
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
  std::unordered_map<operators, std::string> symbol_map{{operators::EQ, "="}, {operators::NE, "!="},
                                                        {operators::GT, ">"}, {operators::GTE, ">="},
                                                        {operators::LT, "<"}, {operators::LTE, "<="}};
};
} // namespace tome
