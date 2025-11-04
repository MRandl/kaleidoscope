#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <istream>

// Token types
enum Token {
  tok_eof = -1,
  tok_def = -2,
  tok_extern = -3,
  tok_identifier = -4,
  tok_number = -5,
};

class Lexer {
public:
  // Constructor taking an input stream
  explicit Lexer(std::istream& input);

  // Get the next token from input
  int gettok();

  // Accessors for token values
  const std::string& getIdentifier() const { return identifier_str_; }
  double getNumVal() const { return num_val_; }

private:
  // Advance to next character
  int advance();

  // Skip comment lines
  void skipComment();

  // Member variables
  std::istream& input_;
  int last_char_;
  std::string identifier_str_;
  double num_val_;
};

#endif // LEXER_H
