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
  [[nodiscard]]
  bool evaluate(const expr &predicate, const json &doc) const
  {
    return std::visit(overloaded{[&](const logical_expr &e) -> bool {
                                   if (e.op == expr_op::OR)
                                   {
                                     for (const auto &child : e.children)
                                     {
                                       if (evaluate(*child, doc))
                                       {
                                         return true;
                                       }
                                     }
                                     return false;
                                   }
                                   for (const auto &child : e.children)
                                   {
                                     if (!evaluate(*child, doc))
                                     {
                                       return false;
                                     }
                                   }
                                   return true;
                                 },
                                 [&](const binary_expr &e) -> bool {
                                   const auto it = doc.find(e.left);
                                   if (it == doc.end())
                                   {
                                     return false;
                                   }
                                   const json &value = *it;
                                   switch (e.op)
                                   {
                                   case expr_op::EQ:
                                     return value == e.right;
                                   case expr_op::NE:
                                     return value != e.right;
                                   case expr_op::GT:
                                     return value > e.right;
                                   case expr_op::LT:
                                     return value < e.right;
                                   case expr_op::GTE:
                                     return value >= e.right;
                                   case expr_op::LTE:
                                     return value <= e.right;
                                   default:
                                     return false; // AND/OR are not leaf ops
                                   }
                                 }},
                      predicate);
  }

  T &storage_;
};
} // namespace tome
