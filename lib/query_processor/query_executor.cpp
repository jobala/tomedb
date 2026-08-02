module;
#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <vector>
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

  std::pair<bool, json> operator()(const insert &plan)
  {
    auto doc = plan.doc;
    auto id = to_string(generate_uuidv7());
    json primary_key{{"id_", id}};

    doc["id_"] = id;
    auto res = storage_.put(plan.collection + ":" + id, json::to_bson(doc));

    return std::make_pair(false, primary_key);
  }

  std::pair<bool, json> operator()(const projection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    std::cout << res.first;
    std::cout << plan.fields[0];
    return std::make_pair(true, res);
  }

  std::pair<bool, json> operator()(const selection &plan)
  {
    auto res = std::visit(*this, *plan.child);
    if (!res.first)
    {
      return std::make_pair(false, res);
    }

    if (!evaluate(plan.predicate, res.second))
    {
      return std::make_pair(true, res.second);
    }

    return std::make_pair(true, res.second);
  }

  std::pair<bool, json> operator()(const scan &plan)
  {
    auto res = storage_.next(plan.collection);
    const auto &bytes = *res;

    if (!res)
    {
      return std::make_pair(false, nullptr);
    }

    if (bytes.empty())
    {
      return std::make_pair(false, nullptr);
    }

    try
    {
      json j;
      j = json::from_bson(bytes);

      return std::make_pair(true, j);
    } catch (const json::parse_error &e)
    {
      std::cout << "Failed to parse document: " << e.what() << "\n";
      return std::make_pair(false, nullptr);
    }
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
