#include "VoxelMeshGenerator.h"

#include "VoxelSpace.h"

VoxelMesh VoxelMeshGenerator::generateVoxelMesh(const VoxelSpace& vs) const
{
	VoxelMesh chunkMesh;

	const glm::ivec3& dim{ vs.getDimensions() };

	for (int x = 0; x < dim.x; ++x)
		for (int y = 0; y < dim.y; ++y)
			for (int z = 0; z < dim.z; ++z)
			{
                const glm::ivec3 currentVoxelPos{ x, y, z };

                const Voxel v{ vs.getVoxel(currentVoxelPos) };

                // TODO NOW: create more efficient algorithm; render only visible faces
				if (v.id == Voxel::ID::Solid)
				{
                    VoxelMesh voxelMesh;

                    const Color voxelColor{ v.r, v.g, v.b };

                    // for faces on border of voxel space boundary
                    if (x == 0)
                        insertFace(voxelMesh,generateLeftFace(currentVoxelPos),voxelColor);
                    else if (x == dim.x - 1)
                        insertFace(voxelMesh,generateRightFace(currentVoxelPos),voxelColor);

                    if (y == 0)
                        insertFace(voxelMesh,generateBottomFace(currentVoxelPos),voxelColor);
                    else if (y == dim.x - 1)
                        insertFace(voxelMesh,generateTopFace(currentVoxelPos),voxelColor);

                    if (z == 0)
                        insertFace(voxelMesh,generateFrontFace(currentVoxelPos),voxelColor);
                    else if (z == dim.x - 1)
                        insertFace(voxelMesh,generateBackFace(currentVoxelPos),voxelColor);

                    /*
                    // generates faces adjacent to non solid neighbors
                    const glm::ivec3 rightVoxelPos{ x+1, y, z };
                    if (isVoxelSolid(vs, rightVoxelPos))
                        insertFace(voxelMesh, generateRightFace(currentVoxelPos), voxelColor);

                    const glm::ivec3 topVoxelPos{ x, y+1, z };
                    if (isVoxelSolid(vs, topVoxelPos))
                        insertFace(voxelMesh, generateTopFace(currentVoxelPos), voxelColor);

                    const glm::ivec3 backVoxelPos{ x, y, z+1 };
                    if (isVoxelSolid(vs, backVoxelPos))
                        insertFace(voxelMesh, generateBackFace(currentVoxelPos), voxelColor);

                    const glm::ivec3 leftVoxelPos{ x-1, y, z };
                    if (isVoxelSolid(vs, leftVoxelPos))
                        insertFace(voxelMesh, generateLeftFace(currentVoxelPos), voxelColor);

                    const glm::ivec3 bottomVoxelPos{ x, y-1, z };
                    if (isVoxelSolid(vs, bottomVoxelPos))
                        insertFace(voxelMesh, generateBottomFace(currentVoxelPos), voxelColor);

                    const glm::ivec3 frontVoxelPos{ x, y , z-1 };
                    if (isVoxelSolid(vs, frontVoxelPos))
                        insertFace(voxelMesh, generateFrontFace(currentVoxelPos), voxelColor);
                    */
                    const glm::ivec3 rightVoxelPos{ x+1, y, z };
                    if (x + 1 < dim.x)
                    {
                        const Voxel rightV{ vs.getVoxel(rightVoxelPos) };

                        if (rightV.id == Voxel::ID::Empty)
                            insertFace(voxelMesh, generateRightFace(currentVoxelPos), voxelColor);
                    }

                    const glm::ivec3 topVoxelPos{ x, y+1, z };
                    if (y + 1 < dim.y)
                    {
                        const Voxel topV{ vs.getVoxel(topVoxelPos) };

                        if (topV.id == Voxel::ID::Empty)
                            insertFace(voxelMesh, generateTopFace(currentVoxelPos), voxelColor);
                    }

                    const glm::ivec3 backVoxelPos{ x, y, z+1 };
                    if (z + 1 < dim.z)
                    {
                        const Voxel backV{ vs.getVoxel(backVoxelPos) };

                        if (backV.id == Voxel::ID::Empty)
                            insertFace(voxelMesh, generateBackFace(currentVoxelPos), voxelColor);
                    }

                    const glm::ivec3 leftVoxelPos{ x-1, y, z };
                    if (x - 1 >= 0)
                    {
                        const Voxel leftV{ vs.getVoxel(leftVoxelPos) };

                        if (leftV.id == Voxel::ID::Empty)
                            insertFace(voxelMesh, generateLeftFace(currentVoxelPos), voxelColor);
                    }

                    const glm::ivec3 bottomVoxelPos{ x, y-1, z };
                    if (y - 1 >= 0)
                    {
                        const Voxel bottomV{ vs.getVoxel(bottomVoxelPos) };

                        if (bottomV.id == Voxel::ID::Empty)
                            insertFace(voxelMesh, generateBottomFace(currentVoxelPos), voxelColor);
                    }

                    const glm::ivec3 frontVoxelPos{ x, y , z-1 };
                    if (z - 1 >= 0)
                    {
                        const Voxel frontV{ vs.getVoxel(frontVoxelPos) };

                        if (frontV.id == Voxel::ID::Empty)
                            insertFace(voxelMesh, generateFrontFace(currentVoxelPos), voxelColor);
                    }

                    // actually add vertex data for current voxel to whole mesh
                    addVoxelToChunk(
                        chunkMesh,
                        voxelMesh
                    );
				}
			}

    return chunkMesh;
}

void VoxelMeshGenerator::insertFace(
    VoxelMesh& voxelMesh,
    const Quad& face, 
    const Color& voxelColor
) const
{
    voxelMesh.vertices.insert(
        voxelMesh.vertices.end(),
        face.vertices.begin(),
        face.vertices.end()
    );
    
    voxelMesh.normals.insert(
        voxelMesh.normals.end(),
        face.normals.begin(),
        face.normals.end()
    );

    static constexpr int VERTICES_PER_FACE{ 6 };

    const size_t oldColorsSize{ voxelMesh.colors.size() };

    voxelMesh.colors.resize(oldColorsSize + VERTICES_PER_FACE);

    for (int i = 0; i < VERTICES_PER_FACE; ++i)
        voxelMesh.colors[i + oldColorsSize] = voxelColor;
}

void VoxelMeshGenerator::addVoxelToChunk(
    VoxelMesh& chunkMesh, 
    const VoxelMesh& voxelMesh
) const
{
    chunkMesh.vertices.insert(
        chunkMesh.vertices.end(),
        voxelMesh.vertices.begin(),
        voxelMesh.vertices.end()
    );

    chunkMesh.normals.insert(
        chunkMesh.normals.end(),
        voxelMesh.normals.begin(),
        voxelMesh.normals.end()
    );

    chunkMesh.colors.insert(
        chunkMesh.colors.end(),
        voxelMesh.colors.begin(),
        voxelMesh.colors.end()
    );
}

bool VoxelMeshGenerator::isVoxelSolid(const VoxelSpace& vs, const glm::ivec3& pos) const
{
    const glm::vec3& vsDim{ vs.getDimensions() };

    const bool isInBounds{
        (0 <= pos.x && pos.x < vsDim.x) &&
        (0 <= pos.y && pos.y < vsDim.y) &&
        (0 <= pos.z && pos.z < vsDim.z)
    };

    if (isInBounds)
    {
        const Voxel& v{ vs.getVoxel(pos) };

        return v.id == Voxel::ID::Solid;
    }

    return false;
}

void VoxelMeshGenerator::offsetVertices(
	std::vector<Position>& vertices, 
	const glm::ivec3& offset
) const
{
	for (Position& position : vertices)
	{
		position.x += offset.x;
		position.y += offset.y;
		position.z += offset.z;
	}
}

void VoxelMeshGenerator::offsetVertices(
    std::array<Position, VERTICES_PER_QUAD>& vertices, 
    const glm::ivec3& offset
) const
{
    for (Position& position : vertices)
    {
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
    }
}

typedef Quad VoxelMeshQuad;

VoxelMeshQuad VoxelMeshGenerator::generateTopFace(
    const glm::ivec3& relativePosition
) const
{
    static constexpr Quad topFace =
    {
        std::array<Position, VERTICES_PER_QUAD>{
            // tri 1
            Position{ 0,1,0 },
            Position{ 1,1,0 },
            Position{ 0,1,1 },
            // tri 2
            Position{ 1,1,1 },
            Position{ 1,1,0 },
            Position{ 0,1,1 },
        },

        std::array<Normal, VERTICES_PER_QUAD>{
            // tri 1
            Normal{ 0,1,0 },
            Normal{ 0,1,0 },
            Normal{ 0,1,0 },
            // tri 2
            Normal{ 0,1,0 },
            Normal{ 0,1,0 },
            Normal{ 0,1,0 },
        }
    };

    Quad quad{ topFace };
    offsetVertices(quad.vertices, relativePosition);

    return quad;
}

VoxelMeshQuad VoxelMeshGenerator::generateBottomFace(
    const glm::ivec3& relativePosition
) const
{
    static constexpr Quad bottomFace =
    {
        std::array<Position, VERTICES_PER_QUAD>{
            // tri 1
            Position{ 0,0,0 },
            Position{ 1,0,0 },
            Position{ 0,0,1 },
            // tri 2
            Position{ 1,0,1 },
            Position{ 1,0,0 },
            Position{ 0,0,1 },
        },

        std::array<Normal, VERTICES_PER_QUAD>{
            // tri 1
            Normal{ 0,-1,0 },
            Normal{ 0,-1,0 },
            Normal{ 0,-1,0 },
            // tri 2
            Normal{ 0,-1,0 },
            Normal{ 0,-1,0 },
            Normal{ 0,-1,0 },
        }
    };

    Quad quad{ bottomFace };
    offsetVertices(quad.vertices, relativePosition);

    return quad;
}

VoxelMeshQuad VoxelMeshGenerator::generateLeftFace(
    const glm::ivec3& relativePosition
) const
{
    static constexpr Quad leftFace =
    {
        std::array<Position, VERTICES_PER_QUAD>{
            // tri 1
            Position{ 0,0,0 },
            Position{ 0,0,1 },
            Position{ 0,1,0 },
            // tri 2 
            Position{ 0,1,1 },
            Position{ 0,0,1 },
            Position{ 0,1,0 },
        },

        std::array<Normal, VERTICES_PER_QUAD>{
            // tri 1
            Normal{ -1,0,0 },
            Normal{ -1,0,0 },
            Normal{ -1,0,0 },
            // tri 2
            Normal{ -1,0,0 },
            Normal{ -1,0,0 },
            Normal{ -1,0,0 },
        }
    };


    Quad quad{ leftFace };
    offsetVertices(quad.vertices, relativePosition);

    return quad;
}

VoxelMeshQuad VoxelMeshGenerator::generateRightFace(
    const glm::ivec3& relativePosition
) const
{
    static constexpr Quad rightFace =
    {
        std::array<Position, VERTICES_PER_QUAD>{
            // tri 1
            Position{ 1,0,0 },
            Position{ 1,0,1 },
            Position{ 1,1,0 },
            // tri 2 
            Position{ 1,1,1 },
            Position{ 1,0,1 },
            Position{ 1,1,0 },
        },

        std::array<Normal, VERTICES_PER_QUAD>{
            // tri 1
            Normal{ 1,0,0 },
            Normal{ 1,0,0 },
            Normal{ 1,0,0 },
            // tri 21 0
            Normal{ 1,0,0 },
            Normal{ 1,0,0 },
            Normal{ 1,0,0 },
        }
    };

    Quad quad{ rightFace };
    offsetVertices(quad.vertices, relativePosition);

    return quad;
}

VoxelMeshQuad VoxelMeshGenerator::generateFrontFace(
    const glm::ivec3& relativePosition
) const
{
    static constexpr Quad frontFace =
    {
        std::array<Position, VERTICES_PER_QUAD>{
            // tri 1
            Position{ 0,0,0 },
            Position{ 1,0,0 },
            Position{ 0,1,0 },
            // tri 2
            Position{ 1,1,0 },
            Position{ 1,0,0 },
            Position{ 0,1,0 },
        },

        std::array<Normal, VERTICES_PER_QUAD>{
            // tri 1
            Normal{ 0,0,-1 },
            Normal{ 0,0,-1 },
            Normal{ 0,0,-1 },
            // tri 2 
            Normal{ 0,0,-1 },
            Normal{ 0,0,-1 },
            Normal{ 0,0,-1 },
        }
    };

    Quad quad{ frontFace };
    offsetVertices(quad.vertices, relativePosition);

    return quad;
}

VoxelMeshQuad VoxelMeshGenerator::generateBackFace(
    const glm::ivec3& relativePosition
) const
{
    static constexpr Quad backFace =
    {
        std::array<Position, VERTICES_PER_QUAD>{
            // tri 1
            Position{ 0,0,1 },
            Position{ 1,0,1 },
            Position{ 0,1,1 },
            // tri 2
            Position{ 1,1,1 },
            Position{ 1,0,1 },
            Position{ 0,1,1 },
        },

        std::array<Normal, VERTICES_PER_QUAD>{
            // tri 1
            Normal{ 0,0,1 },
            Normal{ 0,0,1 },
            Normal{ 0,0,1 },
            // tri 2
            Normal{ 0,0,1 },
            Normal{ 0,0,1 },
            Normal{ 0,0,1 },
        }
    };

    Quad quad{ backFace };
    offsetVertices(quad.vertices, relativePosition);

    return quad;
}

const std::vector<Position>& VoxelMeshGenerator::getCubeVertices() const
{
    // cube
    static const std::vector<Position> cubePosVertices = {

        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },

        { -0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },

        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },

        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },

        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f, -0.5f },

        { -0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f },
    };

    return cubePosVertices;
}

const std::vector<TextureCoordinate2D>& VoxelMeshGenerator::getCubeTexCoords() const
{
    static const std::vector<TextureCoordinate2D> cubeTexCoords = {

    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },

    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },

    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },

    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f }
    };

    return cubeTexCoords;
}

const std::vector<Normal>& VoxelMeshGenerator::getCubeNormals() const
{
    static const std::vector<Normal> cubeNormals = {
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },

   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },

   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },

   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },

   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },

   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
    };

    return cubeNormals;
}