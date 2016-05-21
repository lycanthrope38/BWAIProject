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

void WorkerManager::addWorkerGas(BWAPI::Unit unit)
{
	workerGas.insert(unit).second;
}

void WorkerManager::addWorkerMinerals(BWAPI::Unit unit)
{
	workerMinerals.insert(unit).second;
}



WorkerManager::WorkerManager()
{
}


WorkerManager::~WorkerManager()
{
}
