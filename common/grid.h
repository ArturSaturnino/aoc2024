#pragma once

#include <array>
#include <vector>
#include <iterator>

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

template <typename G>
class GridLine;

template <typename T>
class Grid
{
public:

	using value_type = T;
	using pointer = T*;
	using reference = T&;

	template<typename G>
	friend class GridLineItr;


	Grid(const std::vector<std::span<T>>& data) :
		m_data(data),
		m_nRows(m_data.size()),
		m_nCols(m_data.empty() ? 0 : m_data[0].size())

	{
	};

	reference get (int64_t row, int64_t col) { return m_data[m_nRows - row - 1][col]; };
	GridLine<Grid<T>> getLine(int64_t row, int64_t col, dir d)
	{
		return GridLine(row, col, d, *this);
	}

	int64_t nRows() { return m_nRows; }
	int64_t nCols() { return m_nCols; }

private:
	std::vector<std::span<T>> m_data;
	int64_t m_nRows;
	int64_t m_nCols;
};

template <typename G>
class GridLineItr
{
public:
	using value_type = typename G::value_type;
	using pointer = typename G::pointer;
	using reference = typename G::reference;
	using difference_type = int64_t;
	using iterator_category = std::forward_iterator_tag;

	using underlVectorItr = typename std::vector<std::span<value_type>>::iterator;
	
	GridLineItr(int64_t row, int64_t col, dir direction, G& grid):
		m_row{row},
		m_col{col},
		m_dir{direction},
		m_gridItr{ --grid.m_data.end() }
	{
	}

	GridLineItr(void):
		m_row{ },
		m_col{ },
		m_dir{ },
		m_gridItr{ }
	{
	}
	
	GridLineItr& operator++()
	{
		switch (m_dir)
		{
		case dir::E:
			++m_col;
			break;
		case dir::W:
			--m_col;
			break;
		case dir::N:
			++m_row;
			break;
		case dir::S:
			--m_row;
			break;
		case dir::NE:
			++m_row;
			++m_col;
			break;
		case dir::SE:
			++m_col;
			--m_row;
			break;
		case dir::SW:
			--m_row;
			--m_col;
			break;
		case dir::NW:
			++m_row;
			--m_col;
			break;
		default:
			break;
		}
		return *this;
	}

	GridLineItr operator++(int)
	{
		auto itr = *this;
		this->operator++();
		return itr;
	}
	
	reference operator*() const
	{
		return const_cast<reference>(GridLineItr::operator*());
	}

	reference operator*()
	{
		return (*(m_gridItr - m_row))[m_col];
	}

	bool operator==(const GridLineItr& rhs) const
	{
		return m_row == rhs.m_row && m_col == rhs.m_col;
	}

private:
	int64_t m_row;
	int64_t m_col;
	dir m_dir;
	underlVectorItr m_gridItr;
};

static_assert(std::forward_iterator<GridLineItr<Grid<char>>>);

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
	}

	GridLineItr<G> begin()
	{
		return GridLineItr<G>(m_srow, m_scol, m_dir, m_grid);
	}

	GridLineItr<G> end()
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
			numSteps = std::min(m_srow+1, m_grid.nCols() - m_scol);
			endCol = m_scol + numSteps;
			endRow = m_srow - numSteps;
			break;
		case dir::SW:
			numSteps = std::min(m_srow+1, m_scol+1);
			endCol = m_scol - numSteps;
			endRow = m_srow - numSteps;
			break;
		case dir::NW:
			numSteps = std::min(m_grid.nRows() - m_srow, m_scol+1);
			endCol = m_scol - numSteps;
			endRow = m_srow + numSteps;
			break;
		default:
			break;
		}

		return GridLineItr<G>(endRow, endCol, m_dir, m_grid);
	}

private:
	int64_t m_srow;
	int64_t m_scol;
	dir m_dir;
	G& m_grid;
};



