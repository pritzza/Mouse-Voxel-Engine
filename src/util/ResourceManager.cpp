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
	//const auto result{ resources.emplace({ id, data }) };
	//const bool insertionHappened{ resources.emplace({ id, data }).second };
	//
	//if (insertionHappened)
	//	return;
	
	//auto resourceIterator{ result.first };
	//resourceIterator->second = data;
	resources[id] = data;
}

template<typename Resource>
void ResourceManager<Resource>::update()
{
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