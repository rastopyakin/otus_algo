#ifndef TEST_HPP
#define TEST_HPP

#include "types.hpp"
#include <functional>
#include <vector>
#include <tuple>
#include <iterator>
#include <iostream>

using SearchFunT = pos_type(std::string_view, std::string_view, pos_type);
using StringFunT = std::function<pos_collection_type(std::string_view)>;
using SearchFunAllT = pos_collection_type(std::string_view, std::string_view);
using TestCaseT = std::tuple<std::string, std::string, pos_collection_type>;

void test();

template <class SearchFun>
void test_random(SearchFun *search_f, size_t text_len = 100, size_t pattern_len = 15);

bool test_case(SearchFunT search_f, const TestCaseT &t);
bool test_case(SearchFunAllT search_f, const TestCaseT &t);

void test_cases(SearchFunT search_f, const std::vector<TestCaseT> &tests);
void test_cases(SearchFunAllT search_f, const std::vector<TestCaseT> &tests);

void test_comparative(StringFunT f1, StringFunT f2, size_t len = 100);

void test_complex(SearchFunT search_f);
void test_complex(SearchFunAllT search_f);

void print(const TestCaseT &t);

template <class T, template <class, class...> class Cont, class... Rest>
auto operator<<(std::ostream &os, const Cont<T, Rest...> &v)
    -> std::enable_if_t<!std::is_same_v<T, char>, std::ostream &> {

  std::copy(std::begin(v), std::end(v), std::ostream_iterator<T>{os, " "});
  return os;
}

const static std::vector<TestCaseT> TESTS{
    {{""}, {""}, {}},
    {{"aa"}, {"aaaaa"}, {}},
    {"aaa", "", {}},
    {"aaa", "z", {}},
    {"aaa", "a", {0, 1, 2}},
    {"aaaa", "aa", {0, 1, 2}},
    {"aab", "b", {2}},
    {"aaab", "ab", {2}},
    {"abababad", "abad", {4}},
    {"ababab", "ab", {0, 2, 4}},
    {"abababa", "aba", {0, 2, 4}},
    {"abcdabcabcdabcdabc", "abcdabc", {0, 7, 11}},
    {"abazzzba", "aba", {0}},
    {"abc", "abc", {0}},
    {"aabbc", "abc", {}}        // will hang if Boyer-Moore uses badchar only
};

#endif /* TEST_HPP */
