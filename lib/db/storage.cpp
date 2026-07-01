module;

#include "rocksdb/status.h"
#include <memory>
#include <optional>
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>
#include <stdexcept>
#include <string_view>

export module db:storage;

namespace tome
{
struct storage
{
  storage(std::string db_path)
  {
    rocksdb::Options options{};
    options.create_if_missing = true;

    auto status = rocksdb::DB::Open(options, db_path, &db_);
    if (!status.ok())
    {
      throw std::runtime_error(status.ToString());
    }
  }

  auto put(std::string_view key, std::string_view value) -> void
  {
    auto status = db_->Put(write_options_, key, value);
    if (!status.ok())
    {
      throw std::runtime_error(status.ToString());
    }
  }

  auto get(std::string_view key) -> std::optional<std::string>
  {
    std::string value;
    auto status = db_->Get(read_options_, key, &value);

    if (status.IsNotFound())
    {
      return std::nullopt;
    }

    if (!status.ok())
    {
      throw std::runtime_error(status.ToString());
    }

    return value;
  }

  auto erase(std::string_view key) -> void
  {
    auto status = db_->Delete(write_options_, key);
    if (!status.ok())
    {
      throw std::runtime_error(status.ToString());
    }
  }

private:
  std::unique_ptr<rocksdb::DB> db_;
  rocksdb::ReadOptions read_options_;
  rocksdb::WriteOptions write_options_;
};
} // namespace tome
