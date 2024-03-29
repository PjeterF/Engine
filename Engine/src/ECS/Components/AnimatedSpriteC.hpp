#pragma once

#include "ComponentBase.hpp"
#include "SpriteC.hpp"

struct TextureDivision
{
	TextureDivision(float x = 0, float y = 0, float width = 1, float height = 1);
	float x, y, width, height;
};

class AnimatedSpriteC : public SpriteC
{
public:
	AnimatedSpriteC(Resource<Texture>* texture = nullptr, std::vector<TextureDivision> divisions = std::vector<TextureDivision>(), int frameDuration = 1);

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	std::vector<TextureDivision> divisions;
	int frameDuration;
	int currentFrame = 0;
	int counter = 0;
	bool paused = false;
};