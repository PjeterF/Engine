#include "ComponentBase.hpp"

ComponentBase::ComponentBase(int type)
{
	this->type = type;
}

int ComponentBase::getType()
{
	return type;
}

void ComponentBase::to_json(nlohmann::json& j) const
{
	j["type"] = this->type;
}

void ComponentBase::from_json(nlohmann::json& j)
{
	this->type = j["type"];
}
