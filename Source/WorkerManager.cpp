#include "WorkerManager.h"
#include <BWAPI.h>

int WorkerManager::getNumMineralWorkers()
{
	int count = 0;

	for (BWAPI::Unit i : BWAPI::Broodwar->self()->getUnits())
	{
		if ((i)->isGatheringMinerals())
		{
			count++;
		}
	}
	return count;
}

int WorkerManager::getNumGasWorkers()
{
	int count = 0;

	for (BWAPI::Unit i : BWAPI::Broodwar->self()->getUnits())
	{
		if ((i)->isGatheringGas())
		{
			count++;
		}
	}
	return count;
}

int WorkerManager::getWorkerCount()
{
	int count = 0;

	for (BWAPI::Unit i : BWAPI::Broodwar->self()->getUnits())
	{
		if (i->getType().isWorker())
		{
			count++;
		}
	}
	return count;
}



void WorkerManager::addWorkerGas(BWAPI::Unit gas)
{
	std::set<BWAPI::Unit> workers;

	//limiting the number of geysers we will mine from to 2
	if (!(gases.size() > 2))
	{
		gases.insert(std::make_pair(gas, workers));
	}
}
	
/*
Issues a command to a worker to mine the nearest mineral patch
*/
bool WorkerManager::returnToMineral(BWAPI::Unit worker)
{
	BWAPI::Unit closestMineral = NULL;
	for (BWAPI::Unit m : BWAPI::Broodwar->getMinerals())
	{
		if (closestMineral == NULL || worker->getDistance(m) < worker->getDistance(closestMineral))
			closestMineral = m;
	}
	if (closestMineral != NULL)
	{
		if (worker->rightClick(closestMineral))
		{
			return true;
		}
		else
		{
			BWAPI::Broodwar->printf("WorkerManager Error: command to mine minerals failed");
			return false;
		}
	}
	BWAPI::Broodwar->printf("WorkerManager Error: unable to find mineral field");
	return false;
}

/*
Issues a command to a worker to mine the nearest mineral patch
*/
bool WorkerManager::returnToGas(BWAPI::Unit worker)
{
	BWAPI::Unit closestGeyser = NULL;
	for (BWAPI::Unit m : BWAPI::Broodwar->getGeysers())
	{
		if (closestGeyser == NULL || worker->getDistance(m) < worker->getDistance(closestGeyser))
			closestGeyser = m;
	}
	if (closestGeyser != NULL)
	{
		if (worker->rightClick(closestGeyser))
		{
			return true;
		}
		else
		{
			BWAPI::Broodwar->printf("WorkerManager Error: command to mine gas failed");
			return false;
		}
	}
	BWAPI::Broodwar->printf("WorkerManager Error: unable to find gas field");
	return false;
}

bool WorkerManager::makeAvailable(BWAPI::Unit worker)
{
	if (availableWorkers.insert(worker).second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkerManager::makeUnavailable(BWAPI::Unit worker)
{
	if (availableWorkers.erase(worker))
	{
		return true;
	}
	else
	{
		return false;
	}
}




//void WorkerManager::addWorkerGas(BWAPI::Unit unit)
//{
//	workerGas.insert(unit).second;
//}
//
//void WorkerManager::addWorkerMinerals(BWAPI::Unit unit)
//{
//	workerMinerals.insert(unit).second;
//}



WorkerManager::WorkerManager()
{
}


WorkerManager::~WorkerManager()
{
}
