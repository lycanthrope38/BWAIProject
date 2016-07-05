#include "WorkerManager.h"
#include <BWAPI.h>
#include "ArmyOrder.h"

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
			//BWAPI::Broodwar->printf("WorkerManager Error: command to mine minerals failed");
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
	if (Broodwar->getFrameCount() > 3000)
	{
		minerals.erase(buildingManager->getBuildingExpandWorker());
	}
	for (BWAPI::Unit u : minerals)
	{
		if (u->isIdle() && u != buildingManager->getBuildingWorker() && u != buildingManager->getBuildingExpandWorker())
		{
			if (u->gather(u->getClosestUnit(BWAPI::Filter::IsMineralField)))
			{

			}
		}



	}
}

void WorkerManager::gatherGas()
{

	for (BWAPI::Unit u : gases)
	{
		if (u->isIdle() && u != buildingManager->getBuildingWorker())
		{
			if (u->gather(u->getClosestUnit(BWAPI::Filter::IsRefinery)))
			{

			}
		}

	}
}




int WorkerManager::getAvailableCount()
{
	return availableWorkers.size();
}

void WorkerManager::handlerResourceWorker()
{
	if (getNumMineralWorkers() < 15)
	{
		//orderQueue.push(new OrderType(UnitTypes::Protoss_Probe,orderQueue->PRIORITY_HIGH));
	}
}

void WorkerManager::handlerNumberWorker()
{

	if ((getNumMineralWorkers() < 15 || getNumGasWorkers() < 3) && staticOrderQueue->isEmpty())
	{
		Unit resourceDepot = buildingManager->getExpansion();
		if (buildingManager->getSizeExpansion() > 0 && resourceDepot->isIdle() && !armyOrder->train((new OrderType(Broodwar->self()->getRace().getWorker(), resourceDepot->getType(), 1))))
		{

		}

	}


}

void WorkerManager::handlerAddWorker(Unit worker)
{
	if ((getNumMineralWorkers() < 15 && Collections::getUnitVolume(Broodwar->self(), Broodwar->self()->getRace().getRefinery()) == 0)
		|| getNumGasWorkers() > 3)
	{
		addWorkerMinerals(worker);
		return;
	}
	if (getNumGasWorkers() <= 3)
	{
		addWorkerGas(worker);
	}
	else
		addWorkerMinerals(worker);
}



void WorkerManager::removeWorker(BWAPI::Unit unit)
{
	gases.erase(unit);
	minerals.erase(unit);
	availableWorkers.erase(unit);
}


bool WorkerManager::addWorkerGas(BWAPI::Unit unit)
{
	BWAPI::Broodwar->printf("addWorkerGas '%d'", unit->getID());
	if (gases.insert(unit).second)
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
	if (minerals.insert(unit).second)

	{
		return true;
	}
	else
	{
		return false;
	}
}


WorkerManager::WorkerManager()
{
	orderQueue = OrderQueue::getInstance();
	buildingManager = BuidingManager::newInstance();
	staticOrderQueue = StaticOrder::getInstance();
	armyOrder = new ArmyOrder(Broodwar->self());
}


WorkerManager::~WorkerManager()
{
}