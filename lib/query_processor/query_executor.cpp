module;
#include <iostream>
#include <utility>
#include <variant>
export module query_processor:executor;

import :logical_plan;
import :expr;
import types;
import uuidv7;
import result_types;

namespace tome
{
// struct expr_evaluator
// {
//   bool evaluate(expr predicate, const json &doc) const
//   {
//     return std::visit(overloaded{[&filter](const &literal) {
//                                    // doesn't make sense to have this, there's nothing to compare a literal to
//                                    return false;
//                                  },
//                                  [&filter](const &field_expr) {
//                                    // this is the base case, handle nested properties?
//                                    // return false if doc, doesn't have field
//                                    // literal should also include value
//                                    return false;
//                                  },
//                                  [&filter](const &logical_expr) {
//                                    // apply logical operation on all children
//                                    return false;
//                                  },
//                                  [&filter](const &binary_expr) {
//                                    // left and right should be true
//                                    return false;
//                                  }},
//                       predicate);
//   } // namespace tome
// };

export template <store T>
struct executor
{
  executor(T &store) : storage_(store) {}

  std::pair<bool, result> operator()(const insert &plan)
  {
    auto doc = plan.doc;
    auto id = to_string(generate_uuidv7());
    json primary_key{{"id_", id}};

    doc.insert(primary_key.begin(), primary_key.end());
    auto res = storage_.put(id, doc.dump());

    return std::make_pair(false, insert_result{.inserted_id = id});
  }

  std::pair<bool, result> operator()(const projection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    std::cout << res.first;
    std::cout << plan.fields[0];
    return std::make_pair(true, get_result{});
  }

  std::pair<bool, result> operator()(const selection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    std::cout << res.first;
    //
    // json doc{};
    // plan.evaluate(doc);
    return std::make_pair(true, get_result{});
  }

  std::pair<bool, result> operator()(const scan &plan)
  {
    std::cout << plan.collection;
    return std::make_pair(true, get_result{});
  }

private:
  T &storage_;
};
} // namespace tome
