module;

#include <string>

export module analyzer:tokenizer;

export namespace tomedb
{
enum class token_type {
  lbrace,
  rbrace,
  colon,
};

struct token
{
  token_type type;
};

struct tokenizer
{
  tokenizer(const std::string &program) : program_(program) {}
  auto get_next_token() -> token { return token{}; }

  token look_ahead;

private:
  std::string program_;
};
} // namespace tomedb
