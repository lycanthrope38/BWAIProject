#include "ScoutManager.h"

ScoutManager::ScoutManager() :scout(nullptr)
{
}

void ScoutManager::setScout(BWAPI::Unit scout)
{
	this->scout = scout;
}

BWAPI::Unit ScoutManager::getScout()
{
	return scout;
}

void ScoutManager::update()
{
}

/*
sends the scout to the possible enemy base locations
*/
void ScoutManager::sendScout()
{
	if (!scout || !scout->exists())
	{
		return;
	}

	if (!BWTA::getStartLocations().empty())
	{
		auto s = BWTA::getStartLocations().begin();

		if ((*s)->getRegion() != BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion())
		{
			scout->move((*s)->getPosition(), false);
		}
		else
		{
			s++;
			scout->move((*s)->getPosition(), false);
		}

		while (s != BWTA::getStartLocations().end())
		{
			if ((*s)->getRegion() != BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion())
			{
				scout->move((*s)->getPosition(), true);
			}
			s++;
		}


		/*for (auto u : BWAPI::Broodwar->self()->getUnits())
		{
			if (u->getType().isResourceDepot())
			{
				scout->move(u->getPosition(), true);
				break;
			}
		}*/
	}
	else
	{
		BWAPI::Broodwar->printf("ScoutManager Error: cannot send scout because map has not been analysed");
	}
}


void ScoutManager::scoutExpos()
{
	if (!BWTA::getBaseLocations().empty())
	{
		scout->move((*BWTA::getBaseLocations().begin())->getPosition(), false);

		for (auto base : BWTA::getBaseLocations())
		{
			scout->move(base->getPosition(), true);
		}
	}
	else
	{
		return;
	}

	for (auto u : BWAPI::Broodwar->self()->getUnits())
	{
		if (u->getType().isResourceDepot())
		{
			scout->move(u->getPosition(), true);
			break;
		}
	}
}

/*
adds an enemy base to the set of known enemy base locations
*/
void ScoutManager::addEnemyBase(BWAPI::Unit enemyBase)
{
	knownEnemyBases.insert(enemyBase->getPosition());
}

void ScoutManager::removeEnemyBase(BWAPI::Unit enemyBase)
{
	knownEnemyBases.erase(enemyBase->getPosition());
}

/*
checks all of the known enemy base locations and returns the one that is closest to the unit* given as a parameter
*/
BWAPI::Position ScoutManager::getClosestEnemyBase(BWAPI::Unit unit)
{
	BWAPI::Position closestEnemyBase = BWAPI::Position(0, 0);

	if (knownEnemyBases.empty())
	{
		return BWAPI::Position(0, 0);
	}

	for (const auto& base : knownEnemyBases)
	{
		if ((closestEnemyBase == BWAPI::Position(0, 0)) || (unit->getDistance(base) < unit->getDistance(closestEnemyBase)))
		{
			closestEnemyBase = base;
		}
	}

	return closestEnemyBase;
}

/*
returns a known enemy base location
*/
BWAPI::Position ScoutManager::getEnemyBase()
{
	if (knownEnemyBases.empty())
	{
		return BWAPI::Position(0, 0);
	}
	
	//refineries are not properly removed from the set of buildings when destroyed
	//if we find a position of a neutral geyser in the set of enemy bases, then remove it
	for (auto geyser : BWAPI::Broodwar->getGeysers())
	{
		if (geyser->getPosition() == *knownEnemyBases.begin() && geyser->getType().isNeutral())
		{
			knownEnemyBases.erase(geyser->getPosition());
			break;
		}
	}

	if (knownEnemyBases.empty())
	{
		return BWAPI::Position(0, 0);
	}

	return *knownEnemyBases.begin();
}

/*
adds a discovered enemy unit to the set of known enemy units
*/
void ScoutManager::addEnemyUnit(BWAPI::Unit unit)
{
	knownEnemyUnits.insert(std::make_pair(unit, unit->getType()));
}

/*
removes a destroyed enemy unit from the set of known enemy units
*/
void ScoutManager::removeEnemyUnit(BWAPI::Unit unit)
{
	for (auto u : knownEnemyUnits)
	{
		if (u.first == unit)
		{
			knownEnemyUnits.erase(u);
			break;
		}
	}
}

ScoutManager::~ScoutManager()
{
}