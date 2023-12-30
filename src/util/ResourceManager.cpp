#include "ResourceManager.h"

template<typename Resource>
inline std::shared_ptr<Resource> ResourceManager<Resource>::get(int id) const
{
	if (resources.find(id) == resources.end())
		return nullptr;

	return resources.at(id);
}

template<typename Resource>
inline void ResourceManager<Resource>::set(int id, const std::shared_ptr<Resource>& data)
{
	resources[id] = data;
}

template<typename Resource>
void ResourceManager<Resource>::freeUnusedResources()
{
	for (auto it : resources)
	{
		const int id{ it.first };
		if (resourceUseCount(id) == 1)	// nothing else using resource
			unload(id);
	}
}

template<typename Resource>
inline void ResourceManager<Resource>::unload(int id)
{
	if (resources.find(id) != resources.end())
		resources.erase(id);
}

template<typename Resource>
inline int ResourceManager<Resource>::resourceUseCount(int id) const
{
	const std::shared_ptr<Resource>& res{ get(id) };
	
	if (res == nullptr)
		return 0;

	return res.use_count();
}