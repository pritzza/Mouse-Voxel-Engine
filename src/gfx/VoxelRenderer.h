#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <glm/mat4x4.hpp>

#include "../VoxelModel.h"

class VoxelObject;
class VoxelShader;

class VoxelRenderer {
public:
	// Eenders voxel model with voxel shader at function call
	void render(const VoxelObject& object, const VoxelShader& shader);
	
	// Enqueues voxel model to be batch rendered in groups by underlying voxel grid
	// the voxel shader used to render each batch of models is the one
	// passed for the first element of each group. Objects are rendered
	// upon call of renderInstances()
	void instance(const VoxelObject& object, const VoxelShader& shader);

	// Eenders all enqued voxel modes in batches based on underlying voxel grid
	// of each model. Calling this clears all batches too.
	void renderInstances();

private:
	// every unique voxel model mapped to all the transforms of each model instanced
	std::unordered_map<std::shared_ptr<VoxelModel>, std::vector<const glm::mat4*>> instanceTransforms;

	// every unique voxel model mapped to first VoxelShader given on instance() call
	std::unordered_map<std::shared_ptr<VoxelModel>, const VoxelShader&> instanceShaders;

private:
	void clearInstances();
};