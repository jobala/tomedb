#include <any>
#include <string>
#include <unordered_map>
#include <variant>

namespace tomedb
{

struct insert_statement
{
  std::string collection;
  std::unordered_map<std::string, std::any> document;
};

using statement = std::variant<insert_statement>;
} // namespace tomedb
