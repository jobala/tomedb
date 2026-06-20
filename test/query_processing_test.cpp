#include <gtest/gtest.h>

import db;
import types;

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
  std::string explanation;

  tome::query find_by_title{.filter = tome::document{{"title", "inferno"}}};
  explanation = books.find(find_by_title)->explain();
  ASSERT_EQ("filter(title=\"inferno\")\n\tscan(books)", explanation);

  tome::query find_by_age_comparison{.filter = tome::document{{"age", {{"$gt", 5}}}}};
  explanation = books.find(find_by_age_comparison)->explain();
  ASSERT_EQ("filter(age>5)\n\tscan(books)", explanation);

  tome::query find_by_title_and_author{.filter = tome::document{{"$and",
                                                                 {
                                                                     {{"title", "inferno"}},
                                                                     {{"author", "dan brown"}},
                                                                 }}}};
  explanation = books.find(find_by_title_and_author)->explain();
  ASSERT_EQ("filter((title=\"inferno\") and (author=\"dan brown\"))\n\tscan(books)", explanation);
}

TEST(query_processing, update_query) {}
TEST(query_processing, delete_query) {}
