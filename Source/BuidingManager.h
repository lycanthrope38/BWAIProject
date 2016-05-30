#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <set>
class BuidingManager
{
public:
	BWAPI::TilePosition centre;
	std::set<BWAPI::Unit> availableWorkers;
	std::set<BWAPI::Unit*> allWorkers;
public:
	BuidingManager::BuidingManager();
	bool BuidingManager::placeBuilding(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition approxLocation);
	BWAPI::TilePosition BuidingManager::getClosestBase(BWAPI::Unit unit);
	BWAPI::TilePosition getNextBase(BWAPI::TilePosition basePosition, BWAPI::TilePosition shiftPosition);
	bool placeExpansion(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location);
	void setCentre(BWAPI::TilePosition tilePosition);
	BWAPI::Unit getBuilding(BWAPI::UnitType buildingType);
	void startUpgrade(BWAPI::UnitType element, BWAPI::UpgradeType upgrade);
	BWAPI::TilePosition moveWorker(BWAPI::Unit unit, BWAPI::Position position);
	bool BuidingManager::createBuilding(BWAPI::Unit builder, BWAPI::UnitType building);
	bool makeAvailable(BWAPI::Unit worker);
	int getWorkerCount();
	BWAPI::Unit getWorker();
	int getNumMineralWorkers();
	int getNumGasWorkers();
	bool addWorker(BWAPI::Unit* newWorker);

	~BuidingManager();
};
