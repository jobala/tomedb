#include <gtest/gtest.h>

import db;

TEST(query_processing, insert_query)
{
  tome::db library{};
  tome::document book = {{"author", "ngugi wa thiong'o"}};

  auto books = library.collection("books");
  auto explanation = books.explain_insert(book);

  const auto result = "collection(books)\n\tdocument: {\"author\":\"ngugi wa thiong'o\"}";
  ASSERT_EQ(result, explanation);
}

TEST(query_processing, find_query) {}
TEST(query_processing, find_query_with_predicate) {}
TEST(query_processing, update_query) {}
TEST(query_processing, update_query_with_predicate) {}
TEST(query_processing, delete_query) {}
