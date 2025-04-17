#include "ECS.h"

//TODO: Implement all the functions from ECS.
int Entity::GetId() const 
{
	return id;
}

//Implementation of the function template
template <typename T> void System::RequireCompnent()
{
	const auto componentId = Component<T>::GetId();
	componentSignature.set(componentId);
}
//TODO
//void AddEntityToSystem(Entity entity);
//void RemoveEntityToSystem(Entity entity);
//std::vector<Entity> GetSystemEntities() const;
//Signature& GetComponentSignature() const;