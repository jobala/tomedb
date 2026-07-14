module;
#include <iostream>
#include <utility>
#include <variant>
export module query_processor:executor;

import :logical_plan;
import types;

namespace tome
{
export struct result
{
};

export struct executor
{
  std::pair<bool, result> operator()(const insert &plan)
  {
    std::cout << plan.collection;
    return std::make_pair(false, result{});
  }

  std::pair<bool, result> operator()(const projection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    std::cout << res.first;

    std::cout << plan.fields[0];
    return std::make_pair(true, result{});
  }

  std::pair<bool, result> operator()(const selection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    std::cout << res.first;

    json doc{};
    plan.evaluate(doc);
    return std::make_pair(true, result{});
  }

  std::pair<bool, result> operator()(const scan &plan)
  {
    std::cout << plan.collection;
    return std::make_pair(true, result{});
  }
};
} // namespace tome
