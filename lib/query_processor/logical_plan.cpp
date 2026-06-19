module;
#include <format>
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

struct explainer
{
  auto operator()(const scan &scan) const -> std::string { return std::format("scan({})", scan.collection); }
  auto operator()(const selection &selection) const -> std::string
  {
    auto child = std::visit(*this, *selection.child);
    return std::format("filter()\n\t{}", child);
  }
  auto operator()(const projection &projection) const -> std::string
  {
    auto child = std::visit(*this, *projection.child);
    return std::format("projection () \n\t{}", child);
  }
};
} // namespace tome
