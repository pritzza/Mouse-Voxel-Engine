#pragma once

#include <memory>
#include <string>
#include <unordered_map>

template <typename Resource>
class ResourceManager
{
public:
	std::shared_ptr<Resource> get(int id) const;
	void set(int id, const std::shared_ptr<Resource>& data);

	// gives a change to remove all unused resources
	void update();

private:
	std::unordered_map<int, std::shared_ptr<Resource>> resources;

private:
	void unload(int id);
	int resourceUseCount(int id) const;

};

#include "ResourceManager.cpp"
