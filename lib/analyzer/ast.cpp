module;

#include <memory>
#include <string>
#include <variant>

export module analyzer:ast;

namespace tomedb
{
struct literal_expr;
struct binary_expr;
struct ast;

using expr = std::variant<literal_expr, binary_expr>;
using literal = std::variant<std::string, int, bool>;

struct literal_expr
{
  literal value;
};

struct binary_expr
{
  std::string op;
  std::unique_ptr<expr> left;
  std::unique_ptr<expr> right;
};

struct printer
{
  std::string operator()(const literal_expr &expr) const { return std::visit(*this, expr.value); }
  std::string operator()(const binary_expr &expr) const
  {
    return "(" + std::visit(*this, *expr.left) + " " + expr.op + " " + std::visit(*this, *expr.right) + ")";
  }
  std::string operator()(const int &value) const { return std::to_string(value); }
  std::string operator()(const bool &value) const { return value ? "true" : "false"; }
  std::string operator()(const std::string &value) const { return value; }
};

struct ast
{
  expr body;

  auto print() const -> std::string { return std::visit(printer_, this->body); }

private:
  printer printer_{};
};
} // namespace tomedb
