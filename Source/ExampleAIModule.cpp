#include "ExampleAIModule.h"
#include "Collections.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;

bool stopTraining ;
bool isHavingExpand;


DWORD WINAPI AnalyzeThread()
{
	BWTA::analyze();

	if (BWTA::getStartLocation(BWAPI::Broodwar->self()) != NULL)	
	{
		home = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
	}
	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

void ExampleAIModule::drawTerrainData()
{
	//we will iterate through all the base locations, and draw their outlines.
	for (const auto& baseLocation : BWTA::getBaseLocations())
	{
		TilePosition p = baseLocation->getTilePosition();

		//draw outline of center location
		Position leftTop(p.x * TILE_SIZE, p.y * TILE_SIZE);
		Position rightBottom(leftTop.x + 4 * TILE_SIZE, leftTop.y + 3 * TILE_SIZE);
		Broodwar->drawBoxMap(leftTop, rightBottom, Colors::Blue);

		//draw a circle at each mineral patch
		for (const auto& mineral : baseLocation->getStaticMinerals())
		{
			Broodwar->drawCircleMap(mineral->getInitialPosition(), 30, Colors::Cyan);
		}

		//draw the outlines of Vespene geysers
		for (const auto& geyser : baseLocation->getGeysers())
		{
			TilePosition p1 = geyser->getInitialTilePosition();
			Position leftTop1(p1.x * TILE_SIZE, p1.y * TILE_SIZE);
			Position rightBottom1(leftTop1.x + 4 * TILE_SIZE, leftTop1.y + 2 * TILE_SIZE);
			Broodwar->drawBoxMap(leftTop1, rightBottom1, Colors::Orange);
		}

		//if this is an island expansion, draw a yellow circle around the base location
		if (baseLocation->isIsland())
		{
			Broodwar->drawCircleMap(baseLocation->getPosition(), 80, Colors::Yellow);
		}
	}

	//we will iterate through all the regions and ...
	for (const auto& region : BWTA::getRegions())
	{
		// draw the polygon outline of it in green
		BWTA::Polygon p = region->getPolygon();
		for (size_t j = 0; j < p.size(); ++j)
		{
			Position point1 = p[j];
			Position point2 = p[(j + 1) % p.size()];
			Broodwar->drawLineMap(point1, point2, Colors::Green);
		}
		// visualize the chokepoints with red lines
		for (auto const& chokepoint : region->getChokepoints())
		{
			Position point1 = chokepoint->getSides().first;
			Position point2 = chokepoint->getSides().second;
			Broodwar->drawLineMap(point1, point2, Colors::Red);
		}
	}
}

void ExampleAIModule::onStart()
{
	
	Broodwar << "The map is " << Broodwar->mapName().c_str() << "!" << std::endl;

	Broodwar->enableFlag(Flag::UserInput);

	Broodwar->setCommandOptimizationLevel(2);

	
	if (Broodwar->isReplay())
	{
	
		Broodwar << "The following players are in this replay:" << std::endl;

		
		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
		
			if (!p->isObserver())
				Broodwar << p->getName().c_str() << ", playing as " << p->getRace().c_str() << std::endl;
		}

	}
	else 
	{
	
		if (Broodwar->enemy()) 
			Broodwar << "The matchup is " << Broodwar->self()->getRace().c_str() << " vs "
			<< Broodwar->enemy()->getRace().c_str() << std::endl;
	}

	BWTA::readMap();
	analyzed = false;
	analysis_just_finished = false;

	//AnalyzeThread();
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, nullptr, 0, nullptr);

	pool = false;

	supplyBuilderTemp = nullptr;

	armyOrder = new ArmyOrder(Broodwar->self());

	mainOrderQueue = OrderQueue();
	//test đẩy 20 Zealot vào hàng đợi

	buildingManager = BuidingManager();

	workerManager = WorkerManager();

	mainOrderQueue.isAssimilatorBuilt = false;

	stopTraining = false;
	isHavingExpand = false;

	for (Unit i : Broodwar->self()->getUnits())
	{
		if (i->getType().isWorker())
		{
			workerManager.makeAvailable(i);
		}
		else if (i->getType() == UnitTypes::Protoss_Nexus)
		{
			buildingManager.addExpansion(i);
		}
			
	}

	mainOrderQueue.push(UnitTypes::Protoss_Gateway, OrderQueue::PRIORITY_HIGH);
	mainOrderQueue.push(UnitTypes::Protoss_Gateway, OrderQueue::PRIORITY_HIGH);

	/*mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
*/
	/*mainOrderQueue.push(UnitTypes::Protoss_Assimilator, OrderQueue::PRIORITY_HIGH,12);
	mainOrderQueue.push(UnitTypes::Protoss_Forge, OrderQueue::PRIORITY_HIGH);
*/
	//mainOrderQueue.push(UnitTypes::Protoss_Nexus, OrderQueue::PRIORITY_HIGH);


	/*mainOrderQueue.push(UnitTypes::Protoss_Cybernetics_Core, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Citadel_of_Adun, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Templar_Archives, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Robotics_Facility, OrderQueue::PRIORITY_NORMAL);
	*/
	mainOrderQueue.push(UnitTypes::Protoss_Assimilator, OrderQueue::PRIORITY_HIGH);
	mainOrderQueue.push(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Gateway, 100, OrderQueue::PRIORITY_NORMAL);
}

void ExampleAIModule::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}

void ExampleAIModule::onFrame()
{
	
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	//BWTA draw
	if (analyzed)
	{
		drawTerrainData();
	}

	if (analysis_just_finished)
	{
		Broodwar << "Finished analyzing map." << std::endl;;
		analysis_just_finished = false;
		if ((Broodwar->self()->getRace() != BWAPI::Races::Zerg) )
		{
			buildingManager.setCentre(TilePosition(home->getCenter()));
		}
	}

	
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	
	supplyCounter = Broodwar->self()->supplyUsed() / 2;
	
	supplyTotalCounter = Broodwar->self()->supplyTotal() / 2;
	
	supplyAvailable = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();

	if (workerManager.getNumMineralWorkers() <= (workerManager.limitWorker-2))
	{
		workerManager.gatherMineral();
	}
	else
	{
		stopTraining = true;
	}

	if (mainOrderQueue.isAssimilatorBuilt)
	{
		stopTraining = false;
		if (workerManager.getNumMineralWorkers() <= workerManager.limitWorker)
		{
			workerManager.gatherMineral();
		}
		else
		{
			if (!stopTraining)
			{
				workerManager.gatherGas();
				if (workerManager.getNumGasWorkers() >= 3)
				{
					stopTraining = true;
				}
			}
			
		}
	}
	
	if (buildingManager.getSizeExpansion()<2)
	{
		

		/*if (createNexus())
		{
			Broodwar->printf("createNexus createNexus createNexus '%d'",isHavingExpand?1:0);
			isHavingExpand = true;
		}*/
		createNexus();
		
	}
	

	if (Broodwar->getFrameCount() % 17 == 0)
	{
		mainOrderQueue.execute();
	}

	
	if (Broodwar->getFrameCount() % 7 == 0)
	{	
				
		for (auto &u : Broodwar->self()->getUnits())
		{
			if (!u->exists())
				continue;

			
			if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
				continue;

			
			if (u->isLoaded() || !u->isPowered() || u->isStuck())
				continue;

			
			if (!u->isCompleted() || u->isConstructing())
				continue;

			
			if (u->getType().isWorker())
			{
				if (supplyCounter == 8)
				{
					if (scoutManager.getScout() == nullptr)
					{
						scoutManager.setScout(u);
					}
				}
			
				
				if (buildingManager.getWorkerCount() <= 2)
				{
				
					buildingManager.makeAvailable(u);
				}
				

				if (u->isIdle())
				
					if (u->isCarryingGas() || u->isCarryingMinerals())
					{
						u->returnCargo();
					}
					else if (!u->getPowerUp()&&u!=workerManager.getWorker())  
					{                             
						
						if (!u->gather(u->getClosestUnit(IsMineralField)))
						{
							
							Broodwar << Broodwar->getLastError() << std::endl;
						}
					}
			
				} 

			
			else if (u->getType().isResourceDepot()) 
			{
				
				if (!stopTraining)
				{
					if (u->isIdle() && !u->train(u->getType().getRace().getWorker()))
					{
						
						Position pos = u->getPosition();
						Error lastErr = Broodwar->getLastError();
						Broodwar->registerEvent([pos, lastErr](Game*){ Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
							nullptr,    
							Broodwar->getLatencyFrames());  

						UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
						static int lastChecked = 0;

						if (lastErr == Errors::Insufficient_Supply &&
							lastChecked + 400 < Broodwar->getFrameCount() &&
							Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
						{
							lastChecked = Broodwar->getFrameCount();
							
							Unit worker = buildingManager.getWorker();
							if (worker)
							{
								
								if (supplyProviderType.isBuilding())
								{
									
									buildingManager.createBuilding(worker, supplyProviderType);
								}
								else
								{
									worker->train(supplyProviderType);
								}
							} 
						} 
					} 
				}
				else
				{
					if (supplyCounter == supplyTotalCounter)
					{

						Error lastErr = Broodwar->getLastError();


						UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
						static int lastChecked = 0;

						if (lastErr == Errors::Insufficient_Supply &&
							lastChecked + 400 < Broodwar->getFrameCount() &&
							Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
						{
							lastChecked = Broodwar->getFrameCount();

							Unit worker = buildingManager.getWorker();
							if (worker)
							{

								if (supplyProviderType.isBuilding())
								{
									buildingManager.createBuilding(worker, supplyProviderType);
								}

							}
						}

					}
				}
			}
		} // closure: unit iterator
	}
}


bool ExampleAIModule::createNexus()
{
	UnitType buildingType = UnitTypes::Protoss_Nexus;

			if (BWAPI::Broodwar->self()->minerals() >= buildingType.mineralPrice() && BWAPI::Broodwar->self()->gas() >= buildingType.gasPrice())
			{
				if (buildingType.isResourceDepot())
				{

					nextExpansionLocation = buildingManager.getClosestBase(buildingManager.getWorker());
					BWAPI::Broodwar->printf("nextExpansionLocationAfter : '%d' '%d'", nextExpansionLocation.x, nextExpansionLocation.y);
					if (nextExpansionLocation == BWAPI::TilePosition(0, 0))
					{
						return false;
					}
					//if we can't reach the closest base (e.g. its on an island)
					else if (!buildingManager.getWorker()->hasPath(BWAPI::Position(nextExpansionLocation)))
					{
						//get the next closest
						nextExpansionLocation = buildingManager.getNextClosestBase(buildingManager.getWorker());
						//if the next closest is non-existent or also unreachable then give up and move on to the next build order item
						if ((nextExpansionLocation == BWAPI::TilePosition(0, 0)) || !buildingManager.getWorker()->hasPath(BWAPI::Position(nextExpansionLocation)))
						{
							return false;
						}

					}
					expanding = true;

					if (buildingManager.getWorker())
					{
						if (expanding && nextExpansionLocation != BWAPI::TilePosition(0, 0))
						{
							BWAPI::Broodwar->printf("MOVE nextExpansionLocation : '%d' '%d'", nextExpansionLocation.x, nextExpansionLocation.y);

							buildingManager.getWorker()->move(BWAPI::Position(nextExpansionLocation), false);


							expanding = false;
							if (buildingManager.placeExpansion(buildingManager.getWorker(), buildingType, nextExpansionLocation))
							{
								return true;
							}
						}

					}

					return false;


				}
			}
		
}


void ExampleAIModule::onSendText(std::string text)
{
	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());
}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName().c_str() << " said \"" << text.c_str() << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
	if ((unit->getPlayer()->isEnemy(Broodwar->self())) && (unit->getType().isBuilding()))
	{
		scoutManager.addEnemyBase(unit);
	}
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
		
	if ( Broodwar->getFrameCount() > 1)
	{
		if (unit->getPlayer() == Broodwar->self())
		{
			
			if (unit->getType().isWorker())
			{
				workerManager.makeAvailable(unit);
			}
			else if (unit->getType()==UnitTypes::Protoss_Nexus)
			{
				buildingManager.addExpansion(unit);
			}
		}
	}
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{

	if (unit->getType().isBuilding())
	{
		Unit worker = buildingManager.getWorker();
		mainOrderQueue.build(unit->getType());
	
	}
	if (unit->getType().isResourceDepot())
	{
		buildingManager.removeExpansion(unit);
		createNexus();
	}
	
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
	if (!(unit->getPlayer() == Broodwar->self()))
	{
		scoutManager.addEnemyBase(unit);
	}
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName.c_str() << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		
		if (unit->getType().isWorker() )
		{
			workerManager.makeAvailable(unit);
		}
		else if (unit->getType()==UnitTypes::Protoss_Nexus)
		{
			buildingManager.addExpansion(unit);
		}
		
	}
}