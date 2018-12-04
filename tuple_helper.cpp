#include <boost/optional.hpp>
#include <tuple>

#include <iostream>
#include <cstdlib>
#include <memory>

#include "demangle.hpp"

template <typename T>
using optional = boost::optional<T>;

template <typename... Ts>
struct tuple_reverse;

template <>
struct tuple_reverse<std::tuple<>> { 
    using type = std::tuple<>;
};

template <typename T, typename... Ts>
struct tuple_reverse<std::tuple<T, Ts...>> {
    using head = std::tuple<T>;
    using tail = typename tuple_reverse<std::tuple<Ts...>>::type;

    using type =
        decltype(std::tuple_cat(std::declval<tail>(), std::declval<head>()));
};

template <typename ...T>
struct make_tuple_element_optional;

template <>
struct make_tuple_element_optional<std::tuple<>> { 
    using type = std::tuple<>;
};

template <typename T, typename... Ts>
struct make_tuple_element_optional<std::tuple<T, Ts...>> {
    using head = std::tuple<optional<T>>;
    using tail = typename make_tuple_element_optional<std::tuple<Ts...>>::type;

    using type =
        decltype(std::tuple_cat(std::declval<tail>(), std::declval<head>()));
};

int main(int argc, char ** argv) {
    std::tuple<int, double, float> t;
    std::cout << demangle(typeid(make_tuple_element_optional<std::tuple<int, double, float>>::type).name()) << std::endl;
}
