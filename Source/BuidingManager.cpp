#include "BuidingManager.h"

using namespace BWAPI;

#define SPIRALLIMIT 100
#define MINERALDIST 7

BuidingManager* BuidingManager::buidingManager = nullptr;
bool BuidingManager::isInstanced = false;

bool BuidingManager::placeBuilding(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition approxLocation)
{


	bool isCloseToCentre = false;
	bool closeToGas = false;
	int count = 0;
	int spiralCount = 1;
	bool isX = true;
	bool isBuiding = false;
	BWAPI::TilePosition buildPosition;
	BWAPI::TilePosition shiftPositionX(2, 0);
	BWAPI::TilePosition shiftPositionY(0, 2);

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

	buildPosition = approxLocation;


	while (!builder->build(building, buildPosition))
	{

		do{
			//check the location of the closest minerals - continue to increment position without /
			//attempting to construct until the minerals are no longer too close
			//This is to avoid constructing structures in mineral lines
			BWAPI::Unit closestCentre = nullptr;


			for (Unit m : Broodwar->getMinerals())
			{
				if (closestCentre == nullptr || buildPosition.getDistance((m)->getTilePosition()) < buildPosition.getDistance(closestCentre->getTilePosition()))
					closestCentre = m;
			}
			if (closestCentre != nullptr)
			{
				//	BWAPI::Broodwar->printf("isCloseToCentre isCloseToCentre ");

				if ((3 < buildPosition.getDistance(approxLocation)) && (buildPosition.getDistance(approxLocation) < MINERALDIST))
				{
					isCloseToCentre = true;
					BWAPI::Broodwar->printf("isCloseToCentre isCloseToCentre isCloseToCentre");
				}
				else
				{
					isCloseToCentre = false;
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

			BWAPI::Broodwar->printf("spiralCount spiralCount spiralCount '%d'", spiralCount);
			//search is cut off at SPIRALLIMIT to prevent it from taking too long or placing building to far from approxLocation
			if (spiralCount == SPIRALLIMIT)
			{
				//	Broodwar->printf("Building Placement Error: no acceptable build location found for %s", building.getName().c_str());
				return false;
			}
		} while (isCloseToCentre);
	}
	return true;
}

bool BuidingManager::aroundBuilding(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition aroundLocation)
{

	BWAPI::Broodwar->printf("aroundBuilding aroundBuilding aroundBuilding");

	bool isCloseToCentre = false;
	bool closeToGas = false;
	int count = 0;
	int spiralCount = 1;
	bool isX = true;
	bool isBuiding = false;
	BWAPI::TilePosition buildPosition;
	BWAPI::TilePosition shiftPositionX(1, 0);
	BWAPI::TilePosition shiftPositionY(0, 1);

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
	else if (!aroundLocation.isValid())
	{
		aroundLocation = aroundLocation.makeValid();
	}

	buildPosition = aroundLocation;


	while (!builder->build(building, buildPosition))
	{

		do{
			//check the location of the closest minerals - continue to increment position without /
			//attempting to construct until the minerals are no longer too close
			//This is to avoid constructing structures in mineral lines
			BWAPI::Unit closestCentre = nullptr;

			if (aroundLocation != TilePosition(0, 0))
			{
				if ((buildPosition.getDistance(aroundLocation) <= 1))
				{
					isCloseToCentre = true;
				}
				else
				{
					isCloseToCentre = false;
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
			if (spiralCount % 2 != 0)
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
			if (spiralCount == 50)
			{
				//	Broodwar->printf("Building Placement Error: no acceptable build location found for %s", building.getName().c_str());
				return false;
			}
		} while (isCloseToCentre);
	}
	return true;
}

bool BuidingManager::aroundBuilding(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition aroundLocation, bool isLarge)
{

	BWAPI::Broodwar->printf("aroundBuilding aroundBuilding aroundBuilding");

	bool isCloseToCentre = false;
	bool closeToGas = false;
	int count = 0;
	int spiralCount = 1;
	bool isX = true;
	bool isBuiding = false;
	BWAPI::TilePosition buildPosition;
	BWAPI::TilePosition shiftPositionX(2, 0);
	BWAPI::TilePosition shiftPositionY(0, 2);

	if (builder->getType() != BWAPI::Broodwar->self()->getRace().getWorker())
	{
		BWAPI::Broodwar->printf("Building Placement Error: unit type '%s' unit cannot create structures", builder->getType().getName().c_str());
		return false;
	}
	else if (!building.isBuilding() )
	{
		BWAPI::Broodwar->printf("Building Placement Error: type: '%s' is not a valid structure", building.getName().c_str());
		return false;
	}
	else if (!aroundLocation.isValid())
	{
		aroundLocation = aroundLocation.makeValid();
	}

	buildPosition = aroundLocation;


	while (!builder->build(building, buildPosition) && !Broodwar->canBuildHere(buildPosition, building, builder, false))
	{

		do{
			//check the location of the closest minerals - continue to increment position without /
			//attempting to construct until the minerals are no longer too close
			//This is to avoid constructing structures in mineral lines
			BWAPI::Unit closestCentre = nullptr;

			if (aroundLocation != TilePosition(0, 0))
			{
				if ((buildPosition.getDistance(aroundLocation) <= 3))
				{
					isCloseToCentre = true;
				}
				else
				{
					isCloseToCentre = false;
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
			if (spiralCount % 2 != 0)
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
			if (spiralCount == 50)
			{
				//	Broodwar->printf("Building Placement Error: no acceptable build location found for %s", building.getName().c_str());
				return false;
			}
		} while (isCloseToCentre);
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



	buildPosition = BWAPI::Broodwar->getBuildLocation(building, builder->getTilePosition());

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
bool BuidingManager::placeSpecific(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location)
{
	if (builder->build(building, TilePosition(builder->getPosition())))
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

bool BuidingManager::makeAvailableBuildingWorker(BWAPI::Unit worker)
{
	if (availableBuildingWorkers.insert(worker).second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BWAPI::Unit BuidingManager::getBuildingExpandWorker()
{
	BWAPI::Unit worker;

	for (Unit i : availableBuildingWorkers)
	{
		if (i != NULL&&i != getBuildingWorker())
		{
			worker = i;
			return worker;
		}
	}
	return NULL;
}


BWAPI::Unit BuidingManager::getBuildingWorker()
{
	BWAPI::Unit worker;

	for (Unit i : availableBuildingWorkers)
	{
		if (i != NULL)
		{
			worker = i;
			return worker;
		}
	}
	return NULL;
}

void BuidingManager::removeBuildingWorker(Unit u)
{
	availableBuildingWorkers.erase(u);

	if (availableBuildingWorkers.size() == 0)
	{
		int count = 0;
		for (Unit i : Broodwar->self()->getUnits())
		{
			if (i->getType().isWorker())
			{
				makeAvailableBuildingWorker(i);
				count += 1;
				if (count == 3)
				{
					return;
				}
			}

		}
	}
}


int BuidingManager::getWorkerCount()
{
	return availableBuildingWorkers.size();
}

/*
find the closest available base location to the given unit
*/
BWAPI::TilePosition BuidingManager::getClosestBase(BWAPI::Unit unit)
{
	BWAPI::TilePosition buildPosition = TilePosition(0, 0);
	BWAPI::TilePosition buildPositionFirst = TilePosition(0, 0);
	double minDist = 0;
	bool token;
	int countTryToFind = 0;
	if (expansions.size() > 0)
	{
		do
		{
			countTryToFind += 1;
			for (std::set<BWTA::BaseLocation*>::const_iterator i = BWTA::getBaseLocations().begin(); i != BWTA::getBaseLocations().end(); i++)
			{
				if ((*i)->getTilePosition() != buildPositionFirst)
				{
					token = false;
					for (BWAPI::Unit j : expansions)
					{
						if ((j)->getTilePosition() == (*i)->getTilePosition())
						{
							token = true;
						}
					}
					if (!token)
					{
						if ((minDist == 0) || (unit->getPosition().getDistance((*i)->getPosition()) < minDist))
						{
							minDist = unit->getPosition().getDistance((*i)->getPosition());
							buildPosition = (*i)->getTilePosition();
						}
					}
				}
			}
			buildPositionFirst = buildPosition;
			if (countTryToFind == 10)
			{
				break;
			}
		} while (!unit->hasPath(BWAPI::Position(buildPosition)) && buildPosition != TilePosition(0, 0));
	}
	if (countTryToFind == 10)
	{
		Broodwar->printf("cant find an expansion location");
		return TilePosition(0, 0);
	}
	return buildPosition;

}

bool BuidingManager::buildingExpand()
{
	UnitType buildingType = UnitTypes::Protoss_Nexus;
	TilePosition tilePositionFirst;
	Unit u = getBuildingExpandWorker();

	if (u)
	{
		Broodwar->printf("getBuildingExpandWorker");
	}

	if (BWAPI::Broodwar->self()->minerals() >= buildingType.mineralPrice() && BWAPI::Broodwar->self()->gas() >= buildingType.gasPrice())
	{
		if (buildingType.isResourceDepot())
		{

			nextExpansionLocation = getClosestBase(u);

			if (nextExpansionLocation == BWAPI::TilePosition(0, 0))
			{
				return false;
			}

			expanding = true;

			if (u)
			{
				if (expanding)
				{

					u->move(BWAPI::Position(nextExpansionLocation), false);


					expanding = false;
					if (placeSpecific(u, buildingType, nextExpansionLocation))
					{
						return true;
					}
				}

			}

			return false;


		}
	}
	return false;
}

BWAPI::TilePosition BuidingManager::getExpansionLocation()
{
	if (nextExpansionLocation != BWAPI::TilePosition(0, 0))
	{
		return nextExpansionLocation;
	}
	return TilePosition(0,0);
}

void BuidingManager::addExpansion(BWAPI::Unit expansion)
{

	expansions.insert(expansion);

}

BWAPI::Unit BuidingManager::getExpansion()
{
	for (Unit u : expansions)
	{
		return u;
	}
	return nullptr;
}

int BuidingManager::getSizeExpansion()
{
	return expansions.size();
}

void BuidingManager::removeExpansion(BWAPI::Unit expansion)
{
	expansions.erase(expansion);
}

BuidingManager::BuidingManager()
{
	//orderQueue = OrderQueue::getInstance();
}

BuidingManager::~BuidingManager()
{
}