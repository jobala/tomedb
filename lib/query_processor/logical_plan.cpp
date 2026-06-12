module;
#include <any>
#include <string>
#include <unordered_map>
#include <variant>
module query_processor:logical_plan;

namespace tome
{
struct insert
{
  std::string collection_;
  std::unordered_map<std::string, std::any> values_;
};

using logical_plan = std::variant<insert>;
} // namespace tome
