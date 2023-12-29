#include "ResourceManager.h"

//void ResourceManager::load(const std::string& id, const VoxelSpace& vs)
//{
//	unload(id);	// emplace/insert only work if there is no value at key
//	voxelSpaces.emplace(id, vs);
//
//	// when a voxel space is stored, generate and store its mesh
//	static VoxelMeshGenerator meshGen;
//	const VoxelMesh mesh{ meshGen.generateVoxelMesh(vs) };
//
//	voxelMeshVAOs.emplace(id, VAO{});
//
//	voxelMeshVAOs.at(id).defineAttribute(mesh.vertices);
//	voxelMeshVAOs.at(id).defineAttribute(mesh.normals);
//	voxelMeshVAOs.at(id).defineAttribute(mesh.colors);
//}
//
//void ResourceManager::unload(const std::string& id)
//{
//	if (voxelSpaces.find(id) != voxelSpaces.end())
//		voxelSpaces.erase(id);
//
//	if (voxelMeshVAOs.find(id) != voxelMeshVAOs.end())
//		voxelMeshVAOs.erase(id);
//}
//
//const VoxelSpace& ResourceManager::getVoxelSpace(const std::string& id) const
//{
//	return voxelSpaces.at(id);
//}
//
//const VAO& ResourceManager::getVoxelMeshVAO(const std::string& id) const
//{
//	return voxelMeshVAOs.at(id);
//}
