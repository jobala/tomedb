module;

#include <memory>
#include <string>
#include <variant>

export module query_processor:logical_expr;

namespace tome
{
struct eq;
export using logical_expr = std::variant<eq>;

struct eq
{
  std::string op;
  std::unique_ptr<logical_expr> left;
  std::unique_ptr<logical_expr> right;
};
} // namespace tome
