#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <type_traits>
#include <vector>

enum class dir
{
	E,
	W,
	N,
	S,
	NE,
	SE,
	SW,
	NW
};

static std::array<dir,8> dirs
{
	dir::E,
	dir::W,
	dir::N,
	dir::S,
	dir::NE,
	dir::SE,
	dir::SW,
	dir::NW
};

static std::array<dir, 4> cardinal_dirs
{
	dir::E,
	dir::W,
	dir::N,
	dir::S
};


constexpr std::pair<int64_t, int64_t> moveInDir(dir direction, int64_t row, int64_t col)
{

	switch (direction)
	{
	case dir::E:
		++col;
		break;
	case dir::W:
		--col;
		break;
	case dir::N:
		++row;
		break;
	case dir::S:
		--row;
		break;
	case dir::NE:
		++row;
		++col;
		break;
	case dir::SE:
		++col;
		--row;
		break;
	case dir::SW:
		--row;
		--col;
		break;
	case dir::NW:
		++row;
		--col;
		break;
	default:
		break;
	}

	return { row, col };
}


constexpr dir rotateRight(dir d)
{
	switch (d)
	{
	case dir::E:
		return dir::S;
		break;
	case dir::W:
		return dir::N;
		break;
	case dir::N:
		return dir::E;
		break;
	case dir::S:
		return dir::W;
		break;
	case dir::NE:
		return dir::SE;
		break;
	case dir::SE:
		return dir::SW;
		break;
	case dir::SW:
		return dir::NW;
		break;
	case dir::NW:
		return dir::NE;
		break;
	default:
		break;
	}
}

constexpr dir rotateLeft(dir d)
{
	switch (d)
	{
	case dir::E:
		return dir::N;
		break;
	case dir::W:
		return dir::S;
		break;
	case dir::N:
		return dir::W;
		break;
	case dir::S:
		return dir::E;
		break;
	case dir::NE:
		return dir::NW;
		break;
	case dir::SE:
		return dir::NE;
		break;
	case dir::SW:
		return dir::SE;
		break;
	case dir::NW:
		return dir::SW;
		break;
	default:
		break;
	}
}

constexpr dir reverse(dir d)
{
	switch (d)
	{
	case dir::E:
		return dir::W;
		break;
	case dir::W:
		return dir::E;
		break;
	case dir::N:
		return dir::S;
		break;
	case dir::S:
		return dir::N;
		break;
	case dir::NE:
		return dir::SW;
		break;
	case dir::SE:
		return dir::NW;
		break;
	case dir::SW:
		return dir::NE;
		break;
	case dir::NW:
		return dir::SE;
		break;
	default:
		break;
	}
}




template <typename G>
class GridLine;

template <typename G>
class GridLineItr;

template <typename G>
using ConstGridLineItr = GridLineItr<const G>;

template <typename R, typename T>
concept RangeOfRangesOf = 
	std::convertible_to<std::ranges::range_value_t<std::ranges::range_value_t<R>>, T>
	;


template <typename T>
class Grid
{
public:

	using value_type = T;
	using pointer = std::vector<std::vector<T>>::pointer;
	using reference = T&;

	using const_reference = const T&;
	using const_pointer = std::vector<std::vector<T>>::const_pointer;


	template<typename G>
	friend class GridLineItr;

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
		return std::forward<Self>(self)->get(p.first, p.second);
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


	int64_t nRows() const { return m_nRows; }
	int64_t nCols() const { return m_nCols; }

private:
	std::vector<std::vector<T>> m_data;
	int64_t m_nRows;
	int64_t m_nCols;
};

class GridLineWalker
{
public:
	
	GridLineWalker(int64_t row, int64_t col, dir direction) :
		m_row{ row },
		m_col{col},
		m_dir{direction}
	{ }

	GridLineWalker() :
		m_row{  },
		m_col{  },
		m_dir{  }
	{
	}

	GridLineWalker& operator++()
	{
		const auto [row, col] = moveInDir(m_dir, m_row, m_col);
		m_row = row;
		m_col = col;
		return *this;
	}

	GridLineWalker operator++(int)
	{
		auto itr = *this;
		this->operator++();
		return itr;
	}

	GridLineWalker& operator--()
	{
		const auto [row, col] = moveInDir(reverse(m_dir), m_row, m_col);
		m_row = row;
		m_col = col;
		return *this;
	}

	GridLineWalker operator--(int)
	{
		auto itr = *this;
		this->operator--();
		return itr;
	}

	bool operator==(const GridLineWalker& rhs) const
	{
		return m_row == rhs.m_row && m_col == rhs.m_col;
	}

	int64_t getRow() const { return m_row; }
	int64_t getCol() const { return m_col; }

private:
	int64_t m_row;
	int64_t m_col;
	dir m_dir;
};

template <typename I>
class GridLineItrBase;

template <typename I>
bool operator==(const GridLineItrBase<I>& lhs, const GridLineItrBase<I>& rhs);

template <typename I>
class GridLineItrBase
{
public:

	using reference = std::iterator_traits<I>::reference;
	using underlVectorPtr = std::iterator_traits<I>::pointer;

	class GridLineItrBase(int64_t row, int64_t col, dir direction, underlVectorPtr ptr):
		m_walker{ row, col, direction },
		m_gridItr{ ptr }
	{
	}

	class GridLineItrBase() :
		m_walker {},
		m_gridItr{ }
	{
	}

	I& operator++()
	{
		++m_walker;
		return static_cast<I&>(*this);
	}

	I operator++(int)
	{
		const I itr = static_cast<I&>(*this);
		this->operator++();
		return itr;
	}

	I& operator--()
	{
		--m_walker;
		return static_cast<I&>(*this);
	}

	I operator--(int)
	{
		const I itr = static_cast<I&>(*this);
		this->operator--();
		return itr;
	}

	reference operator*() const
	{
		return const_cast<reference>(GridLineItrBase::operator*());
	}

	reference operator*()
	{
		return (*(m_gridItr - m_walker.getRow()))[m_walker.getCol()];
	}

	
	friend bool operator==<I> (const GridLineItrBase& lhs, const GridLineItrBase& rhs);


	int64_t getRow() const { return m_walker.getRow(); }
	int64_t getCol() const { return m_walker.getCol(); }

protected:
	GridLineWalker m_walker;
	underlVectorPtr m_gridItr;
};

template <typename I>
bool operator==(const GridLineItrBase<I>& lhs, const GridLineItrBase<I>& rhs)
{
	return lhs.m_walker == rhs.m_walker;
}


template<typename G>
struct std::iterator_traits<typename GridLineItr<G>>
{
	using value_type = typename G::value_type;
	using pointer = std::conditional<std::is_const<G>::value, typename G::const_pointer, typename G::pointer>::type;
	using reference = std::conditional<std::is_const<G>::value, typename G::const_reference, typename G::reference>::type;
	using difference_type = int64_t;
	using iterator_category = std::bidirectional_iterator_tag;
};

template <typename G>
class GridLineItr : public GridLineItrBase<GridLineItr<G>>
{
public:
	
	using pointer = typename std::iterator_traits<GridLineItr>::pointer;

	GridLineItr(int64_t row, int64_t col, dir direction, G& grid) :
		GridLineItrBase<GridLineItr<G>>{ row, col, direction, static_cast<pointer>(&(*--grid.m_data.end()))}
	{
	}
	

	GridLineItr(void) :
		GridLineItrBase<GridLineItr<G>>{}
	{
	}
	
};

static_assert(std::bidirectional_iterator<GridLineItr<Grid<char>>>);
static_assert(std::bidirectional_iterator<ConstGridLineItr<Grid<char>>>);


template <typename G>
class GridLineBase
{
	
public:
	GridLineBase(int64_t startRow, int64_t startCol, dir direction, G& grid) :
		m_srow{ startRow },
		m_scol{ startCol },
		m_dir{ direction },
		m_grid{ grid }
	{
	}
protected:

	std::pair<int64_t, int64_t> getEnd() const
	{
		int64_t endCol = m_scol;
		int64_t endRow = m_srow;
		int64_t numSteps = 0;

		switch (m_dir)
		{
		case dir::E:
			endCol = m_grid.nCols();
			break;
		case dir::W:
			endCol = -1;
			break;
		case dir::N:
			endRow = m_grid.nRows();
			break;
		case dir::S:
			endRow = -1;
			break;
		case dir::NE:
			numSteps = std::min(m_grid.nRows() - m_srow, m_grid.nCols() - m_scol);
			endCol = m_scol + numSteps;
			endRow = m_srow + numSteps;
			break;
		case dir::SE:
			numSteps = std::min(m_srow + 1, m_grid.nCols() - m_scol);
			endCol = m_scol + numSteps;
			endRow = m_srow - numSteps;
			break;
		case dir::SW:
			numSteps = std::min(m_srow + 1, m_scol + 1);
			endCol = m_scol - numSteps;
			endRow = m_srow - numSteps;
			break;
		case dir::NW:
			numSteps = std::min(m_grid.nRows() - m_srow, m_scol + 1);
			endCol = m_scol - numSteps;
			endRow = m_srow + numSteps;
			break;
		default:
			break;
		}
		return { endRow, endCol };
	}

	int64_t m_srow;
	int64_t m_scol;
	dir m_dir;
	G& m_grid;
};


template <typename G>
class GridLine : GridLineBase<G>
{
public:


	GridLine(int64_t startRow, int64_t startCol, dir direction, G& grid) :
		GridLineBase<G>{ startRow , startCol, direction, grid }
	{
	};

	GridLineItr<G> begin()
	{
		return GridLineItr<G>(GridLineBase<G>::m_srow, GridLineBase<G>::m_scol, GridLineBase<G>::m_dir, GridLineBase<G>::m_grid);
	}


	GridLineItr<G> end()
	{
		auto [endRow, endCol] = GridLineBase<G>::getEnd();
		return GridLineItr<G>(endRow, endCol, GridLineBase<G>::m_dir, GridLineBase<G>::m_grid);
	}

};