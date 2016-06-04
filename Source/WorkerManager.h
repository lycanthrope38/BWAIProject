#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <set>

class WorkerManager
{
public:
	std::set<std::pair<BWAPI::Unit, std::set<BWAPI::Unit>>> gases;
	std::set<BWAPI::Unit> availableWorkers;
	int limitWorker = 12;
public:
	int WorkerManager::getWorkerCount();
	int WorkerManager::getNumMineralWorkers();
	int WorkerManager::getNumGasWorkers();
	int WorkerManager::getAvailableCount();
	int WorkerManager::getIdleCount();
	void addWorkerMinerals(BWAPI::Unit unit);
	void addWorkerGas(BWAPI::Unit unit);
	bool WorkerManager::returnToMineral(BWAPI::Unit worker);
	bool WorkerManager::returnToGas(BWAPI::Unit worker);
	bool WorkerManager::makeAvailable(BWAPI::Unit worker);
	bool WorkerManager::makeUnavailable(BWAPI::Unit worker);
	void gatherMineral();
	void gatherGas();

	WorkerManager();
	~WorkerManager();
};

