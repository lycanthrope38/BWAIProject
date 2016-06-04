﻿#include "BuidingManager.h"

using namespace BWAPI;

#define SPIRALLIMIT 400
#define MINERALDIST 7


bool BuidingManager::placeBuilding(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition approxLocation)
{
	bool closeToMinerals = false;
	bool closeToGas = false;
	int count = 0;
	int spiralCount = 1;
	bool isX = true;
	bool isBuiding = false;
	BWAPI::TilePosition buildPosition;
	BWAPI::TilePosition shiftPositionX(5, 0);
	BWAPI::TilePosition shiftPositionY(0, 5);
	//lam error
	//check parameters are acceptable
	if (builder->getType() != BWAPI::Broodwar->self()->getRace().getWorker())
	{
		BWAPI::Broodwar->printf("Building Placement Error: unit type '%s' unit cannot create structures", builder->getType().getName().c_str());
		return false;
	}
	else if (!building.isBuilding())
	{
		BWAPI::Broodwar->printf("Building Placement Error: type: '%s' is not a valid structure", building.getName().c_str());
		return false;
	}
	else if (!approxLocation.isValid())
	{
		approxLocation = approxLocation.makeValid();
	}

	//begin search for valid structure placement at approxLocation and then spiral outwards until /
	//acceptable location is found
	buildPosition = approxLocation;
	while (!builder->build(building, buildPosition))
	{
		do{
			//check the location of the closest minerals - continue to increment position without /
			//attempting to construct until the minerals are no longer too close
			//This is to avoid constructing structures in mineral lines
			BWAPI::Unit closestMineral = NULL;
			for (Unit m : Broodwar->getMinerals())
			{
				if (closestMineral == NULL || buildPosition.getDistance((m)->getTilePosition()) < buildPosition.getDistance(closestMineral->getTilePosition()))
					closestMineral = m;
			}
			if (closestMineral != NULL)
			{
				if (buildPosition.getDistance(closestMineral->getTilePosition()) < MINERALDIST)
				{
					closeToMinerals = true;
				}
				else
				{
					closeToMinerals = false;
				}
			}

			//check we aren't blocking gas too
			Unit closestGas = NULL;
			for (Unit g : Broodwar->getGeysers())
			{
				if (closestGas == NULL || buildPosition.getDistance((g)->getTilePosition()) < buildPosition.getDistance(closestGas->getTilePosition()))
					closestGas = g;
			}
			if (closestGas != NULL)
			{
				if (buildPosition.getDistance(closestMineral->getTilePosition()) < MINERALDIST)
				{
					closeToGas = true;
				}
				else
				{
					closeToGas = false;
				}
			}

			if (count % spiralCount == 0)
			{
				if (isX)
				{
					isX = false;
					count = 0;
				}
				else {
					spiralCount++;
					count = 0;
					isX = true;
				}
			}
			count++;
			if (spiralCount % 2 == 0)
			{
				if (isX)
				{
					buildPosition -= shiftPositionX;
				}
				else
				{
					buildPosition -= shiftPositionY;
				}
			}
			else
			{
				if (isX)
				{
					buildPosition += shiftPositionX;
				}
				else
				{
					buildPosition += shiftPositionY;
				}
			}
			//search is cut off at SPIRALLIMIT to prevent it from taking too long or placing building to far from approxLocation
			if (spiralCount == SPIRALLIMIT)
			{
				//	Broodwar->printf("Building Placement Error: no acceptable build location found for %s", building.getName().c_str());
				return false;
			}
		} while (closeToMinerals || closeToGas);
	}
	return true;
}

bool BuidingManager::createBuilding(BWAPI::Unit builder, BWAPI::UnitType building)
{
	BWAPI::TilePosition buildPosition;

	
	//if we have analyzed the map then we will set the buildPosition at the centre of our region, otherwise it will be set to our main base
	if (centre != TilePosition(0, 0))
	{
		buildPosition = centre;
	}
	else
	{
		for (Unit i : Broodwar->self()->getUnits())
		{
			if ((i)->getType().isResourceDepot())
			{
				buildPosition = (i)->getTilePosition();
				break;
			}
		}
	}

	//buildPosition = moveWorker(builder, builder->getPosition() + shiftPositionX);

	
		if (placeBuilding(builder, building, buildPosition))
		{
			return true;
		}
	
	return false;

}


void BuidingManager::setCentre(BWAPI::TilePosition tilePosition)
{
	centre = tilePosition;
}

// xây dựng các công trình tại vị trí hiện tại của builder để tránh việc mở rộng và xây dựng không đúng chỗ
bool BuidingManager::placeExpansion(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location)
{
	if (builder->build(building,TilePosition(builder->getPosition())))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// đẩy dịch chuyển các vị trí xây nhà 
BWAPI::TilePosition BuidingManager::getNextBase(BWAPI::TilePosition basePosition, BWAPI::TilePosition shiftPosition)
{
	return basePosition;
}



BWAPI::Unit BuidingManager::getBuilding(BWAPI::UnitType buildingType)
{
	for (Unit i : Broodwar->getAllUnits())
	{
		if (i->getType().isBuilding())
		if ((i->getType() == buildingType) && (i->isIdle()) && (i->isCompleted()))
		{
			return i;
		}
	}
	return NULL;
}

void BuidingManager::startUpgrade(UnitType element, UpgradeType upgrade)
{
	BWAPI::Unit structure;

	structure = getBuilding(element);
	if (structure != NULL && structure->isCompleted())
	{
		structure->upgrade(upgrade);
	}
	else
	{
		Broodwar->printf("ProductionManager Error: No structure available to research '%s'", element.getName().c_str());
	}
}

//move woker to specific place 
TilePosition BuidingManager::moveWorker(Unit unit, Position position)
{
	/*while (unit->getPosition()!=position)
	{*/
		unit->move(position, false);
	//}
	
	return unit->getTilePosition();
}

bool BuidingManager::makeAvailable(BWAPI::Unit worker)
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

BWAPI::Unit BuidingManager::getWorker()
{
	BWAPI::Unit worker;

	for (Unit i : availableWorkers)
	{
		if (i != NULL)
		{
			worker = i;
			return worker;
		}
	}
	return NULL;
}

int BuidingManager::getWorkerCount()
{
	return availableWorkers.size();
}

BWAPI::Position BuidingManager::getNextClosestPlaceBuidling()
{
	auto s = BWTA::getStartLocations().begin();
	BWTA::BaseLocation* home = BWTA::getStartLocation(BWAPI::Broodwar->self());
	BWTA::BaseLocation* temp;

	int count = 0;

	while (s != BWTA::getStartLocations().end())
	{
		if ((*s)->getRegion() != BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion())
		{
			
			if (count==0||(*s)->getPosition().getDistance(home->getPosition())<temp->getPosition().getDistance(home->getPosition()))
			{
				temp = (*s);
				count = 1;
			}
		}
		s++;
	}
	return temp->getPosition();

}





BuidingManager::BuidingManager()	
{
}


BuidingManager::~BuidingManager()
{
}
