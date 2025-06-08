#pragma once

#include "../LogManager/LogManager.h"


#include <vector>
#include <memory>
#include <bitset>
#include <set>
#include <unordered_map>
#include <typeindex>



const unsigned int MAX_COMPONENTS = 32;
//Signature
// ////////////////////////////////////////////////////////////////////////////
//We use a bitset (1s and 0s) to keep track of which components an entity has,
//also helps to keep track of which entities a system is intrested in.
// ////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

//defining pool
class IPool
{
	public:
		virtual ~IPool() {}

};
template <typename T>
class Pool : public IPool
{
private:
	std::vector<T> data;
public:
	Pool (int size = 100)
	{
		data.resize(size); //capacity of data
	}
	~Pool() = default;

	bool isEmpty() const 
	{
		return data.empty();
	}

	int GetSize() const
	{
		return data.size();
	}

	void Resize(int n) 
	{
		data.resize(n);
	}

	void Clear()
	{
		data.clear();
	}

	void Add(T entry)
	{
		data.push_back(entry);
	}

	void Set(int index, T object)
	{
		data[index] = object;
	}
	
	T& Get(int index)
	{
		return data[index];
	}

	T& operator [](unsigned int index)
	{
		return data[index];
	}
};

struct IComponent 
{
protected:
	static int nextId;
};
//used to assign unique id to a component type
template <typename T>
class Component : public IComponent
{
public:
	//static means it will be same for every instances of the class
	//it also means that for TransformComponent,SpriteComponent,BoxColliderComponent each will have different id as we are incrementing it
	static int GetId()
	{
		static auto id = nextId++;
		return id;
	}
};


class Entity
{
private:
	int id;
public:
	Entity(int id) : id(id) {}
	Entity(const Entity& enitity) = default;
	int GetId() const;

	bool operator ==(const Entity& other) const 
	{
		return id == other.id;
	}

	bool operator !=(const Entity& other) const
	{
		return id != other.id;
	}
	bool operator >(const Entity& other) const
	{
		return id > other.id;
	}
	bool operator <(const Entity& other) const
	{
		return id < other.id;
	}


	template<typename TComponent, typename ...TArgs> void AddComponent(TArgs && ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent GetComponent(Entity entity) const;
	//hold a pointer to the entity's owner registry
	class Registry* registry;
};

//system processes entities that contain a specific signature
class System 
{
private:
	//this signature tells about which component is present to get the system acting on that entity
	// for example if this system is parent of a movement system then it needs to act on entities with movementcomponent added to them
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	//modern c++ syntax to let complier know it has constructor and destructor registered
	System() = default;
	virtual ~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityToSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	//Define the component type T that entities must have
	template <typename TComponent> void RequireCompnent();
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The registry ,amages the creation and destruction of entities, add systems and components.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Registry 
{

private:
	int numEntities = 0;

	std::vector<std::shared_ptr<IPool>> componentPools;  //maps entity Id as index and pool as list of component it has                      

	std::vector<Signature> entityComponentSignatures; // keeps track of entitycomponents for a given entity (vector index = Enitity ID

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;


	std::set<Entity> entitiesToBeAdded; // entities awaiting creation in the next registry Update()
	std::set<Entity> entitiesToBeKilled; // entities awaiting destruction in the next registry update()

public:
	Registry();
	~Registry();
	void Update();
	// TODO:
	//management of entities
	Entity CreateEntity();
	//template <typename T> void RemoveComponent(Entity entity);
	//template <typename T> bool HasComponent(Entity entity) const;
	//template <typename T> T& GetComponent(Entity entity) const;
	void KillEntity(Entity entity);
	// GetComponent(Entity entity)
	

	// Component Management
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity) const;
	template <typename TComponent> TComponent GetComponent(Entity entity) const;
	
	// System Management
	template <typename TSystem, typename ...TArgs> void AddSystems(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> const TSystem& GetSystem() const;

	// checks the component signature of an entity and adds the entity to the systems
	// taht are interested in it
	void AddEntityToSystem(Entity entity);

};

template<typename TSystem, typename ...TArgs>
void Registry::AddSystems(TArgs && ...args)
{
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem()
{
	auto system = systems.find(std::find(std::type_index(typeid(TSystem))));
	systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const
{
	auto system = systems.find(std::find(std::type_index(typeid(TSystem))));
	return (system != systems.end());
}

template<typename TSystem>
const TSystem& Registry::GetSystem() const
{
	auto system = systems.find(std::find(std::type_index(typeid(TSystem))));
	return *std::static_pointer_cast<TSystem>(system->second);
}

//always put definition of template in header as compiler needs the defination everytime compile
template <typename TComponent> void System::RequireCompnent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}


template<typename TComponent, typename ...TArgs> void Registry::AddComponent(Entity entity, TArgs && ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	if (componentId >= componentPools.size())
		componentPools.resize(componentId+1, nullptr);
	
	if (!componentPools[componentId])
	{
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool =	std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	if (entityId >= componentPool->GetSize())
	{
		componentPool->Resize(numEntities);
	}
	TComponent newComponent(std::forward<TArgs>(args)...);
	componentPool->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);

	LOG(LM_Info, "Component Added Successfully with id : ", componentId, " was added to entity id: ", entityId);
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	entityComponentSignatures[entityId].set(componentId,false);
	LOG(LM_Info, "Component Removed Successfully with id : ", componentId, " was added to entity id: ", entityId);
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	return entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
inline TComponent Registry::GetComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentpool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentpool->Get(entityId);
}

template<typename TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs && ...args)
{
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
inline void Entity::RemoveComponent()
{
	registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
inline bool Entity::HasComponent() const
{
	return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
inline TComponent Entity::GetComponent(Entity entity) const
{
	return registry->GetComponent<TComponent>(*this);
}
