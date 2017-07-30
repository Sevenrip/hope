#pragma once

namespace hope {
namespace detail {

template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...> { using type = seq<S...>; };

template<typename Function, typename Tuple, size_t ... I>
auto callFunctionWithTupleAsArgsHelper(Function f, Tuple t, std::index_sequence<I ...>) {
     return f(std::get<I>(t) ...);
}

template<typename Function, typename Tuple>
auto callFunctionWithTupleAsArgs(Function f, Tuple tuple) {
    static const auto size = std::tuple_size<Tuple>::value;
    static auto sequence = std::make_index_sequence<size>{};
    return callFunctionWithTupleAsArgsHelper(f, tuple, sequence);     
}


template<typename T>
using remove_cv_and_ref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <typename T> struct identity { typedef T type; };

}
}