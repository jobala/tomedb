module;

#include <memory>
#include <string>
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
      // TODO: collate all the children
      return logical_expr{.op = op, .children = std::move(children)};
    }

    field_expr json_field{.field = key};
    return binary_expr{.op = "eq",
                       .left = std::make_unique<expr>(std::move(json_field)),
                       .right = std::make_unique<expr>(std::move(json_field))};
  }
};
} // namespace tome
