/**
 *  A switch(type) implementation inspired by the exmaple from boost::hana tutorial:
 *  http://boostorg.github.io/hana/index.html#tutorial-quickstart-any
 *
 *  The original example accepts a boost::any, which is not necessary and causes extra
 *  runtime overhead, i.e. it needs to search each of the lambdas to find type match.
 *
 *  I modify it to accept any type directly, and use if constexpr(std::is_same_v<>)
 *  to reduce runtime search thanks to the c++17 features.
 *
 */
#include <boost/hana.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace boost;

//! [cases]
template <typename T>
auto case_ = [](auto f) { return hana::make_pair(hana::type_c<T>, f); };
struct default_t;
auto default_ = case_<default_t>;
//! [cases]
//! [process]
template <typename Any, typename Default>
auto process(Any &, Default &default_) {
  return default_();
}
template <typename Any, typename Default, typename Case, typename... Rest>
auto process(Any &a, Default &default_, Case &case_,
             Rest &... rest) {
  using T = typename decltype(+hana::first(case_))::type;
  if constexpr (std::is_same_v<std::decay_t<Any>, std::decay_t<T>>) {
    return hana::second(case_)(a);
  } else {
    return process(a, default_, rest...);
  }
}
//! [process]
//! [switch_]
template <typename Any> auto switch_(Any &a) {
  return [&a](auto... cases_) {
    auto cases = hana::make_tuple(cases_...);
    auto default_ = hana::find_if(cases, [](auto const &c) {
      return hana::first(c) == hana::type_c<default_t>;
    });
    static_assert(default_ != hana::nothing,
                  "switch is missing a default_ case");
    auto rest = hana::filter(cases, [](auto const &c) {
      return hana::first(c) != hana::type_c<default_t>;
    });
    return hana::unpack(rest, [&](auto &... rest) {
      return process(a, hana::second(*default_),
                     rest...);
    });
  };
}
//! [switch_]

template <typename T> std::string ToString(const T &t) {
  std::string r = switch_(t)(
      case_<int>([](auto i) { return "int: "s + std::to_string(i); }),
      case_<char>([](auto c) { return "char: "s + std::string{c}; }),
      default_([] { return "unknown"s; }));
  return r;
}

int main(int argc, char **argv) {
  std::vector<int> v{1, 2, 3};

  std::cout << ToString(1) << std::endl;
  std::cout << ToString('c') << std::endl;
  std::cout << ToString(v) << std::endl;
  return 0;
}

