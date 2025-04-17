#pragma once
#include <bitset>
#include <vector>
const unsigned int MAX_COMPONENTS = 32;

//Signature
// ////////////////////////////////////////////////////////////////////////////
//We use a bitset (1s and 0s) to keep track of which components an entity has,
//also helps to keep track of which entities a system is intrested in.
// ////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;
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
	Entity(int id) : id(id) {};
	int GetId() const;
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
	Signature& GetComponentSignature() const;

	//Define the component type T that entities must have
	template <typename T> void RequireCompnent();
};

class Registry 
{
	

};