#include <gtest/gtest.h>
#include <string>
#include <vector>

import analyzer;

using std::string;
using std::vector;

struct program
{
  string src;
  string ast;
};

TEST(TestAnalyzer, Literals)
{
  vector<program> programs{
      {"30", "30"},
      {"true", "true"},
      {"hello", "hello"},
  };

  for (const auto &prog : programs)
  {
    tomedb::tokenizer tokenizer{prog.src};
    tomedb::parser parser{tokenizer};

    const auto ast = parser.parse();
    auto printed = ast.print();
  }
}
