#pragma once

#include "ComponentBase.hpp"

class RenderingLayerC : public ComponentBase
{
public:
	RenderingLayerC(int layer = 0);

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	int layer = 0;
};