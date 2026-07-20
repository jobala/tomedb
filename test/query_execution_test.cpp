#include <gtest/gtest.h>
#include <iostream>

import db;
import types;

TEST(query_execution, insert)
{
  tome::db library{"library"};
  auto books = library.collection("books");

  tome::document book = {{"author", "dan brown"}, {"title", "inferno"}, {"age", 10}};
  auto res = books.insert(book)->execute();
  std::cout << "output: " << res.dump() << "\n";
}

TEST(query_execution, find) {}
TEST(query_execution, update) {}
TEST(query_execution, delete_one) {}
