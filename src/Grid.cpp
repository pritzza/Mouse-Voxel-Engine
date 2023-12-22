#include "Grid.h"

template <typename T>
Grid<T>::Grid(const glm::ivec3& dim, const T& nullValue) 
	:
	dim{ dim },
	size{ dim.x * dim.y * dim.z },
	nullValue{ nullValue },
	cells( size, nullValue )
{}

/// todo: implement everything

template <typename T>
const T& Grid<T>::get(int index) const
{
	return {};
}

template <typename T>
const T& Grid<T>::get(const glm::ivec3& coord) const
{
	return {};
}

template <typename T>
const std::optional<T>& Grid<T>::getSafe(int index) const
{
	return {};
}

template <typename T>
const std::optional<T>& Grid<T>::getSafe(const glm::ivec3& coord) const
{
	return {};
}

template <typename T>
bool Grid<T>::set(const glm::ivec3 coord, const T& val)
{
	return false;
}

template <typename T>
bool Grid<T>::set(int index, const T& val)
{
	return false;
}

template <typename T>
void Grid<T>::setAll(const T& val)
{
}

template <typename T>
void Grid<T>::clear() 
{
}

template <typename T>
bool Grid<T>::copy(const Grid<T>& other, const glm::ivec3& posOffset, bool copyNullOfOther)
{
	return false;
}

template <typename T>
void Grid<T>::replace(const T& replacingVal, const T& newVal) 
{
}

template <typename T>
bool Grid<T>::isCellOverlap(const Grid<T>& other) const
{
	return false;
}

template <typename T>
std::vector<glm::ivec3> Grid<T>::getNonNullAdjCoords(const glm::ivec3& coord) const
{
	return {};
}

template <typename T>
std::vector<T> Grid<T>::getAdjacent(const glm::ivec3& coord) const
{
	return {};
}

template <typename T>
std::vector<glm::ivec3> Grid<T>::getAdjacentCoords(const glm::ivec3& coord) const
{
	return {};
}

template <typename T>
std::vector<T> Grid<T>::getSurrounding(const glm::ivec3& coord) const
{
	return {};
}

template <typename T>
std::vector<glm::ivec3> Grid<T>::getSurroundingCoords(const glm::ivec3& coord) const
{
	return {};
}
