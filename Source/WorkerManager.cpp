#include "WorkerManager.h"
#include <BWAPI.h>

WorkerManager* WorkerManager::workerManager = nullptr;
bool WorkerManager::isInstanced = false;
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


int WorkerManager::getIdleCount()
{
	int count = 0;

	for (BWAPI::Unit i : BWAPI::Broodwar->self()->getUnits())
	{
		if (i->getType().isWorker())
		{
			if (i->isIdle())
			{
				count++;
			}

		}
	}
	return count;
}






//void WorkerManager::addWorkerGas(BWAPI::Unit gas)
//{
//	std::set<BWAPI::Unit> workers;
//
//	//limiting the number of geysers we will mine from to 2
//	if (!(gasesPrepare.size() > 2))
//	{
//		gasesPrepare.insert(std::make_pair(gas, workers));
//	}
//}

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
			//BWAPI::Broodwar->printf("WorkerManager Error: command to mine mineralsPrepare failed");
			return false;
		}
	}
	//BWAPI::Broodwar->printf("WorkerManager Error: unable to find mineral field");
	return false;
}

/*
Issues a command to a worker to mine the nearest mineral patch
*/
bool WorkerManager::returnToGas(BWAPI::Unit worker)
{
	BWAPI::Unit closestGeyser = nullptr;
	for (BWAPI::Unit m : BWAPI::Broodwar->getGeysers())
	{
		if (closestGeyser == nullptr || worker->getDistance(m) < worker->getDistance(closestGeyser))
			closestGeyser = m;
	}
	if (closestGeyser != nullptr)
	{
		if (worker->gather(closestGeyser))
		{
			return true;
		}
		else
		{
			//BWAPI::Broodwar->printf("WorkerManager Error: command to mine gas failed");
			return false;
		}
	}
	//BWAPI::Broodwar->printf("WorkerManager Error: unable to find gas field");
	return false;
}

void WorkerManager::tranferWorker()
{

	gatherMineral();

	gatherGas();
}

void WorkerManager::gatherMineral()
{
	
	for (BWAPI::Unit u : mineralsPrepare)
	{
		if (u->isIdle() && u != buildingManager->getBuildingWorker())
		{
			if (u->gather(u->getClosestUnit(BWAPI::Filter::IsMineralField)))
			{
				
			}
		}
		
	

	}
}

void WorkerManager::gatherGas()
{
	
	for (BWAPI::Unit u : gasesPrepare)
	{
		if (u->isIdle() && u != buildingManager->getBuildingWorker())
		{
			if (u->gather(u->getClosestUnit(BWAPI::Filter::IsRefinery)) )
			{
				
			}
		}

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


int WorkerManager::getAvailableCount()
{
	return availableWorkers.size();
}

void WorkerManager::handlerResourceWorker()
{
	if (getNumMineralWorkers()<15)
	{
		//orderQueue.push(new OrderType(UnitTypes::Protoss_Probe,orderQueue->PRIORITY_HIGH));
	}
}

void WorkerManager::handlerNumberWorker()
{

		if (getNumMineralWorkers()<15||getNumGasWorkers()<3)
		{
			Unit resourceDepot = buildingManager->getExpansion();
			resourceDepot->train(resourceDepot->getType().getRace().getWorker());
		}
	
	
}

void WorkerManager::handlerAddWorker(Unit worker)
{
	if (getNumMineralWorkers()<15)
	{
		addWorkerMinerals(worker);
		return;
	}
	if (getNumGasWorkers()<=3)
	{
		addWorkerGas(worker);
	}

}



void WorkerManager::removeWorker(BWAPI::Unit unit)
{
	gasesPrepare.erase(unit);
	mineralsPrepare.erase(unit);
	availableWorkers.erase(unit);
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

bool WorkerManager::addWorkerGas(BWAPI::Unit unit)
{
	BWAPI::Broodwar->printf("addWorkerGas '%d'" ,unit->getID());
	if (gasesPrepare.insert(unit).second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkerManager::addWorkerMinerals(BWAPI::Unit unit)
{
	//BWAPI::Broodwar->printf("addWorkerMinerals '%d'" ,unit->getID());
	if (mineralsPrepare.insert(unit).second)

	{
		return true;
	}
	else
	{
		return false;
	}
}

//by anh

bool WorkerManager::gatherGas(BWAPI::Unit worker, BWAPI::Unit refinery){
	return worker->gather(refinery);
}

WorkerManager::WorkerManager()
{
	orderQueue = OrderQueue::getInstance();
	buildingManager = BuidingManager::newInstance();
}


WorkerManager::~WorkerManager()
{
}