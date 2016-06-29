#include "BuidingManager.h"

using namespace BWAPI;

#define SPIRALLIMIT 200
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
	else if (!approxLocation.isValid())
	{
		approxLocation = approxLocation.makeValid();
	}

	buildPosition = approxLocation;
	while (!builder->build(building, buildPosition))
	{
		do{
			//check the location of the closest mineralsPrepare - continue to increment position without /
			//attempting to construct until the mineralsPrepare are no longer too close
			//This is to avoid constructing structures in mineral lines
			BWAPI::Unit closestCentre = NULL;

			for (Unit m : Broodwar->getMinerals())
			{
				if (closestCentre == NULL || buildPosition.getDistance((m)->getTilePosition()) < buildPosition.getDistance(closestCentre->getTilePosition()))
					closestCentre = m;
			}
			if (closestCentre!=NULL)
			{
				if (buildPosition.getDistance(approxLocation) < MINERALDIST)
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
	if (availableBuildingWorkers.size()==0)
	{
		int count = 0;
		for (Unit i : Broodwar->self()->getUnits())
		{
			if (i->getType().isWorker())
			{
				makeAvailableBuildingWorker(i);
				count += 1;
				if (count==3)
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
	double minDist = 0;
	bool token;

	//Broodwar->printf("getClosestBase,getClosestBase,getClosestBase '%d'",expansions.size());

	for (std::set<BWTA::BaseLocation*>::const_iterator i = BWTA::getBaseLocations().begin(); i != BWTA::getBaseLocations().end(); i++)
	{
		token = false;
		for (BWAPI::Unit j : expansions)
		{
			//Broodwar->printf("j : '%d %d' vs i '%d %d'", j->getPosition().x, j->getPosition().y, (*i)->getPosition().x, (*i)->getPosition().y);

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
	if (buildPosition == TilePosition(0, 0))
	{
		Broodwar->printf("cant find new expansion location");
	}
	return buildPosition;
}

/*
returns the second closest base, if the closest is unavailable for some reason
*/
BWAPI::TilePosition BuidingManager::getNextClosestBase(BWAPI::Unit unit,BWAPI::TilePosition tilePosition)
{
	BWAPI::TilePosition buildPosition = TilePosition(0, 0);
	double minDist = 0;
	bool token;

	for (std::set<BWTA::BaseLocation*>::const_iterator i = BWTA::getBaseLocations().begin(); i != BWTA::getBaseLocations().end(); i++)
	{
		if ((*i)->getTilePosition()!=tilePosition)
		{
			token = false;
			for (Unit j : expansions)
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
	if (buildPosition == TilePosition(0, 0))
	{
		Broodwar->printf("cant find new expansion location");
	}
	return buildPosition;
}

bool BuidingManager::buildingExpand()
{
	UnitType buildingType = UnitTypes::Protoss_Nexus;
	TilePosition tilePositionFirst;
	for (BWAPI::Unit u : BWAPI::Broodwar->self()->getUnits()){
		if (u->getType().isWorker() && u->isIdle()){
			if (BWAPI::Broodwar->self()->minerals() >= buildingType.mineralPrice() && BWAPI::Broodwar->self()->gas() >= buildingType.gasPrice())
			{
				if (buildingType.isResourceDepot())
				{

					nextExpansionLocation = getClosestBase(u);
					tilePositionFirst = nextExpansionLocation;
					//	BWAPI::Broodwar->printf("nextExpansionLocationAfter : '%d' '%d'", nextExpansionLocation.x, nextExpansionLocation.y);
					if (nextExpansionLocation == BWAPI::TilePosition(0, 0))
					{
						return false;
					}
					//if we can't reach the closest base (e.g. its on an island)
					else if (!u->hasPath(BWAPI::Position(nextExpansionLocation)))
					{
						//get the next closest
						nextExpansionLocation = getNextClosestBase(u, tilePositionFirst);
						//if the next closest is non-existent or also unreachable then give up and move on to the next build order item
						if ((nextExpansionLocation == BWAPI::TilePosition(0, 0)) || !u->hasPath(BWAPI::Position(nextExpansionLocation)))
						{
							return false;
						}

					}
					expanding = true;

					if (u)
					{
						if (expanding && nextExpansionLocation != BWAPI::TilePosition(0, 0))
						{
							//BWAPI::Broodwar->printf("MOVE nextExpansionLocation : '%d' '%d'", nextExpansionLocation.x, nextExpansionLocation.y);

							u->move(BWAPI::Position(nextExpansionLocation), false);


							expanding = false;
							if (placeExpansion(u, buildingType, nextExpansionLocation))
							{
								return true;
							}
						}

					}

					return false;


				}
			}
		}
	}
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
}


BuidingManager::~BuidingManager()
{
}
