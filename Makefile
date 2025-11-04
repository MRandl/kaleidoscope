CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
SRCDIR = src
TESTDIR = tests
INCDIR = include
BUILDDIR = build

# Source files
LEXER_SRC = $(SRCDIR)/lexer.cpp
TEST_SRC = $(TESTDIR)/lexer_test.cpp

# Output binaries
TEST_BIN = $(BUILDDIR)/lexer_test

.PHONY: all test clean

all: test

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Build test executable
$(TEST_BIN): $(LEXER_SRC) $(TEST_SRC) $(INCDIR)/lexer.h | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(LEXER_SRC) $(TEST_SRC) -o $(TEST_BIN)

# Run tests
test: $(TEST_BIN)
	@echo "Running tests..."
	@$(TEST_BIN)

# Clean build artifacts
clean:
	rm -rf $(BUILDDIR)
