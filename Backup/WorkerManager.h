#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <set>

class WorkerManager
{
public:
	std::set<BWAPI::Unit> workerMinerals;
	std::set<BWAPI::Unit> workerGas;
public:
	int WorkerManager::getWorkerCount();
	int WorkerManager::getNumMineralWorkers();
	int WorkerManager::getNumGasWorkers();
	void addWorkerMinerals(BWAPI::Unit unit);
	void addWorkerGas(BWAPI::Unit unit);
	WorkerManager();
	~WorkerManager();
};

