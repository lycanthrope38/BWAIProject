#pragma once

#include <BWAPI.h>
#include <BWTA.h>

class ScoutManager
{
private:
	BWAPI::Unit scout;
	std::set<BWAPI::Position> knownEnemyBases;
	std::set<std::pair<BWAPI::Unit, BWAPI::UnitType>> knownEnemyUnits;
public:
	ScoutManager();

	void update();
	void setScout(BWAPI::Unit scout);
	BWAPI::Unit getScout();
	void sendScout();
	void scoutExpos();

	void addEnemyBase(BWAPI::Unit enemyBase);
	void removeEnemyBase(BWAPI::Unit enemyBase);

	BWAPI::Position getClosestEnemyBase(BWAPI::Unit unit);
	BWAPI::Position getEnemyBase();

	void addEnemyUnit(BWAPI::Unit unit);
	void removeEnemyUnit(BWAPI::Unit unit);

	~ScoutManager();
};