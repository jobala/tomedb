#include <gtest/gtest.h>

import db;

TEST(query_processing, insert_query)
{
  tome::db library{};
  tome::document book = {{"author", "ngugi wa thiong'o"}};

  auto books = library.collection("books");
  auto explanation = books.insert(book)->explain();

  const auto result = "collection(books)\n\tdocument: {\"author\":\"ngugi wa thiong'o\"}";
  ASSERT_EQ(result, explanation);
}

TEST(query_processing, find_query)
{
  tome::db library{};
  tome::document book = {{"author", "dan brown"}, {"title", "inferno"}, {"age", 10}};
  auto books = library.collection("books");

  tome::query find_by_title{.filter = tome::document{{"title", "inferno"}}};

  // query find_by_title_and_author{{"title", "dan brown"}};
  // query find_by_title_or_author{{"title", "dan brown"}};
  // query find_by_age_comparison{{"title", "dan brown"}};

  auto explanation = books.find(find_by_title)->explain();
  ASSERT_EQ("filter(title=inferno)\n\tscan(books)", explanation);
}

TEST(query_processing, update_query) {}
TEST(query_processing, delete_query) {}
