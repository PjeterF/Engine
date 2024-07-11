#include "ShooterS.hpp"

#include "../../functions.hpp"

#include <filesystem>

ShooterS::ShooterS(std::vector<std::string> marbleArchetypeFilepaths, MovementS& msys, RenderingS& rsys, CollisionS& csys, AnimationS& asys, MarbleCollisionResolutionS& mrsys)
	: marbleArchetypeFilepaths(marbleArchetypeFilepaths), msys(msys), rsys(rsys), csys(csys), asys(asys), mrsys(mrsys)
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(MarbleShooter))
	};
}

void ShooterS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto shooterCPool = ComponentPoolManager::getInstance().getPool<MarbleShooter>();

	for (auto ID : entities)
	{
		auto& transform = transformPool->get(ID);
		auto& shooterC = shooterCPool->get(ID);

		if (shooterC.counter != 0)
			shooterC.counter--;

		if (shooterC.currentShotID == -1)
		{
			shooterC.currentShotID = shooterC.nextShotID;
			shooterC.nextShotID = -1;
		}
		else
		{
			Transform& currTrans = transformPool->get(shooterC.currentShotID);

			glm::vec2 offset =  transform.height * glm::vec2(cos(transform.rot), -sin(transform.rot));
			currTrans.x = transform.x + offset.x;
			currTrans.y = transform.y + offset.y;
		}

		if (shooterC.nextShotID == -1)
		{
			int marbleID = ZumaFn::spawnMarble(marbleArchetypeFilepaths[rand() % marbleArchetypeFilepaths.size()], { transform.x, transform.y }, "");

			msys.addEntity(marbleID);
			rsys.addEntity(marbleID);
			csys.addEntity(marbleID);
			asys.addEntity(marbleID);
			mrsys.addEntity(marbleID);

			shooterC.nextShotID = marbleID;
		}
		else
		{
			Transform& nextTrans = transformPool->get(shooterC.nextShotID);

			nextTrans.x = transform.x;
			nextTrans.y = transform.y;
		}
	}
}

void ShooterS::handleEvent(Event& event)
{
	SysBase::handleEvent(event);

	switch (event.getType())
	{
	case Event::Shoot:
	{
		auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
		auto shooterCPool = ComponentPoolManager::getInstance().getPool<MarbleShooter>();
		auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

		glm::vec2* targetPos = (glm::vec2*)event.getPayload();

		for (auto ID : entities)
		{
			auto& transform = transformPool->get(ID);
			auto& shooterC = shooterCPool->get(ID);

			if (shooterC.currentShotID == -1 || shooterC.counter!=0)
				continue;

			glm::vec2 dir = { targetPos->x - transform.x, targetPos->y - transform.y };
			if (glm::length(dir) < 0.001)
				dir = { 0, 1 };
			else
				dir = glm::normalize(dir);

			Velocity& shotVel = velocityPool->get(shooterC.currentShotID);

			shotVel.x = shooterC.shotSpeed * dir.x;
			shotVel.y = shooterC.shotSpeed * dir.y;

			shooterC.currentShotID = -1;
			shooterC.counter = shooterC.cooldown;

			transform.rot = atan2(-dir.y, dir.x)-3.14/2;
		}
	}
	case Event::UpdateMarbleArchetypes:
	{
		namespace fs = std::filesystem;

		fs::path dirPath(archetypesDirectoryFilepath);

		marbleArchetypeFilepaths.clear();
		for (auto& entry : fs::directory_iterator(dirPath))
		{
			std::string fileName = entry.path().string();
			marbleArchetypeFilepaths.push_back(fileName);
		}
	}
	break;
	}
}