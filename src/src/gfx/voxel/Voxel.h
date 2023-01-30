#pragma once

struct Voxel
{
	enum class ID
	{
		Empty,
		Solid,
		ERROR
	};

	ID id;
	float r{ 1.f };
	float g{ 1.f };
	float b{ 1.f };
};

// todo: make is to each voxel can have its own material, is that possible even?
/*
#include "../Materials.h"

struct Voxel
{
	bool exists{ false };
	Materials::ID materialID{ Materials::ID::VOID };
};

namespace Voxels
{
	static constexpr Voxel EMPTY{
		false,
		Materials::ID::VOID
	};

	static constexpr Voxel SOLID{
		true,
		Materials::ID::PolishedBronze
	};

	static constexpr Voxel ERROR{
		true,
		Materials::ID::VOID
	};

	enum class ID
	{
		Empty,
		Solid,
		ERROR
	};

	EnumArray<ID, Voxel> VOXELS =
	{
		{ ID::Empty,		      Voxels::EMPTY },
		{ ID::Solid,		      Voxels::SOLID },
		{ ID::ERROR,		      Voxels::ERROR },

	};
};
*/