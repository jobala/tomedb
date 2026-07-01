#include <gtest/gtest.h>

import db;
import types;

TEST(query_execution, insert)
{
  tome::db library{"library"};
  auto books = library.collection("books");

  tome::document book = {{"author", "dan brown"}, {"title", "inferno"}, {"age", 10}};
  books.insert(book);
}

TEST(query_execution, find) {}
TEST(query_execution, update) {}
TEST(query_execution, delete_one) {}
