#include "Grid.h"

#include "Math.h"

#include <algorithm>

template <typename T>
Grid<T>::Grid(const glm::ivec3& dim, const T& nullValue) 
	:
	dim{ dim },
	size{ dim.x * dim.y * dim.z },
	nullValue{ nullValue },
	cells( size, nullValue )
{
	assert(dim.x > 0 && dim.y > 0 && dim.z > 0);
}

template <typename T>
const T& Grid<T>::get(int index) const
{
	assert(Math::isInside(index, 0, size));

	return cells[index];
}

template <typename T>
const T& Grid<T>::get(const glm::ivec3& coord) const
{
	assert(Math::isInside(coord, dim));
	const int index{ Math::toIndex(coord, dim) };
	return get(index);
}

template <typename T>
const T* const Grid<T>::getSafe(int index) const
{
	if (Math::isInside(index, 0, size))
		return &cells[index];

	return nullptr;
}

template <typename T>
const T* const Grid<T>::getSafe(const glm::ivec3& coord) const
{
	if (Math::isInside(coord, dim))
	{
		const int index{ Math::toIndex(coord, dim) };
		return getSafe(index);
	}

	return nullptr;
}

template<typename T>
const std::vector<const T*> Grid<T>::get(const std::vector<glm::ivec3>& coords) const
{
	std::vector<const T*> selectCells;

	for (const glm::ivec3& coord : coords)
	{
		const T* cell{ getSafe(coord) };
		if (cell)
			selectCells.push_back(cell);
	}

	return selectCells;
}

template <typename T>
bool Grid<T>::set(const glm::ivec3 coord, const T& val)
{
	if (Math::isInside(coord, dim))
	{
		const int index{ Math::toIndex(coord, dim) };
		return set(index, val);
	}
	
	return false;
}

template <typename T>
bool Grid<T>::set(int index, const T& val)
{
	if (Math::isInside(index, 0, size))
	{
		const T& existingVal{ cells[index] };

		if (existingVal != val)
		{
			cells[index] = val;
			wasAlteredFlag = true;
		}
		return true;
	}

	return false;
}

template <typename T>
void Grid<T>::setAll(const T& val)
{
	std::fill(cells.begin(), cells.end(), val);
}

template <typename T>
void Grid<T>::clear() 
{
	setAll(nullValue);
}

template <typename T>
bool Grid<T>::copy(const Grid<T>& other, const glm::ivec3& posOffset, bool copyNullOfOther)
{
	// todo
	return false;
}

template <typename T>
void Grid<T>::replace(const T& replacingVal, const T& newVal) 
{
	// todo
}

template <typename T>
bool Grid<T>::isCellOverlap(const Grid<T>& other) const
{
	// todo
	return false;
}

template <typename T>
std::vector<glm::ivec3> Grid<T>::getNonNullAdjCoords(const glm::ivec3& coord) const
{
	// todo
	return {};
}

template <typename T>
std::vector<const T*> Grid<T>::getAdjacent(const glm::ivec3& coord) const
{
	return get(getAdjacentCoords(coord));
}

template <typename T>
std::vector<glm::ivec3> Grid<T>::getAdjacentCoords(const glm::ivec3& coord) const
{
	const std::vector<glm::ivec3> adjCoords =
	{
		coord + glm::ivec3{ 1, 0, 0 },
		coord + glm::ivec3{ 0, 1, 0 },
		coord + glm::ivec3{ 0, 0, 1 },
		coord + glm::ivec3{-1, 0, 0 },
		coord + glm::ivec3{ 0,-1, 0 },
		coord + glm::ivec3{ 0, 0,-1 },
	};

	return adjCoords;
}

template <typename T>
std::vector<const T*> Grid<T>::getSurrounding(const glm::ivec3& coord) const
{
	return get(getSurroundingCoords(coord));
}

template <typename T>
std::vector<glm::ivec3> Grid<T>::getSurroundingCoords(const glm::ivec3& coord) const
{
	std::vector<glm::ivec3> surroundingCoords;

	const std::array<glm::ivec3, 27> SURROUNDING_OFFSETS{ Math::getCubeSurroundingCoords() };

	for (const glm::ivec3& offset : SURROUNDING_OFFSETS)
		surroundingCoords.emplace_back(coord + offset);

	return surroundingCoords;
}

template<typename T>
void Grid<T>::ammendAlterations()
{
	wasAlteredFlag = false;
}
