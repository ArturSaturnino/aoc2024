#pragma once

#include "grid_helpers.h"


template <typename R, typename T>
concept RangeOfRangesOf = 
	std::ranges::range<R>&&
	std::ranges::range<std::ranges::range_value_t<R>> &&
	std::convertible_to<std::ranges::range_value_t<std::ranges::range_value_t<R>>, T>;


template <typename T>
class Grid
{
public:

	using value_type = T;
	using pointer = std::vector<std::vector<T>>::pointer;
	using reference = T&;

	using const_reference = const T&;
	using const_pointer = std::vector<std::vector<T>>::const_pointer;

	template<typename G, typename W>
	friend class GridItr;

	Grid(const RangeOfRangesOf<T> auto& data)
	{
		for (const auto& d : data)
		{
			m_data.push_back(std::vector<T>{});
			std::copy(d.begin(), d.end(), std::back_inserter(m_data.back()));
		}
		m_nRows = m_data.size();
		m_nCols = m_data.empty() ? 0 : m_data[0].size();
	};

	Grid(int64_t nRows, int64_t nCols) :
		m_data (nRows, std::vector<T>(nCols)),
		m_nRows{ nRows },
		m_nCols{ nCols }
	{
	};

	const_reference get (int64_t row, int64_t col) const
	{ 
		return m_data[m_nRows - row - 1][col]; 
	};

	reference get(int64_t row, int64_t col) 
	{ 
		return m_data[m_nRows - row - 1][col]; 
	};

	template <class Self>
	constexpr auto&& get(this Self&& self, std::pair<int64_t, int64_t> p)
	{
		return std::forward<Self>(self).get(p.first, p.second);
	}
	
	GridLine<Grid<T>> getLine(int64_t row, int64_t col, dir d)
	{
		return GridLine(row, col, d, *this);
	}

	GridLine<const Grid<T>> getLine(int64_t row, int64_t col, dir d) const
	{
		return GridLine(row, col, d, *this);
	}

	bool inBounds(int64_t row, int64_t col) const 
	{
		return row >= 0 && row < m_nRows && col >= 0 && col < m_nCols;
	}

	bool inBounds(std::pair<int64_t, int64_t> p) const
	{
		return inBounds(p.first, p.second);
	}

	GridRowMajorItr<Grid<T>> begin()
	{
		return GridRowMajorItr(GridRowMajorWalker(0, 0, m_nCols), *this);
	}

	GridRowMajorItr<Grid<T>> end()
	{
		return GridRowMajorItr(GridRowMajorWalker(m_nRows, 0, m_nCols), *this);
	}


	GridRowMajorItr<const Grid<T>> begin() const
	{
		return GridRowMajorItr(GridRowMajorWalker(0, 0, m_nCols), *this);
	}

	GridRowMajorItr<const Grid<T>> end() const
	{
		return GridRowMajorItr(GridRowMajorWalker(m_nRows, 0, m_nCols), *this);
	}


	int64_t nRows() const { return m_nRows; }
	int64_t nCols() const { return m_nCols; }

private:
	std::vector<std::vector<T>> m_data;
	int64_t m_nRows;
	int64_t m_nCols;
};

template <typename T >
struct GridTaits<Grid<T>>
{
	using value_type = typename Grid<T>::value_type;
	using pointer = typename Grid<T>::pointer;
	using reference = typename Grid<T>::reference;
};

template <typename T >
struct GridTaits<const Grid<T>>
{
	using value_type = typename Grid<T>::value_type;
	using pointer = typename Grid<T>::const_pointer;
	using reference = typename Grid<T>::const_reference;
};


static_assert(std::bidirectional_iterator<GridLineItr<Grid<char>>>);
static_assert(std::bidirectional_iterator<ConstGridLineItr<Grid<char>>>);

static_assert(std::bidirectional_iterator<GridRowMajorItr<Grid<char>>>);
static_assert(std::bidirectional_iterator<ConstGridRowMajorItr<Grid<char>>>);