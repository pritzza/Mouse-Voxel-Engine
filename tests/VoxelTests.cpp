#include "VoxelTests.h"

#include <iostream>

#include "../src/voxel/Voxel.h"
#include "../src/voxel/VoxelGrid.h"

bool testFullySolid1x1x1()
{
    bool success{ true };

    VoxelGrid grid(glm::ivec3(1, 1, 1));
    grid.setVoxel({ 0, 0, 0 }, Voxel(Voxel::ID::Filled, VoxelGraphicsData()));

    // Check surrounding bits for each voxel
    if (Math::countSetBits(grid.getSurroundingData().at(0)) != 1)
    {
        std::cout << "error! Voxel at (0, 0, 0) should have 1 neighbor, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(0)) << " neighbors." << std::endl;
        success = false;
    }

    if (!success)
        grid.printSurroundingDebugInfo(false);

    return true;
}

bool testFullySolid3x1x1()
{
    bool success{ true };

    VoxelGrid grid(glm::ivec3(3, 1, 1));
    for (int x = 0; x < 3; ++x) {
        grid.setVoxel({ x, 0, 0 }, Voxel(Voxel::ID::Filled, VoxelGraphicsData()));
    }

    // Check surrounding bits for each voxel
    if (Math::countSetBits(grid.getSurroundingData().at(0)) != 2)
    {
        std::cout << "error! Voxel at (0, 0, 0) should have 2 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(0)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(1)) != 3)
    {
        std::cout << "error! Voxel at (1, 0, 0) should have 3 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(1)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(2)) != 2)
    {
        std::cout << "error! Voxel at (2, 0, 0) should have 2 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(2)) << " neighbors." << std::endl;
        success = false;
    }

    if (!success)
        grid.printSurroundingDebugInfo(false);

    return success;
}

bool testFullySolid1x3x1()
{
    bool success{ true };

    VoxelGrid grid(glm::ivec3(1, 3, 1));
    for (int x = 0; x < 3; ++x) {
        grid.setVoxel({ 0, x, 0 }, Voxel(Voxel::ID::Filled, VoxelGraphicsData()));
    }

    // Check surrounding bits for each voxel
    if (Math::countSetBits(grid.getSurroundingData().at(0)) != 2)
    {
        std::cout << "error! Voxel at (0, 0, 0) should have 2 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(0)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(1)) != 3)
    {
        std::cout << "error! Voxel at (0, 1, 0) should have 3 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(1)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(2)) != 2)
    {
        std::cout << "error! Voxel at (0, 2, 0) should have 2 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(2)) << " neighbors." << std::endl;
        success = false;
    }

    if (!success)
        grid.printSurroundingDebugInfo(false);

    return success;
}

bool testFullySolid1x1x3()
{
    bool success{ true };

    VoxelGrid grid(glm::ivec3(1, 1, 3));
    for (int x = 0; x < 3; ++x) {
        grid.setVoxel({ 0, 0, x }, Voxel(Voxel::ID::Filled, VoxelGraphicsData()));
    }

    // Check surrounding bits for each voxel
    if (Math::countSetBits(grid.getSurroundingData().at(0)) != 2)
    {
        std::cout << "error! Voxel at (0, 0, 0) should have 2 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(0)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(1)) != 3)
    {
        std::cout << "error! Voxel at (0, 0, 1) should have 3 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(1)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(2)) != 2)
    {
        std::cout << "error! Voxel at (0, 0, 2) should have 2 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(2)) << " neighbors." << std::endl;
        success = false;
    }

    if (!success)
        grid.printSurroundingDebugInfo(false);

    return success;
}

bool testFullySolid3x3x3()
{
    bool success{ true };

    const glm::ivec3 dim{ 3, 3, 3 };
    VoxelGrid grid(dim);
    for (int x = 0; x < 27; ++x) {
        grid.setVoxel(Math::toCoord(x, dim), Voxel(Voxel::ID::Filled, VoxelGraphicsData()));
    }

    // Check the number of neighbors for specific voxels
    if (Math::countSetBits(grid.getSurroundingData().at(0)) != 8)
    {
        std::cout << "error! Voxel at (0, 0, 0) should have 8 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(0)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(1)) != 12)
    {
        std::cout << "error! Voxel at (1, 0, 0) should have 12 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(1)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(26)) != 8)
    {
        std::cout << "error! Voxel at (2, 2, 2) should have 8 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(26)) << " neighbors." << std::endl;
        success = false;
    }
    if (Math::countSetBits(grid.getSurroundingData().at(27 / 2)) != 27)
    {
        std::cout << "error! Voxel at (1, 1, 1) should have 27 neighbors, but it has "
            << Math::countSetBits(grid.getSurroundingData().at(27 / 2)) << " neighbors." << std::endl;
        success = false;
    }

    if (!success)
        grid.printSurroundingDebugInfo(false);

    return success;
}

bool testSparse3x3x3()
{
    bool success{ true };

    const glm::ivec3 dim{ 3, 3, 3 };
    VoxelGrid grid(dim);

    std::vector<glm::ivec3> cornerCoords = {
        { 0, 0, 0 },
        { 2, 0, 0 },
        { 0, 2, 0 },
        { 0, 0, 2 },
        { 0, 2, 2 },
        { 2, 2, 0 },
        { 2, 0, 2 },
        { 2, 2, 2 },
    };

    for (const glm::ivec3& cornerCoord : cornerCoords)
        grid.setVoxel(cornerCoord, Voxel(Voxel::ID::Filled, VoxelGraphicsData()));

    // Check surrounding bits for each voxel
    for (const glm::ivec3& cornerCoord : cornerCoords)
    {
        const int index = Math::toIndex(cornerCoord, dim);
        const int setBits = Math::countSetBits(grid.getSurroundingData().at(index));
        if (setBits != 1)
        {
            std::cout << "error! Voxel at index " << index << " has " << setBits
                << " neighbors, but it should have 1." << std::endl;
            success = false;
        }
    }

    const int setBits = Math::countSetBits(grid.getSurroundingData().at(27 / 2));
    if (setBits != 8 + 1)
    {
        std::cout << "error! Voxel at index " << 27 / 2 << " (center voxel) has " << setBits
            << " neighbors, but it should have 8 + 1 neighbors." << std::endl;
        success = false;
    }

    if (!success)
        grid.printSurroundingDebugInfo(false);

    return success;
}
