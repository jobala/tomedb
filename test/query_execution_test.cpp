#include <gtest/gtest.h>
#include <iostream>

import db;
import types;

TEST(query_execution, insert)
{
  tome::db library{"library"};
  auto books = library.collection("books");

  auto res = books.insert(tome::document{{"author", "dan brown"}, {"title", "inferno"}, {"age", 10}})->execute();
  ASSERT_TRUE(!res.empty());
}

TEST(query_execution, find)
{
  tome::db library{"library"};
  auto books = library.collection("books");

  auto res = books.insert(tome::document{{"author", "dan brown"}, {"title", "inferno"}, {"age", 10}})->execute();
  auto id = res["id"].get<std::string>();

  auto book = books.find(tome::query{.filter = tome::document{{"id", id}}})->execute();
  std::cout << book.dump();
}
TEST(query_execution, update) {}
TEST(query_execution, delete_one) {}
