#include <type_traits>
#include <vector>
#include <iostream>
#include "boost/range/irange.hpp"

namespace detail
{
    template<typename L, typename R>
    struct has_operator_minus_impl
    {
        template<typename T = L, typename U = R> // template parameters here to enable SFINAE
        static auto test(T &&t, U &&u) -> decltype(t - u, void(), std::true_type{});
        static auto test(...) -> std::false_type;
        using type = decltype(test(std::declval<L>(), std::declval<R>()));
    };

    template<typename L, typename R = L>
    struct has_operator_minus : detail::has_operator_minus_impl<L, R>::type {};


    template<typename L>
    struct has_operator_plus_prefix_impl
    {
        template<typename T = L> // template parameters here to enable SFINAE
        static auto test(T &&t) -> decltype(++t, void(), std::true_type{});
        static auto test(...) -> std::false_type;
        using type = decltype(test(std::declval<L>()));
    };

    template<typename L>
    struct has_operator_plus_prefix : detail::has_operator_plus_prefix_impl<L>::type {};
} // namespace detail

template<typename It,
         //typename std::enable_if_t<detail::has_operator_minus<It>::type::value>
         typename = std::enable_if_t<true>
        >
void Minus(It begin, It end) {
    size_t count = end - begin;
    std::cout << "count: " << count << std::endl;
}

int main(int argc, char ** argv) {
    auto range = boost::irange<int>(0, 10);
    std::vector<int> v(10);
    std::cout << detail::has_operator_minus<std::vector<int>::iterator>::type() << std::endl;
    std::cout << detail::has_operator_minus<decltype(std::begin(range))>::type() << std::endl;
    std::cout << detail::has_operator_minus<decltype(std::begin(v))>::type() << std::endl;
    std::cout << detail::has_operator_minus<int>::type() << std::endl;
    std::cout << detail::has_operator_minus<std::vector<int>>::type() << std::endl;
    std::cout << std::enable_if_t<detail::has_operator_minus<decltype(std::begin(range))>::type::value, double>() << std::endl;
    std::cout << std::is_copy_constructible_v<std::vector<int>::iterator> << std::endl;
    std::cout << detail::has_operator_plus_prefix<decltype(std::begin(range))>::type::value << std::endl;
    Minus(std::begin(v), std::end(v));
    return 0;
}
