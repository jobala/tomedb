module;

export module analyzer:parser;

import :tokenizer;
import :ast;

export namespace tomedb
{
class parser
{
public:
  parser(const tokenizer &tokenizer) : tokenizer_(tokenizer) {};
  auto parse() -> ast { return ast{}; }

private:
  tokenizer tokenizer_;
};
} // namespace tomedb
