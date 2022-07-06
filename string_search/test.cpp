#include "test.hpp"
#include "types.hpp"
#include "utf8_tools.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <string_view>
#include <tuple>
#include <vector>

namespace {
void report(bool passed) {
  const char ANSI_RED_BEGIN[] = "\x1B[31m";
  const char ANSI_COLOR_END[] = "\033[0m";
  const char ANSI_GREEN_BEGIN[] = "\x1B[32m";

  std::cout << (passed ? ANSI_GREEN_BEGIN : ANSI_RED_BEGIN);
  std::cout << (passed ? "PASSED" : "FAILED") << ANSI_COLOR_END << "\n";
}
} // anonymous namespace

TestCaseT make_test_case(size_t text_len, size_t pattern_len) {

  assert(text_len > pattern_len);

  std::mt19937 gen;
  std::random_device rd;
  gen.seed(rd());
  std::uniform_int_distribution<pos_type> d{0, text_len - pattern_len};

  pos_type test_pos = d(gen); // codepoints, not bytes

  std::string text;
  text += utf8::random_text(test_pos);
  pos_type test_pos_bytes = text.length(); // now test position is correct for single byte encoding

  std::string pattern = utf8::random_text(pattern_len);

  text += pattern;
  text += utf8::random_text(text_len - pattern_len - test_pos);

  return {text, pattern, {test_pos_bytes}};
}

template <class SearchFun>
void test_random(SearchFun *search_f, size_t text_len, size_t pattern_len) {

  assert(text_len > pattern_len);

  TestCaseT test;

  std::vector<TestCaseT> tests;
  std::generate_n(std::back_inserter(tests), 1000,
                  std::bind(make_test_case, text_len, pattern_len));

  test_cases(search_f, tests);
}

template void test_random<SearchFunAllT>(SearchFunAllT *, size_t, size_t);
template void test_random<SearchFunT>(SearchFunT *, size_t, size_t);

void test_comparative(StringFunT f1, StringFunT f2, size_t len) {

  std::string s{};
  bool passed = true;

  for (int n = 0; n < 1000 && passed; n++) {
    s = utf8::random_text(len, u'a', u'c');
    passed = f1(s) == f2(s);
  }

  report(passed);
  if (!passed) {
    std::cout << "string: " << s << "\n";
    std::cout << "f1: " << f1(s) << "\n";
    std::cout << "f2: " << f2(s) << "\n";
  }
}

bool test_case(SearchFunT search_f, const TestCaseT &t, pos_collection_type &fn_result) {

  const auto &expected = std::get<2>(t);
  pos_type found_pos = search_f(std::get<0>(t), std::get<1>(t), 0);

  fn_result.push_back(found_pos);

  if (expected.empty())
    return found_pos == std::string_view::npos;

  return expected.front() == found_pos;
}

bool test_case(SearchFunAllT search_f, const TestCaseT &t, pos_collection_type &fn_result) {
  fn_result = search_f(std::get<0>(t), std::get<1>(t));
  return std::get<2>(t) == fn_result;
}

template <class SearchFun>
void test_cases(SearchFun search_f, const std::vector<TestCaseT> &tests) {

  bool passed = true;

  pos_collection_type fn_result {};

  auto test = tests.begin();
  for (; test != tests.end(); test++) {
    fn_result.clear();
    passed = test_case(search_f, *test, fn_result);
    if (!passed)
      break;
  }

  report(passed);

  if (!passed) {
    print(*test);
    std::cout << "got: " << fn_result << "\n";
  }
}

void test_cases(SearchFunAllT search_f, const std::vector<TestCaseT> &t) {
  test_cases<SearchFunAllT>(search_f, t);
}
void test_cases(SearchFunT search_f, const std::vector<TestCaseT> &t) {
  test_cases<SearchFunT>(search_f, t);
}

template <class SearchFun> void test_complex(SearchFun search_f) {
  std::cout << "random strings: ";
  test_random(search_f);
  std::cout << "manual test cases: ";
  test_cases(search_f, TESTS);
}

void test_complex(SearchFunT search_f) { test_complex<SearchFunT>(search_f); }
void test_complex(SearchFunAllT search_f) { test_complex<SearchFunAllT>(search_f); }

void print(const TestCaseT &t) {
  std::cout << "text:    " << std::get<0>(t) << "\n";
  std::cout << "pattern: " << std::get<1>(t) << "\n";

  std::cout << "expected: " << std::get<2>(t) << "\n";
}
