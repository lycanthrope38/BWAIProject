﻿#include "ExampleAIModule.h"
#include "Collections.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;

BWTA::Region* home;

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
	// Print the map name.
	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	Broodwar << "The map is " << Broodwar->mapName().c_str() << "!" << std::endl;

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Check if this is a replay
	if (Broodwar->isReplay())
	{
		// Announce the players in the replay
		Broodwar << "The following players are in this replay:" << std::endl;

		// Iterate all the players in the game using a std:: iterator
		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
			// Only print the player if they are not an observer
			if (!p->isObserver())
				Broodwar << p->getName().c_str() << ", playing as " << p->getRace().c_str() << std::endl;
		}

	}
	else // if this is not a replay
	{
		// Retrieve you and your enemy's races. enemy() will just return the first enemy.
		// If you wish to deal with multiple enemies then you must use enemies().
		if (Broodwar->enemy()) // First make sure there is an enemy
			Broodwar << "The matchup is " << Broodwar->self()->getRace().c_str() << " vs "
			<< Broodwar->enemy()->getRace().c_str() << std::endl;
	}

	BWTA::readMap();
	analyzed = false;
	analysis_just_finished = false;

	//AnalyzeThread();
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, nullptr, 0, nullptr);

	pool = false;

	counterGateway = 0;

	supplyBuilderTemp = nullptr;

	armyOrder = new ArmyOrder(Broodwar->self());

	mainOrderQueue = OrderQueue();
	buildingManager = BuidingManager();

<<<<<<< HEAD

	mainOrderQueue.push(UnitTypes::Protoss_Gateway, OrderQueue::PRIORITY_HIGH);
	mainOrderQueue.push(UnitTypes::Protoss_Gateway, OrderQueue::PRIORITY_HIGH);



	mainOrderQueue.push(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Gateway, 100, OrderQueue::PRIORITY_NORMAL);
	
	
	mainOrderQueue.push(UnitTypes::Protoss_Cybernetics_Core, OrderQueue::PRIORITY_HIGH);

	mainOrderQueue.push(UnitTypes::Protoss_Assimilator, OrderQueue::PRIORITY_HIGH);

	mainOrderQueue.push(UnitTypes::Protoss_Forge, OrderQueue::PRIORITY_HIGH);


	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Photon_Cannon, OrderQueue::PRIORITY_NORMAL);
	

=======
	mainOrderQueue.push(UnitTypes::Protoss_Pylon, OrderQueue::PRIORITY_HIGH, 5);
	mainOrderQueue.push(UnitTypes::Protoss_Pylon, OrderQueue::PRIORITY_HIGH, 5);
	mainOrderQueue.push(UnitTypes::Protoss_Pylon, OrderQueue::PRIORITY_HIGH, 5);
	mainOrderQueue.push(UnitTypes::Protoss_Pylon, OrderQueue::PRIORITY_HIGH, 9);
	//mainOrderQueue.push(UnitTypes::Protoss_Pylon, OrderQueue::PRIORITY_HIGH, 15);

	mainOrderQueue.push(UnitTypes::Protoss_Gateway, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Gateway, OrderQueue::PRIORITY_NORMAL);
	mainOrderQueue.push(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Gateway, 100, OrderQueue::PRIORITY_NORMAL);
>>>>>>> master
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
	// Called once every game frame

	// Return if the game is a replay or is paused. Viết code ở bên dưới dòng này!
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

		if ((Broodwar->mapFileName() != "(4)Andromeda.scx"))
		{
			buildingManager.setCentre(TilePosition(home->getCenter()));
		}
	}

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	//Broodwar->drawTextScreen(200, 20, "Counter : %d %d", supplyCounter, supplyTotalCounter);
	Broodwar->drawTextScreen(200, 20, "FPS Counter : %d", Broodwar->getFrameCount());

	//số woker
	supplyCounter = Broodwar->self()->supplyUsed() / 2;
	// tổng unit
	supplyTotalCounter = Broodwar->self()->supplyTotal() / 2;
	//so supply con lai
	supplyAvailable = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();


	//cứ 13 frame sẽ xét hàm mua lính một lần để tránh lag
	if (Broodwar->getFrameCount() % 17 == 0)
	{
		//armyOrder->trainZealot();
		mainOrderQueue.execute();
	}

	if (scoutManager.getScout() != nullptr)
	{
		scoutManager.sendScout();
	}

	//cứ 7 frame sẽ xét việc xây nhà một lần để tránh lag
	if (Broodwar->getFrameCount() % 7 == 0)
	{
		// Iterate through all the units that we own
		for (auto &u : Broodwar->self()->getUnits())
		{
			// Ignore the unit if it no longer exists
			// Make sure to include this block when handling any Unit pointer!
			if (!u->exists())
				continue;

			// Ignore the unit if it has one of the following status ailments
			if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
				continue;

			// Ignore the unit if it is in one of the following states
			if (u->isLoaded() || !u->isPowered() || u->isStuck())
				continue;

			// Ignore the unit if it is incomplete or busy constructing
			if (!u->isCompleted() || u->isConstructing())
				continue;

			// Finally make the unit do some stuff!

			// If the unit is a worker unit
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
				{

					
					
					// Broodwar->sendText("isIdle" + u->isIdle());

					// Order workers carrying a resource to return them to the center,
					// otherwise find a mineral patch to harvest.
					if (u->isCarryingGas() || u->isCarryingMinerals())
					{
						u->returnCargo();
					}
					else if (!u->getPowerUp())  // The worker cannot harvest anything if it
					{                             // is carrying a powerup such as a flag
						// Harvest from the nearest mineral patch or gas refinery
						if (!u->gather(u->getClosestUnit(IsMineralField || IsRefinery)))
						{
							// If the call fails, then print the last error message
							Broodwar << Broodwar->getLastError() << std::endl;
						}
					} // closure: has no powerup
				} // closure: if idle

			}
			else if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
			{
				BWAPI::Broodwar->printf("Building Placement '%d %d %d %d' ", u->getTilePosition().x,u->getTilePosition().y,u->getPosition().x,u->getPosition().y);
				if (supplyBuilderTemp == nullptr){
					// Retrieve the supply provider type in the case that we have run out of supplies
					UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
					Unit supplyBuilder = u->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
						(IsIdle || IsGatheringMinerals) &&
						IsOwned);
					supplyBuilderTemp = supplyBuilder;
				}
				// Order the depot to construct more workers! But only when it is idle.
				if (u->isIdle() && !u->train(u->getType().getRace().getWorker()))
				{
					
					Position pos = u->getPosition();
					
					UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
					
				
						Unit supplyBuilder = u->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
							(IsIdle || IsGatheringMinerals) &&
							IsOwned);
						supplyBuilderTemp = supplyBuilder;
						// If a unit was found
						if (supplyBuilder)
						{
							if (supplyProviderType.isBuilding())
							{
								TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType,
									supplyBuilder->getTilePosition());
								//if (targetBuildLocation)
								//{
								//	BWAPI::Position shiftPositionX(20, 20);
								//	Position position = u->getPosition() + shiftPositionX;
								//	BWAPI::Broodwar->printf("Building Shilf '%d %d' ", position.x, position.y);
									Unit worker = buildingManager.getWorker();
								//	//supplyBuilder->build(supplyProviderType, targetBuildLocation);
								//
								//	buildingManager.moveWorker(worker, position);
									//buildingManager.placeExpansion(worker, supplyProviderType, TilePosition(position.x/32,position.y/32));
									buildingManager.placeBuilding(worker, supplyProviderType,targetBuildLocation);
								//}
							}
							else
							{
								// Train the supply provider (Overlord) if the provider is not a structure
								supplyBuilder->train(supplyProviderType);
							}
						} // closure: supplyBuilder is valid
					
				
				
				} // closure: failed to train idle unit
			}
		} // closure: unit iterator
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
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			BWAPI::Broodwar->printf("onUnitCreate '%d %d %d %d' ", unit->getTilePosition().x, unit->getTilePosition().y, unit->getPosition().x, unit->getPosition().y);
		}
	}
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
	mainOrderQueue.push(unit->getType(), OrderQueue::PRIORITY_HIGH);
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds,
				unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
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
}