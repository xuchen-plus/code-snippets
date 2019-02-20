#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

template <typename T,
          typename TIter = decltype(std::begin(std::declval<T>())),
          typename = decltype(std::end(std::declval<T>()))>
constexpr static auto enumerate(T && iterable)
{
    struct iterator
    {
        size_t i;
        TIter iter;
        bool operator != (const iterator & other) const { return iter != other.iter; }
        void operator ++ () { ++i; ++iter; }
        auto operator * () const { return std::tie(i, *iter); }
    };
    struct iterable_wrapper
    {
        T && iterable;
        auto begin() { return iterator{ 0, std::begin(iterable) }; }
        auto end() { return iterator{ 0, std::end(iterable) }; }
    };
    return iterable_wrapper{ std::forward<T>(iterable) };
}

template <typename... T>
struct iterator_helper {
    using IterableTuple = std::tuple<T...>;
    using IteratorTuple = std::tuple<decltype(std::begin(std::declval<T>()))...>;

    template<size_t ...N>
    static void incr(IteratorTuple & iters, std::index_sequence<N...>) {
        (void)std::initializer_list<int>{((void)++std::get<N>(iters), 0)...};
    }

    template<size_t ...N>
    static auto deref(IteratorTuple & iters, std::index_sequence<N...>) {
        return std::forward_as_tuple(*std::get<N>(iters)...);
    }

    template<size_t ...N>
    static auto begin(IterableTuple & iterables, std::index_sequence<N...>) {
        return std::make_tuple(std::begin(std::get<N>(iterables))...);
    }

    template<size_t ...N>
    static auto end(IterableTuple & iterables, std::index_sequence<N...>) {
        return std::make_tuple(std::end(std::get<N>(iterables))...);
    }
};

template <typename ...T>
constexpr static auto zip(std::tuple<T...> iterable)
{
    using IterableTuple = std::tuple<T...>;
    using IteratorTuple = std::tuple<decltype(std::begin(std::declval<T>()))...>;
    constexpr size_t n = std::tuple_size<IterableTuple>::value;
    using idx_t = std::make_index_sequence<n>;

    struct iterator
    {
        IteratorTuple iters;
        bool operator != (const iterator & other) const { return iters != other.iters; }
        void operator ++ () {
            iterator_helper<T...>::template incr(iters, idx_t{});
        }
        auto operator * () {
            return iterator_helper<T...>::template deref(iters, idx_t{});
        }
    };

    struct iterable_wrapper
    {
        IterableTuple iterable;
        auto begin() { return iterator{ iterator_helper<T...>::template begin(iterable, idx_t{}) }; }
        auto end() { return iterator{ iterator_helper<T...>::template end(iterable, idx_t{}) }; }
    };

    return iterable_wrapper{iterable};
}

int main(int argc, char ** argv) {
    std::vector<int> v1{1,3,5,7,9};
    std::vector<float> v2{2.0,4.0,6.0,8.0,10.0};

    for (const auto &[vv1, vv2] : zip(std::forward_as_tuple(v1, v2))) {
        std::cout << vv1 << "," << vv2 << std::endl;
    }

    for (const auto &[i, vv1] : enumerate(v1)) {
        std::cout << i << "," << vv1 << std::endl;
    }
    return 0;
}
