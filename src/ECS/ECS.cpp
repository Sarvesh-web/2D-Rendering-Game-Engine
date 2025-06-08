#include "ECS.h"
//TODO: Implement all the functions from ECS.

int IComponent::nextId = 0;


int Entity::GetId() const 
{
	return id;
}


//TODO
void System::AddEntityToSystem(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntityToSystem(Entity entity)
{
	//remove it by verifying Id then take index to remove it
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) 
		{
			return entity == other;
		}),entities.end());
}
//void RemoveEntityToSystem(Entity entity);
std::vector<Entity> System::GetSystemEntities() const
{
	return entities;
}
const Signature& System::GetComponentSignature() const 
{
	return componentSignature;
}

Entity Registry::CreateEntity()
{
	int entityId = numEntities++;

	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);

	//make sure the entityComponentSignatures vector can accomodate the new entity
	if (entityId >= entityComponentSignatures.size())
	{
		entityComponentSignatures.resize(entityId+1);
	}


	LOG(LM_Warn, "Entity has been Added with Id : ", entityId);
	return entity;
}

void Registry::AddEntityToSystem(Entity entity)
{
	//check if signature matches then only add it
	for (auto& [index, system] : systems)
	{
		const auto& systemcomponentsignature = system->GetComponentSignature();
		const auto& entityComponentsSignature = entityComponentSignatures[entity.GetId()];
		//if the entityComponentsSignatures has the required component present (not exactly same but pre-requisites then add it to that system)
		bool bSystemIsIntrested = (systemcomponentsignature & entityComponentsSignature) == systemcomponentsignature;
		if(bSystemIsIntrested)
		{
			system->AddEntityToSystem(entity);
		}
	}
}

void Registry::KillEntity(Entity entity)
{
}


Registry::Registry()
{
	LOG(LM_Info, "Registry Constructor Called!");
}

Registry::~Registry()
{
	LOG(LM_Info, "Registry Destructor Called!");
}

void Registry::Update()
{
	//add the entities that are waiting to be created to the active systems
	for (auto entity : entitiesToBeAdded)
	{
		AddEntityToSystem(entity);
	}
	//now clear the array
	entitiesToBeAdded.clear();
}

