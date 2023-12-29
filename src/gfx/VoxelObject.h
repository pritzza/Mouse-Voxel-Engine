#pragma once

#include <memory>

#include "ModelMatrix.h"

#include "../VoxelModel.h"

struct VoxelObject {
	std::shared_ptr<VoxelModel> model;
	ModelMatrix transform;
};