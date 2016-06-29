#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <set>
#include "BuidingManager.h"
#include "OrderQueue.h"

class WorkerManager
{
private:
	static WorkerManager* workerManager;
	static bool isInstanced;

	BuidingManager* buildingManager;
	OrderQueue* orderQueue;
public:
	std::set<BWAPI::Unit> gasesPrepare;
	std::set<BWAPI::Unit> mineralsPrepare;
	std::set<BWAPI::Unit> gasesReal;
	std::set<BWAPI::Unit> mineralsReal;
	std::set<BWAPI::Unit> availableWorkers;
	int limitWorker = 14;
public:
	int WorkerManager::getWorkerCount();
	int WorkerManager::getNumMineralWorkers();
	int WorkerManager::getNumGasWorkers();
	int WorkerManager::getAvailableCount();
	int WorkerManager::getIdleCount();

	bool addWorkerMinerals(BWAPI::Unit unit);
	bool addWorkerGas(BWAPI::Unit unit);

	void removeWorker(BWAPI::Unit unit);
	
	bool WorkerManager::returnToMineral(BWAPI::Unit worker);
	bool WorkerManager::returnToGas(BWAPI::Unit worker);
	
	bool WorkerManager::makeAvailable(BWAPI::Unit worker);
	bool WorkerManager::makeUnavailable(BWAPI::Unit worker);

	void WorkerManager::handlerResourceWorker();
	void WorkerManager::handlerNumberWorker();
	void WorkerManager::handlerAddWorker(Unit worker);
	void WorkerManager::tranferWorker();
	

	void gatherMineral();
	void gatherGas();
	//by anh
	bool gatherGas(BWAPI::Unit worker, BWAPI::Unit refinery);

	static WorkerManager* newInstance()
	{
		if (!isInstanced)
		{
			workerManager = new WorkerManager();
			isInstanced = true;
			return workerManager;
		}
		return workerManager;
	}

	WorkerManager();
	~WorkerManager();
};
