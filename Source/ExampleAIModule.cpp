#include "ExampleAIModule.h"
#include "Collections.h"
#define TOTAL_ATTACK_SCORE 7000
using namespace BWAPI;
using namespace Filter;



bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;

bool stopTraining;
bool isHavingExpand;


Position ExampleAIModule::basePostion = Positions::None;

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


	stopTraining = false;

	supplyBuilderTemp = nullptr;

	armyOrder = new ArmyOrder(Broodwar->self());

	mainOrderQueue = OrderQueue::getInstance();
	//test đẩy 20 Zealot vào hàng đợi


	buildingManager = BuidingManager::newInstance();

	workerManager = WorkerManager();


	for (Unit i : Broodwar->self()->getUnits())
	{
		if (i->getType().isWorker())
		{
			workerManager.makeAvailable(i);
		}
		else if (i->getType() == UnitTypes::Protoss_Nexus)
		{
			buildingManager->addExpansion(i);
		}

	}
	
	isInitPostion = false;
	staticOrderQueue = StaticOrder::getInstance();
	jonSnow = LordCommander::getInstance();
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

	if (!isInitPostion){
		for (Unit u : Broodwar->self()->getUnits()){
			if (u->getType().isBuilding())
			{
				basePostion = u->getPosition();
				break;
			}
		}
	}

	//BWTA draw
	if (analyzed)
	{
		drawTerrainData();
		if (jonSnow->getSelfScore() > TOTAL_ATTACK_SCORE)
			jonSnow->totalAttack(ScoutManager::getInstance().getEnemyBase());
	}

	if (analysis_just_finished)
	{
		Broodwar << "Finished analyzing map." << std::endl;;
		analysis_just_finished = false;
		if ((Broodwar->self()->getRace() != BWAPI::Races::Zerg) && (Broodwar->mapFileName() != "(4)Andromeda.scx"))
		{
			buildingManager->setCentre(TilePosition(home->getCenter()));
		}
	}

	jonSnow->onFrame();

	
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

	if (workerManager.getNumMineralWorkers() <= (workerManager.limitWorker - 2))
	{
		workerManager.gatherMineral();
	}
	else
	{
		stopTraining = true;
	}

	if (mainOrderQueue->isAssimilatorBuilt)
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
	if (buildingManager->getSizeExpansion()<2)
	{

		buildingManager->buildingExpand();

	}
	//cứ 13 frame sẽ xét hàm mua lính một lần để tránh lag
	if (Broodwar->getFrameCount() % 17 == 0)
	{
		//nếu các order lúc đầu game chưa thực hiện xong thì sẽ ưu tiên thực hiện chúng trước
		if (staticOrderQueue->isEmpty())
			mainOrderQueue->execute();
		else
			staticOrderQueue->execute();
	}

	if (ScoutManager::getInstance().getScout() != nullptr)
	{
		ScoutManager::getInstance().sendScout();
		//ScoutManager::getInstance().scoutExpos();
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


			/*	if (StaticOrder::isBuildedGas)
					if (workerManager.getNumGasWorkers() < 3)
						if ((Broodwar->getFrameCount() - StaticOrder::buildedGasFrame) > (Broodwar->self()->getRace().getRefinery().buildTime() + 5))
							workerManager.gatherGas(u, u->getClosestUnit(BWAPI::Filter::IsRefinery));
*/
				if (supplyCounter == 8)
				{
					if (ScoutManager::getInstance().getScout() == nullptr&&u!=buildingManager->getWorker())
					
						ScoutManager::getInstance().setScout(u);
				}

				if (buildingManager->getWorkerCount() <= 2)
				{
					buildingManager->makeAvailable(u);
				}

				if (u->isIdle())
				{
					if (u->isCarryingGas() || u->isCarryingMinerals())
					{
						u->returnCargo();
					}
					else if (!u->getPowerUp() && u != buildingManager->getWorker())
					{

						if (!u->gather(u->getClosestUnit(IsMineralField)))
						{

							Broodwar << Broodwar->getLastError() << std::endl;
						}
					}
				} // closure: if idle

			}
			else if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
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

							Unit worker = buildingManager->getWorker();
							if (worker)
							{

								if (supplyProviderType.isBuilding())
								{

									buildingManager->createBuilding(worker, supplyProviderType);
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

							Unit worker = buildingManager->getWorker();
							if (worker)
							{

								if (supplyProviderType.isBuilding())
								{
									buildingManager->createBuilding(worker, supplyProviderType);
								}

							}
						}

					}
				}
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
	if ((unit->getPlayer()->isEnemy(Broodwar->self())) && (unit->getType().isBuilding()))
	{
		ScoutManager::getInstance().addEnemyBase(unit);
	}
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
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds,
				unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}




}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding())
	{
		Unit worker = buildingManager->getWorker();
		mainOrderQueue->build(unit->getType());

	}
	if (unit->getType().isResourceDepot())
	{
		buildingManager->removeExpansion(unit);
		buildingManager->buildingExpand();
	}

	UnitType uType = unit->getType();


	if (!(uType.isBuilding()) && !(uType.isWorker() && !(uType.isNeutral())))
	{
		LordCommander::getInstance()->removeDeadUnit(unit);
	}

}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
	if (!(unit->getPlayer() == Broodwar->self()))
	{
		ScoutManager::getInstance().addEnemyBase(unit);
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

		if (unit->getType().isWorker())
		{
			workerManager.makeAvailable(unit);
		}
		else if (unit->getType() == UnitTypes::Protoss_Nexus)
		{
			buildingManager->addExpansion(unit);
		}
		//thêm các unit lính
		if (!(unit->getType().isBuilding()) && !(unit->getType().isWorker()) && !(unit->getType().isNeutral())){
			LordCommander::getInstance()->addUnit(unit);
		}
	}


}

