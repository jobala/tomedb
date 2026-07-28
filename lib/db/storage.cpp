module;
#include "rocksdb/iterator.h"
#include "rocksdb/status.h"
#include <expected>
#include <memory>
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
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

  auto put(std::string_view key, std::string_view value) -> std::expected<std::string, std::runtime_error>
  {
    auto status = db_->Put(write_options_, key, value);
    if (!status.ok())
    {
      return std::unexpected(std::runtime_error(status.ToString()));
    }

    return static_cast<std::string>(key);
  }

  auto get(std::string_view key) -> std::expected<std::string, std::runtime_error>
  {
    std::string value;
    auto status = db_->Get(read_options_, key, &value);

    if (status.IsNotFound())
    {
      return std::unexpected(std::runtime_error("key not found"));
    }

    if (!status.ok())
    {
      return std::unexpected(std::runtime_error(status.ToString()));
    }

    return value;
  }

  auto erase(std::string_view key) -> std::expected<std::string, std::runtime_error>
  {
    auto status = db_->Delete(write_options_, key);
    if (!status.ok())
    {
      return std::unexpected(std::runtime_error(status.ToString()));
    }

    return static_cast<std::string>(key);
  }

  auto next(const std::string &prefix) -> std::expected<std::string, std::runtime_error>
  {
    auto found = iters.find(prefix);
    if (found == iters.end())
    {
      read_options_.prefix_same_as_start = true;
      auto iter = std::unique_ptr<rocksdb::Iterator>(db_->NewIterator(read_options_));
      iter->Seek(rocksdb::Slice(prefix));
      iters.insert({prefix, std::move(iter)});
    }

    if (!iters[prefix]->Valid())
    {
      return std::unexpected(std::runtime_error("invalid iterator"));
    }

    auto res = iters[prefix]->value();
    iters[prefix]->Next();

    return res.ToString();
  }

  auto reset(const std::string &prefix) -> void { iters[prefix]->Reset(); }

private:
  std::unique_ptr<rocksdb::DB> db_;
  rocksdb::ReadOptions read_options_;
  rocksdb::WriteOptions write_options_;
  std::unordered_map<std::string, std::unique_ptr<rocksdb::Iterator>> iters;
};
} // namespace tome
