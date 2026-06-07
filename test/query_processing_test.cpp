#include <gtest/gtest.h>

import db;

TEST(query_processing, insert_query)
{
  tomedb::db library{};

  // auto books = library.collection("book");
  //
  // tomedb::document book{{"author", "C.S. Lewis"}, {"title", "The Screwtape Letters"}};

  // insert first generates the insert statement which will be coverted to a logical plan.
  // if explain is true, it prints the logical plan
  // otherwise, it executes the plan
  //   auto plan = books.insert(book, true);
  ASSERT_EQ("books", library.collection("books"));
}

TEST(query_processing, find_query) {}
TEST(query_processing, find_query_with_predicate) {}
TEST(query_processing, update_query) {}
TEST(query_processing, update_query_with_predicate) {}
TEST(query_processing, delete_query) {}
