#pragma once

#include <array>
#include <vector>

#include <glm/vec3.hpp>

#include "../VertexAttributes.h"

class VoxelSpace;

struct VoxelMesh
{
	std::vector<Position> vertices;
	std::vector<Normal> normals;
	std::vector<Color> colors;
};

static constexpr int VERTICES_PER_QUAD{ 6 };

// cool but dumb idea, make veriadic template version
// takes variable number of vertex attributes
// not really necessary but thought the idea was cool lol
struct Quad
{
	std::array<Position, VERTICES_PER_QUAD> vertices;

	// todo: just realized there is only one normal value per quad
	std::array<Normal, VERTICES_PER_QUAD> normals;
};

class VoxelMeshGenerator
{
public:
	VoxelMesh generateVoxelMesh(const VoxelSpace& vs) const;

private:
	// takes a face and inserts its vertices to given vertex attribute vectors
	void insertFace(
		VoxelMesh& voxelMesh,
		const Quad& face,
		const Color& voxelColor
	) const;

	// takes mesh of single voxel and adds it to entire voxel chunk mesh
	void addVoxelToChunk(
		VoxelMesh& chunkMesh,
		const VoxelMesh& voxelMesh
	) const;

	bool isVoxelSolid(const VoxelSpace& vs, const glm::ivec3& pos) const;


	void offsetVertices(
		std::vector<Position>& vertices, 
		const glm::ivec3& offset
	) const;

	void offsetVertices(
		std::array<Position, VERTICES_PER_QUAD>& vertices, 
		const glm::ivec3& offset
	) const;

	const std::vector<Position>& getCubeVertices() const;
	const std::vector<TextureCoordinate2D>& getCubeTexCoords() const;
	const std::vector<Normal>& getCubeNormals() const;

	// generates face of a block at position inside of chunk
	Quad generateTopFace	(const glm::ivec3& relativePosition) const;
	Quad generateBottomFace	(const glm::ivec3& relativePosition) const;
	Quad generateLeftFace	(const glm::ivec3& relativePosition) const;
	Quad generateRightFace	(const glm::ivec3& relativePosition) const;
	Quad generateFrontFace	(const glm::ivec3& relativePosition) const;
	Quad generateBackFace	(const glm::ivec3& relativePosition) const;
};

