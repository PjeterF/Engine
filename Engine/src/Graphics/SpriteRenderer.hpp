#pragma once

#include "../OpenGL/package.hpp"
#include "glm/vec2.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include <iostream>

#include "../Events/EventPackage.hpp"
#include "../Graphics/Camera.hpp"

#include "../ECS/Components/AnimatedSpriteC.hpp"

class SpriteRenderer : public EventObserver
{
public:
	SpriteRenderer(GLuint shaderProgramID, Camera* camera);
	void draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture);
	void draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture, TextureDivision division);

	virtual void handleEvent(Event& event) override;

	Camera* camera;
protected:
	ArrayBuffer* VBO;
	VertexArray* VAO;
	ElementArrayBuffer* EBO;
	GLuint shaderProgramID;

};