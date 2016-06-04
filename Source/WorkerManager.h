#ifndef WorkerManager_h
#define WorkerManager_h

#pragma once
#include "BuidingManager.h"
#include <BWAPI.h>
#include <BWTA.h>
#include <set>

class BuidingManager;

class WorkerManager
{
public:
	std::set<std::pair<BWAPI::Unit, std::set<BWAPI::Unit>>> gases;
	std::set<std::pair<BWAPI::Unit, std::set<BWAPI::Unit>>> bases;
	std::set<BWAPI::Unit> availableWorkers;
	int limitWorker = 12;
	BWAPI::Unit expansionBuilder;
	BuidingManager buidingManager;
public:
	int getWorkerCount();
	int getNumMineralWorkers();
	int getNumGasWorkers();
	int getAvailableCount();
	int getIdleCount();
	void addWorkerMinerals(BWAPI::Unit unit);
	void addWorkerGas(BWAPI::Unit unit);
	bool returnToMineral(BWAPI::Unit worker);
	bool returnToGas(BWAPI::Unit worker);
	bool makeAvailable(BWAPI::Unit worker);
	bool makeUnavailable(BWAPI::Unit worker);
	BWAPI::Unit getExpansionBuilder();
	void setExpansionBuilder();
	void addExpansion(BWAPI::Unit expansion);
	void gatherMineral();
	void gatherGas();

	WorkerManager();
	~WorkerManager();
};

#endif
