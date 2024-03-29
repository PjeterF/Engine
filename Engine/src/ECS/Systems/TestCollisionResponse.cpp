#include "TestCollisionResponse.hpp"

TestCollisionResponse::TestCollisionResponse(CollisionSystem* collisionSystem) : SystemBase(UNPAUSED)
{
	this->requiredComponents = { Transform, BoxCollider, Velocity };
	this->collisionSystem = collisionSystem;
}

void TestCollisionResponse::update(float dt)
{
	for (auto& entity : entities)
	{
		auto transform = (TransformC*)entity->getComponent(CType::Transform);
		auto velocity = (VelocityC*)entity->getComponent(CType::Velocity);
		auto collider = (BoxColliderC*)entity->getComponent(CType::BoxCollider);

		for (auto& collisionID : collider->collisionsIDs)
		{
			auto collision = collisionSystem->getCollision(collisionID);
			if (collision != nullptr)
			{
				if (collision->state == CollisionECS::Enter)
					velocity->velocity = -velocity->velocity;
			}
		}
	}
}
