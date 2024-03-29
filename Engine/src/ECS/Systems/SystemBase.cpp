#include "SystemBase.hpp"
#include "SystemsManager.hpp"

int SystemBase::nextID = 0;

SystemBase::SystemBase(SystemSet set, bool permanent, int type) : EventObserver(ECS)
{
	ID = nextID++;
	SystemsManager::getInstance().addSystem(this, set);
	name = "Sys(" + std::to_string(ID) + ")";
	this->permanent = permanent;
	this->type = type;
}

SystemBase::~SystemBase()
{
	SystemsManager::getInstance().removeSystem(this);
}

void SystemBase::to_json(nlohmann::json& j) const
{
}

void SystemBase::from_json(nlohmann::json& j)
{
}

void SystemBase::earlyUpdate(float dt)
{
}

void SystemBase::lateUpdate(float dt)
{

}

bool SystemBase::addEntity(Ent* entity)
{
	auto it = std::find(entities.begin(), entities.end(), entity);
	if (it != entities.end())
		return false;

	if (!validateComponents(entity))
		return false;

	entities.push_back(entity);
	return true;
}

void SystemBase::removeEntity(int ID)
{
	auto it = entities.begin();
	while (it != entities.end())
	{
		if ((*it)->getID() == ID)
		{
			entities.erase(it);
			return;
		}
		it++;
	}
}

void SystemBase::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		Ent* entity = (Ent*)event.getPayload();
		removeEntity(entity->getID());
	}
	break;
	case Event::ComponentRemoval:
	{
		Ent* entity = (Ent*)event.getPayload();
		auto it = std::find(entities.begin(), entities.end(), entity);
		if (it == entities.end())
			break;

		if (!validateComponents(entity))
			removeEntity(entity->getID());
	}
	break;
	default:
		break;
	}
}

int SystemBase::getID()
{
	return ID;
}

std::string SystemBase::getName()
{
	return name;
}

int SystemBase::getType()
{
	return type;
}

bool SystemBase::isPermanent()
{
	return permanent;
}

bool SystemBase::validateComponents(Ent* entity)
{
	for (auto component : requiredComponents)
	{
		if (!entity->hasComponent(component))
			return false;
	}
	return true;
}
