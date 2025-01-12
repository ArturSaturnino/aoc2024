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

static std::array<dir, 8> dirs
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


constexpr dir getDir(char c)
{
	switch (c)
	{
	case '^':
		return dir::N;
	case '>':
		return dir::E;
	case 'v':
		return dir::S;
	case '<':
		return dir::W;
	default:
		break;
	}

	return dir::SW;
}

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

constexpr std::pair<int64_t, int64_t> moveInDir(dir direction, std::pair<int64_t, int64_t> pos)
{

	return moveInDir(direction, pos.first, pos.second);
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
class Grid;

template <typename G>
struct GridTaits
{
	using value_type = void;
	using pointer = void*;
	using reference = void;
	using const_reference = const void;
	using const_pointer = const void*;
};




template <typename T>
class GridWalkerBase;

template <typename T>
bool operator==(const GridWalkerBase<T>& lhs, const GridWalkerBase<T>& rhs);


template<typename T>
class GridWalkerBase
{
public:

	int64_t getRow() const { return m_row; }
	int64_t getCol() const { return m_col; }

	T operator++(int)
	{
		T obj = static_cast<T&>(*this);
		static_cast<T*>(this)->operator++();
		return obj;
	}

	T operator--(int)
	{
		T obj = static_cast<T&>(*this);
		static_cast<T*>(this)->operator--();
		return obj;
	}

	friend bool operator==<T>(const GridWalkerBase& lhs, const GridWalkerBase& rhs);

protected:

	GridWalkerBase(int64_t row, int64_t col) :
		m_row{ row },
		m_col{ col }
	{
	};

	GridWalkerBase() :
		m_row{  },
		m_col{  }
	{
	};

	int64_t m_row;
	int64_t m_col;

};


template<typename T>
bool operator==(const GridWalkerBase<T>& lhs, const GridWalkerBase<T>& rhs)
{
	return lhs.m_row == rhs.m_row && lhs.m_col == rhs.m_col;
}

class GridLineWalker : public GridWalkerBase<GridLineWalker>
{
public:

	GridLineWalker(int64_t row, int64_t col, dir direction) :
		GridWalkerBase<GridLineWalker>{ row, col },
		m_dir{ direction }
	{ }

	GridLineWalker() :
		GridWalkerBase<GridLineWalker>{ },
		m_dir{  }
	{
	}

	GridLineWalker& operator++()
	{
		const auto [row, col] = moveInDir(m_dir, getRow(), getCol());
		GridWalkerBase<GridLineWalker>::m_row = row;
		GridWalkerBase<GridLineWalker>::m_col = col;
		return *this;
	}

	GridLineWalker& operator--()
	{
		const auto [row, col] = moveInDir(reverse(m_dir), m_row, m_col);
		GridWalkerBase<GridLineWalker>::m_row = row;
		GridWalkerBase<GridLineWalker>::m_col = col;
		return *this;
	}
private:
	dir m_dir;
};

class GridRowMajorWalker : public GridWalkerBase<GridRowMajorWalker>
{
public:

	GridRowMajorWalker(int64_t row, int64_t col, int64_t nCols) :
		GridWalkerBase<GridRowMajorWalker>{ row, col },
		m_nCols{ nCols }
	{ }

	GridRowMajorWalker() :
		GridWalkerBase<GridRowMajorWalker>{ },
		m_nCols{ }
	{
	}

	GridRowMajorWalker& operator++()
	{
		m_col += 1;
		if (m_col >= m_nCols)
		{
			m_col = 0;
			++m_row;
		}
		return *this;
	}

	GridRowMajorWalker& operator--()
	{
		m_col -= 1;
		if (m_col < 0)
		{
			m_col = m_nCols - 1;
			--m_row;
		}
		return *this;
	}

private:
	int64_t m_nCols;
};


template<typename G, typename W>
class GridItr;

template <typename G, typename Walker>
bool operator==(const GridItr<G, Walker>& lhs, const GridItr<G, Walker>& rhs);

template <typename G, typename Walker>
class GridItr
{
public:
	
	using value_type = typename G::value_type;
	using pointer = typename GridTaits<G>::pointer;
	using reference = typename GridTaits<G>::reference;
	using difference_type = int64_t;
	using iterator_category = std::bidirectional_iterator_tag;

	class GridItr(Walker walker, G& grid) :
		m_walker {
		walker
	},
		m_gridItr{ static_cast<pointer>(&(*(--grid.m_data.end()))) }
	{
	}

	class GridItr() :
		m_walker {
	},
		m_gridItr{ }
	{
	}

	GridItr& operator++()
	{
		++m_walker;
		return *this;
	}

	GridItr operator++(int)
	{
		const GridItr itr = *this;
		this->operator++();
		return itr;
	}

	GridItr& operator--()
	{
		--m_walker;
		return *this;
	}

	GridItr operator--(int)
	{
		const GridItr itr = *this;
		this->operator--();
		return itr;
	}

	reference operator*() const
	{
		return const_cast<reference>(GridItr::operator*());
	}

	reference operator*()
	{
		return (*(m_gridItr - m_walker.getRow()))[m_walker.getCol()];
	}


	friend bool operator==<G, Walker> (const GridItr& lhs, const GridItr& rhs);


	int64_t getRow() const { return m_walker.getRow(); }
	int64_t getCol() const { return m_walker.getCol(); }

protected:
	Walker m_walker;
	pointer m_gridItr;
};

template <typename I, typename Walker>
bool operator==(const GridItr<I, Walker>& lhs, const GridItr<I, Walker>& rhs)
{
	return lhs.m_walker == rhs.m_walker;
}

template <typename G>
using GridLineItr = GridItr<G, GridLineWalker>;

template <typename G>
using ConstGridLineItr = GridItr<const G, GridLineWalker>;

template <typename G>
using GridRowMajorItr = GridItr<G, GridRowMajorWalker>;

template <typename G>
using ConstGridRowMajorItr = GridItr<const G, GridRowMajorWalker>;


template <typename G>
class GridLine
{
public:


	GridLine(int64_t startRow, int64_t startCol, dir direction, G& grid) :
		m_srow{ startRow },
		m_scol{ startCol },
		m_dir{ direction },
		m_grid{ grid }
	{
	};


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


	GridLineItr<G> begin()
	{
		return GridLineItr<G>(GridLineWalker{ m_srow, m_scol, m_dir }, m_grid);
	}


	GridLineItr<G> end()
	{
		auto [endRow, endCol] = getEnd();
		return GridLineItr<G>(GridLineWalker{ endRow, endCol, m_dir }, m_grid);
	}

	GridLineItr<G> begin() const
	{
		return ConstGridLineItr<G>(GridLineWalker{ m_srow, m_scol, m_dir }, m_grid);
	}


	GridLineItr<G> end() const
	{
		auto [endRow, endCol] = getEnd();
		return ConstGridLineItr<G>(GridLineWalker{ endRow, endCol, m_dir }, m_grid);
	}

private:

	int64_t m_srow;
	int64_t m_scol;
	dir m_dir;
	G& m_grid;

};