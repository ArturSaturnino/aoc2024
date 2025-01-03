#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

template<typename Tuple>
struct TupleHash
{

    template <std::size_t I>
    std::size_t hashHelper(const Tuple& args) const noexcept
    {
        return std::hash<std::tuple_element_t<I, Tuple>>{}(std::get<I>(args)) ^ (hashHelper<I-1>(args) << 1);
    }

    template <>
    std::size_t hashHelper<0>(const Tuple& args) const noexcept
    {
        return std::hash<std::tuple_element_t<0, Tuple>>{}(std::get<0>(args));
    }

    std::size_t operator()(const Tuple& args) const noexcept
    {
        return hashHelper<std::tuple_size_v<Tuple> -1> (args);
    }
};


template<typename R, typename ...Args>
class CachedFunction
{
public:

    using key_type = std::tuple<std::decay_t<Args>...>;

    CachedFunction(const std::function<R(Args...)>& f):
        m_f{f}
    {
    }

    R operator()(Args... args)
    {
        key_type key = std::make_tuple(args ...);
        if (m_cache.contains(key))
            return m_cache[key];
        return m_cache[key] = m_f(std::forward<decltype(args)>(args)...);
    }



private:

    std::unordered_map<key_type, R, TupleHash<key_type>> m_cache;
    std::function<R(Args...)> m_f;

};
