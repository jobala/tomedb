module;
#include <iostream>
#include <utility>
#include <variant>
export module query_processor:executor;

import :logical_plan;
import types;
import uuidv7;

namespace tome
{
export template <store T>
struct executor
{

  executor(T &store) : storage_(store) {}

  std::pair<bool, std::string> operator()(const insert &plan)
  {
    auto doc = plan.doc;
    auto id = to_string(generate_uuidv7());
    json primary_key{{"id_", id}};

    doc.insert(primary_key.begin(), primary_key.end());
    auto res = storage_.put(id, doc.dump());

    return std::make_pair(false, res.value());
  }

  std::pair<bool, std::string> operator()(const projection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    std::cout << res.first;

    std::cout << plan.fields[0];
    return std::make_pair(true, "");
  }

  std::pair<bool, std::string> operator()(const selection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    std::cout << res.first;

    json doc{};
    plan.evaluate(doc);
    return std::make_pair(true, "");
  }

  std::pair<bool, std::string> operator()(const scan &plan)
  {
    std::cout << plan.collection;
    return std::make_pair(true, "");
  }

private:
  T &storage_;
};
} // namespace tome
