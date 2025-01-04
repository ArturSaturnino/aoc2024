#pragma once

#include <ranges>
#include <concepts>
#include <array>
#include <type_traits>
#include <utility>


template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;


template<typename Tuple>
concept tupleLike = requires (Tuple&& t){ std::get<0>(t); };

template<arithmetic T, size_t N>
class vec
{

public:

	vec(const std::ranges::range auto& r)
	{
		std::copy_n(r.begin(), N, m_data.begin());
	}

	vec(const std::array<T, N>& data) :
		m_data{ data }
	{
	}


	template<typename Tuple> requires tupleLike<Tuple>
	vec(const Tuple& tuple)
	{
		static_assert(N == std::tuple_size_v<Tuple>);
		tupleConstructorHelper<Tuple, N>(tuple);
	}

	vec(void)
	{
	}

	const std::array<T, N>& getData() const
	{
		return m_data;
	}

	T& operator[](std::size_t idx)
	{
		return m_data[idx];
	}

	const T& operator[](std::size_t idx) const
	{
		return m_data[idx];
	}

	vec operator+(const vec& other) const
	{
		vec out;
		for (int i = 0; i < N; ++i)
		{
			out.m_data[i] = m_data[i] + other.m_data[i];
		}
		return out;
	}

	vec operator-(const vec& other) const
	{
		vec out;
		for (int i = 0; i < N; ++i)
		{
			out.m_data[i] = m_data[i] - other.m_data[i];
		}
		return out;
	}

	vec operator*(const T& scalar) const
	{
		vec out;
		for (int i = 0; i < N; ++i)
		{
			out.m_data[i] = m_data[i] * scalar;
		}
		return out;
	}

	vec& operator+=(const vec& other)
	{
		for (int i = 0; i < N; ++i)
		{
			m_data[i] += other.m_data[i];
		}
		return *this;
	}

	vec& operator-=(const vec& other)
	{
		vec out;
		for (int i = 0; i < N; ++i)
		{
			m_data[i] -= other.m_data[i];
		}
		return *this;
	}

	vec& operator*=(const T& scalar)
	{
		vec out;
		for (int i = 0; i < N; ++i)
		{
			m_data[i] *= scalar;
		}
		return *this;
	}

	bool operator==(const vec& other) const
	{
		for (int i = 0; i < N; ++i)
		{
			if (m_data[i] != other.m_data[i])
				return false;
		}
		return true;
	}

	bool operator<(const vec& other) const
	{
		for (int i = 0; i < N; ++i)
		{
			if (m_data[i] < other.m_data[i])
				return true;
			if (m_data[i] > other.m_data[i])
				return false;
		}
		return false;
	}

	bool operator>(const vec& other) const
	{
		for (int i = 0; i < N; ++i)
		{
			if (m_data[i] < other.m_data[i])
				return false;
			if (m_data[i] > other.m_data[i])
				return true;
		}
		return false;
	}


private:

	template<typename Tuple, std::size_t I>
	void tupleConstructorHelper(const Tuple& tuple)
	{
		m_data[I - 1] = std::get<I-1>(tuple);

		if constexpr (I != 1) 
		{
			tupleConstructorHelper<Tuple, I - 1>(tuple);
		}
	}


	std::array<T, N> m_data;
};