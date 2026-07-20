module;
#include <string>
#include <variant>
export module result_types;

import uuidv7;
import types;

export namespace tome
{
struct insert_result
{
  std::string inserted_id;
};

struct update_result
{
  int matched_count;
  int modified_count;
};

struct delete_result
{
  int deleted_count;
};

struct get_result
{
  json doc;
};

using result = std::variant<insert_result, update_result, delete_result, get_result>;
} // namespace tome
