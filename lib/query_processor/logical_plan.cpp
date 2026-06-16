module;
#include <memory>
#include <string>
#include <variant>
#include <vector>
export module query_processor:logical_plan;

import :expr;

namespace tome
{
struct scan;
struct selection;
struct projection;

export using logical_plan = std::variant<scan, selection, projection>;

struct scan
{
  std::string collection;
};

struct selection
{
  std::unique_ptr<logical_plan> child;
  expr predicate;
};

struct projection
{
  std::vector<std::string> fields;
  std::unique_ptr<logical_plan> child;
};
} // namespace tome
