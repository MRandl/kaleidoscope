#include "lexer.h"
#include <cctype>

Lexer::Lexer(std::istream& input) 
  : input_(input), last_char_(' '), num_val_(0.0) {
  // Pre-allocate reasonable capacity to avoid reallocations
  identifier_str_.reserve(64);
}

int Lexer::advance() {
  last_char_ = input_.get();
  return last_char_;
}

void Lexer::skipComment() {
  // Skip until end of line
  int c;
  do {
    c = advance();
  } while (c != EOF && c != '\n' && c != '\r');
  
  if (c != EOF) {
    advance();
  }
}

int Lexer::gettok() {
  // Skip whitespace
  while (isspace(last_char_)) {
    advance();
  }

  // Identifier: [a-zA-Z][a-zA-Z0-9]*
  if (isalpha(last_char_)) {
    identifier_str_.clear();
    identifier_str_ += static_cast<char>(last_char_);
    
    while (isalnum(advance())) {
      identifier_str_ += static_cast<char>(last_char_);
    }

    if (identifier_str_ == "def") {
      return tok_def;
    }
    if (identifier_str_ == "extern") {
      return tok_extern;
    }
    return tok_identifier;
  }

  // Number: [0-9.]+
  if (isdigit(last_char_) || last_char_ == '.') {
    std::string num_str;
    num_str.reserve(32);
    
    do {
      num_str += static_cast<char>(last_char_);
      advance();
    } while (isdigit(last_char_) || last_char_ == '.');

    num_val_ = strtod(num_str.c_str(), nullptr);
    return tok_number;
  }

  // Comment until end of line
  if (last_char_ == '#') {
    skipComment();
    return gettok();
  }

  // End of file
  if (last_char_ == EOF) {
    return tok_eof;
  }

  // Otherwise, return the character as its ASCII value
  int this_char = last_char_;
  advance();
  return this_char;
}
