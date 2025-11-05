#include "lexer.h"
#include <iostream>
#include <sstream>
#include <cassert>


#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    try { \
        std::cout << "Running " << #name << "... "; \
        test_##name(); \
        std::cout << "PASSED\n"; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "FAILED: " << e.what() << "\n"; \
        tests_failed++; \
    } \
} while(0)

#define ASSERT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        std::ostringstream oss; \
        oss << "Expected " << (expected) << " but got " << (actual); \
        throw std::runtime_error(oss.str()); \
    } \
} while(0)

#define ASSERT_TRUE(condition) do { \
    if (!(condition)) { \
        throw std::runtime_error("Condition failed: " #condition); \
    } \
} while(0)

// Test: Empty input returns EOF
TEST(empty_input) {
    std::istringstream input("");
    Lexer lexer(input);
    ASSERT_EQ(tok_eof, lexer.gettok());
}

// Test: Whitespace only returns EOF
TEST(whitespace_only) {
    std::istringstream input("   \n\t  ");
    Lexer lexer(input);
    ASSERT_EQ(tok_eof, lexer.gettok());
}

// Test: Simple identifier
TEST(simple_identifier) {
    std::istringstream input("hello");
    Lexer lexer(input);
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "hello");
}

// Test: Multiple identifiers
TEST(multiple_identifiers) {
    std::istringstream input("foo bar baz");
    Lexer lexer(input);

    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "foo");

    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "bar");

    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "baz");

    ASSERT_EQ(tok_eof, lexer.gettok());
}

// Test: 'def' keyword
TEST(def_keyword) {
    std::istringstream input("def");
    Lexer lexer(input);
    ASSERT_EQ(tok_def, lexer.gettok());
}

// Test: 'extern' keyword
TEST(extern_keyword) {
    std::istringstream input("extern");
    Lexer lexer(input);
    ASSERT_EQ(tok_extern, lexer.gettok());
}

// Test: Keywords with surrounding identifiers
TEST(keywords_mixed) {
    std::istringstream input("foo def bar extern baz");
    Lexer lexer(input);

    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_EQ(tok_def, lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_EQ(tok_extern, lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
}

// Test: Integer number
TEST(integer_number) {
    std::istringstream input("123");
    Lexer lexer(input);
    ASSERT_EQ(tok_number, lexer.gettok());
    ASSERT_EQ(123.0, lexer.getNumVal());
}

// Test: Floating point number
TEST(float_number) {
    std::istringstream input("3.14159");
    Lexer lexer(input);
    ASSERT_EQ(tok_number, lexer.gettok());
    ASSERT_TRUE(lexer.getNumVal() > 3.14 && lexer.getNumVal() < 3.15);
}

// Test: Number starting with decimal point
TEST(decimal_point_start) {
    std::istringstream input(".5");
    Lexer lexer(input);
    ASSERT_EQ(tok_number, lexer.gettok());
    ASSERT_EQ(0.5, lexer.getNumVal());
}

// Test: Multiple numbers
TEST(multiple_numbers) {
    std::istringstream input("1 2.5 3.0");
    Lexer lexer(input);

    ASSERT_EQ(tok_number, lexer.gettok());
    ASSERT_EQ(1.0, lexer.getNumVal());

    ASSERT_EQ(tok_number, lexer.gettok());
    ASSERT_EQ(2.5, lexer.getNumVal());

    ASSERT_EQ(tok_number, lexer.gettok());
    ASSERT_EQ(3.0, lexer.getNumVal());
}

// Test: Single character tokens
TEST(single_char_tokens) {
    std::istringstream input("+ - * / ( )");
    Lexer lexer(input);

    ASSERT_EQ('+', lexer.gettok());
    ASSERT_EQ('-', lexer.gettok());
    ASSERT_EQ('*', lexer.gettok());
    ASSERT_EQ('/', lexer.gettok());
    ASSERT_EQ('(', lexer.gettok());
    ASSERT_EQ(')', lexer.gettok());
}

// Test: Comment skipping
TEST(comment_skipped) {
    std::istringstream input("foo # this is a comment\nbar");
    Lexer lexer(input);

    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "foo");

    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "bar");
}

// Test: Comment at end of file
TEST(comment_at_eof) {
    std::istringstream input("foo # comment");
    Lexer lexer(input);

    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "foo");

    ASSERT_EQ(tok_eof, lexer.gettok());
}

// Test: Full function definition
TEST(function_definition) {
    std::istringstream input("def foo(x y) x + y * 2");
    Lexer lexer(input);

    ASSERT_EQ(tok_def, lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "foo");
    ASSERT_EQ('(', lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "x");
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "y");
    ASSERT_EQ(')', lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_EQ('+', lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_EQ('*', lexer.gettok());
    ASSERT_EQ(tok_number, lexer.gettok());
    ASSERT_EQ(2.0, lexer.getNumVal());
}

// Test: Extern declaration
TEST(extern_declaration) {
    std::istringstream input("extern sin(x)");
    Lexer lexer(input);

    ASSERT_EQ(tok_extern, lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "sin");
    ASSERT_EQ('(', lexer.gettok());
    ASSERT_EQ(tok_identifier, lexer.gettok());
    ASSERT_TRUE(lexer.getIdentifier() == "x");
    ASSERT_EQ(')', lexer.gettok());
}

int main() {
    std::cout << "Running Lexer Tests\n";
    std::cout << "===================\n\n";

    int tests_passed = 0;
    int tests_failed = 0;

    RUN_TEST(empty_input);
    RUN_TEST(whitespace_only);
    RUN_TEST(simple_identifier);
    RUN_TEST(multiple_identifiers);
    RUN_TEST(def_keyword);
    RUN_TEST(extern_keyword);
    RUN_TEST(keywords_mixed);
    RUN_TEST(integer_number);
    RUN_TEST(float_number);
    RUN_TEST(decimal_point_start);
    RUN_TEST(multiple_numbers);
    RUN_TEST(single_char_tokens);
    RUN_TEST(comment_skipped);
    RUN_TEST(comment_at_eof);
    RUN_TEST(function_definition);
    RUN_TEST(extern_declaration);

    std::cout << "\n===================\n";
    std::cout << "Tests passed: " << tests_passed << "\n";
    std::cout << "Tests failed: " << tests_failed << "\n";

    return tests_failed > 0 ? 1 : 0;
}
