module;

#include <string>

export module db;

export namespace tomedb
{

class db
{
public:
  auto collection(std::string collection_name) -> std::string { return collection_name; };
};

} // namespace tomedb
