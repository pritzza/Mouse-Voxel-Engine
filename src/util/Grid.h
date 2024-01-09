#pragma once

#include <optional>
#include <vector>

#include <glm/vec3.hpp>

template <typename T>
class Grid 
{
public:
	Grid(const glm::ivec3& dim, const T& nullValue);
    
	const T& get(int index) const;
	const T& get(const glm::ivec3& coord) const;
	const T* const getSafe(int index) const;
	const T* const getSafe(const glm::ivec3& coord) const;

	const std::vector<const T*> get(const std::vector<glm::ivec3>& coords) const;
	const std::vector<const T*> get(const std::vector<int>& indices) const;

	// returns true if success
	bool set(const glm::ivec3 coord, const T& val);
	bool set(int index, const T& val);

	// sets all cells to val
	void setAll(const T& val);

	// sets all cells to nullVal
	void clear();
	
	// copies the elements of other cells wise into
	// this grid starting at pos.
	// returns false if fails to copy all cells of other over
	//
	// eg: if other is bigger than this or if cells from other
	//     are outside of this grid
	bool copy(const Grid<T>& other, const glm::ivec3& posOffset, bool copyNullOfOther);

	// replaces all instances of 'replacing' with 'newVal'
	void replace(const T& replacingVal, const T& newVal);

	// return true if this grid and the other grid
	// have any non-null cells at corresponding coordinates
	bool isCellOverlap(const Grid<T>& other) const;

	std::vector<glm::ivec3> getNonNullAdjCoords(const glm::ivec3& coord) const;

	// Adjacent cells of O:
	//	 X
	//	XOX
	//	 X
	// 
	// returns strictly adjacent cells
	std::vector<const T*> getAdjacent(const glm::ivec3& coord) const;
	std::vector<glm::ivec3> getAdjacentCoords(const glm::ivec3& coord) const;

	// Surrounding cells of O:
	//	XXX
	//	XOX
	//	XXX
	//
	// returns all cells around coord
	std::vector<const T*> getSurrounding(const glm::ivec3& coord) const;
	std::vector<glm::ivec3> getSurroundingCoords(const glm::ivec3& coord) const;

	// returns if the grid was ever changed since the last time
	// a modification was acknowledged.
	// Basically tells you if a new cell was ever overwritten
	// Initial State:	X X
	// Second State:	X X -> not altered	(After writing X to [1])
	// Third State:		X O -> altered		(After writing O to [1])
	// Fourth State:	X X -> altered		(After writing X to [1])
	bool wasAltered() const { return wasAlteredFlag; }

	// sets internal wasAltered flag to false
	void ammendAlterations();

	int getSize() const
	{
		return size;
	}

	const std::vector<T>& getData() const 
	{
		return cells;
	}

private:
	const glm::ivec3 dim;
	const int size;
	const T nullValue;
	std::vector<T> cells;
	bool wasAlteredFlag{ false };
};


#include "Grid.cpp"