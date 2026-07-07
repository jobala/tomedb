module;
#include <format>
#include <iostream>
export module query_processor:planner;

import :logical_plan;
import types;

namespace tome
{

export struct planner
{
  template <typename T>
  auto plan(const statement_repl &statement) -> logical_plan
  {
    T t{};
    return t.plan(statement);
  }
};
} // namespace tome
