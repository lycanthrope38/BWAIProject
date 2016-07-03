#ifndef BuidingManager_h
#define BuidingManager_h

#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <set>


class BuidingManager
{
private:
	static BuidingManager* buidingManager;
	static bool isInstanced;

//	OrderQueue* orderQueue;
public:
	bool expanding;
	
	BWAPI::TilePosition centre;
	BWAPI::TilePosition	nextExpansionLocation;

	std::set<BWAPI::Unit> availableBuildingWorkers;
	std::set<BWAPI::Unit*> allWorkers;
	std::set<BWAPI::Unit> expansions;

public:	
	BuidingManager::BuidingManager();

	bool BuidingManager::placeBuilding(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition approxLocation);
	bool BuidingManager::aroundBuilding(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition aroundLocation);
	bool placeSpecific(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location);
	bool BuidingManager::createBuilding(BWAPI::Unit builder, BWAPI::UnitType building);
	bool buildingExpand();

	BWAPI::TilePosition BuidingManager::getClosestBase(BWAPI::Unit unit);
	BWAPI::TilePosition getNextBase(BWAPI::TilePosition basePosition, BWAPI::TilePosition shiftPosition);
	BWAPI::TilePosition getNextClosestBase(BWAPI::Unit unit, BWAPI::TilePosition tilePosition);
	
	void setCentre(BWAPI::TilePosition tilePosition);
	BWAPI::Unit getBuilding(BWAPI::UnitType buildingType);
	void startUpgrade(BWAPI::UnitType element, BWAPI::UpgradeType upgrade);
	
	BWAPI::TilePosition moveWorker(BWAPI::Unit unit, BWAPI::Position position);
	bool makeAvailableBuildingWorker(BWAPI::Unit worker);
	BWAPI::Unit getBuildingWorker();
	BWAPI::Unit getBuildingExpandWorker();
	bool addWorker(BWAPI::Unit* newWorker);
	void addExpansion(BWAPI::Unit expansion);
	void removeExpansion(BWAPI::Unit expansion);
	void removeBuildingWorker(BWAPI::Unit u);

	int getWorkerCount();
	int getNumMineralWorkers();
	int getNumGasWorkers();
	int getSizeExpansion();

	BWAPI::Unit getExpansion();

	BWAPI::TilePosition getExpansionLocation();

	
	static BuidingManager* newInstance()
	{
		if (!isInstanced)
		{
			buidingManager = new BuidingManager();
			isInstanced = true;
			return buidingManager;
		}
		return buidingManager;
	}

	~BuidingManager();
};

#endif
