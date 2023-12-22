#include "VoxelGrid.h"

VoxelGrid::VoxelGrid(const glm::ivec3 dim)
	:
	voxels{			dim, DEFAULT_VOXEL_ID		},
	voxelGraphics{	dim, DEFAULT_VOXEL_GRAPHIC	},
	voxelFaces{		dim, DEFAULT_VOXEL_FACES	},
	voxelPositions{ dim, DEFAULT_VOXEL_POSITION }
{}
