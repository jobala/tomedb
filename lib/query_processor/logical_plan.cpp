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
    auto expressions = std::visit(print_, selection.predicate);
    return std::format("filter({})\n\t{}", expressions, child);
  }

  auto operator()(const projection &projection) const -> std::string
  {
    auto child = std::visit(*this, *projection.child);
    std::string fields;

    for (const auto &field : projection.fields)
    {
      fields = std::format(",{}", field);
    }

    if (!fields.empty())
    {
      fields = fields.substr(1);
    }

    return std::format("projection({})\n\t{}", fields, child);
  }

private:
  expr_printer print_;
};
} // namespace tome
