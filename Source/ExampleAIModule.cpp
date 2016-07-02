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
static int lastCheckedExpand = 0;

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

	workerManager = WorkerManager::newInstance();


	for (Unit i : Broodwar->self()->getUnits())
	{
		if (i->getType().isWorker())
		{
			//workerManager->makeAvailable(i);
			workerManager->addWorkerMinerals(i);
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


	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;


	supplyCounter = Broodwar->self()->supplyUsed() / 2;

	supplyTotalCounter = Broodwar->self()->supplyTotal() / 2;

	supplyAvailable = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();

	workerManager->tranferWorker();





	if (lastCheckedExpand + 500 < Broodwar->getFrameCount())
		{
			

			if (Broodwar->getFrameCount() > 10000 && staticOrderQueue->isEmpty() && buildingManager->getSizeExpansion() < 2)
			{
				lastCheckedExpand = Broodwar->getFrameCount();

				Broodwar->printf("buildingExpand buildingExpand buildingExpand");
				//buildingManager->getBuildingWorker()
				if (buildingManager->buildingExpand())
				{
				
				}
				
			}
}




	if (Broodwar->getFrameCount() % 13 == 0)
	{ 
		armyOrder->checkAndTrain();
		if (staticOrderQueue->isEmpty())
			mainOrderQueue->execute();
		else
			staticOrderQueue->execute();
	}

	if (ScoutManager::getInstance().getScout() != nullptr)
	{
		ScoutManager::getInstance().sendScout();

	}

	if (Broodwar->getFrameCount() >= 3000)
	{
		workerManager->handlerNumberWorker();
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


				if (supplyCounter >= 8 && supplyCounter <= 12)
				{
					if (ScoutManager::getInstance().getScout() == nullptr&&u != buildingManager->getBuildingWorker())

						ScoutManager::getInstance().setScout(u);
				}

				if (buildingManager->getWorkerCount() <= 2)
				{
					buildingManager->makeAvailableBuildingWorker(u);
				}


				if (u->isIdle())
				{
					if (u->isCarryingGas() || u->isCarryingMinerals())
					{
						u->returnCargo();
					}
					else if (!u->getPowerUp() && u != buildingManager->getBuildingWorker())
					{

					}
				}

			}
			else if (u->getType().isResourceDepot())
			{

				if ((supplyCounter == supplyTotalCounter) && staticOrderQueue->isEmpty())
				{

					Error lastErr = Broodwar->getLastError();


					UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
					static int lastChecked = 0;

					if (lastErr == Errors::Insufficient_Supply &&
						lastChecked + 400 < Broodwar->getFrameCount() &&
						Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
					{
						lastChecked = Broodwar->getFrameCount();

						Unit worker = buildingManager->getBuildingWorker();
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
	}
}


void ExampleAIModule::onSendText(std::string text)
{

	Broodwar->sendText("%s", text.c_str());
}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{

	Broodwar << player->getName().c_str() << " said \"" << text.c_str() << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{

	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{


	if (target)
	{

		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{

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

	Collections::lastBuildSuccess = Broodwar->getFrameCount();

	Broodwar->printf("isAssimilatorBuilt '%d' '%d'", mainOrderQueue->isAssimilatorBuilt ? 0 : 1, stopTraining ? 0 : 1);
	if (unit->getPlayer() == Broodwar->self())
	{

		if (unit->getType().isWorker())
		{

			workerManager->handlerAddWorker(unit);
			/*if (mainOrderQueue->isAssimilatorBuilt)
			{
			Broodwar->printf("onUnitCreate");
			workerManager->addWorkerGas(unit);
			return;
			}
			workerManager->addWorkerMinerals(unit);
			*/



			//	workerManager->addWorkerMinerals(unit);

			//workerManager->makeAvailable(unit);
		}
		else if (unit->getType() == UnitTypes::Protoss_Nexus)
		{
			buildingManager->addExpansion(unit);
		}
	}




}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding())
	{
		Unit worker = buildingManager->getBuildingWorker();
		mainOrderQueue->build(unit->getType());

	}
	if (unit->getType().isResourceDepot())
	{
		buildingManager->removeExpansion(unit);
		buildingManager->buildingExpand();
	}

	buildingManager->removeBuildingWorker(unit);

	UnitType uType = unit->getType();

	if (unit->getType().isWorker())
	{
		workerManager->removeWorker(unit);
	}

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

	//Broodwar->printf("onUnitComplete");
	if (unit->getPlayer() == Broodwar->self())
	{

		if (unit->getType().isWorker())
		{
			workerManager->handlerAddWorker(unit);
			/*if (mainOrderQueue->isAssimilatorBuilt)
			{
			workerManager->addWorkerGas(unit);
			return;
			}
			workerManager->addWorkerMinerals(unit);
			*/
			//workerManager->makeAvailable(unit);
		}
		else if (unit->getType() == UnitTypes::Protoss_Nexus)
		{
			buildingManager->addExpansion(unit);
		}
		if (unit->getType().isBuilding()){
			if (unit->getType() == UnitTypes::Protoss_Gateway)
				unit->setRallyPoint(((unit->getRegion())->getClosestAccessibleRegion())->getCenter());
		}
		//thêm các unit lính
		if (!(unit->getType().isBuilding()) && !(unit->getType().isWorker()) && !(unit->getType().isNeutral())){
			LordCommander::getInstance()->addUnit(unit);
		}
	}


}
