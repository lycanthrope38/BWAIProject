#include "ExampleAIModule.h"
#include "Collections.h"
#define TOTAL_ATTACK_SCORE 25000
using namespace BWAPI;
using namespace Filter;



bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;

bool stopTraining;
bool isHavingExpand;
static int lastCheckedExpand = 0;

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

	isInitPostion = false;
	increaseTroop = 0;
}

void ExampleAIModule::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}

void ExampleAIModule::onInit(){
	for (Unit u : Broodwar->self()->getUnits()){
		if (u->getType().isBuilding())
		{
			Collections::basePosition = u->getPosition();
			int tmpDistance = 9999999999;
			//left
			if (Collections::basePosition.x - 0 < tmpDistance){
				tmpDistance = Collections::basePosition.x - 0;
				Collections::rootBuildPosition = Position(Collections::basePosition.x + 250, Collections::basePosition.y);
			}
			//right
			if (Broodwar->mapWidth() - Collections::basePosition.x < tmpDistance){
				tmpDistance = Broodwar->mapWidth() - Collections::basePosition.x;
				Collections::rootBuildPosition = Position(Collections::basePosition.x - 250, Collections::basePosition.y);
			}
			//top
			if (Collections::basePosition.y - 0 < tmpDistance){
				tmpDistance = Collections::basePosition.y;
				Collections::rootBuildPosition = Position(Collections::basePosition.x, Collections::basePosition.y + 250);
			}
			//bottom
			if (Broodwar->mapHeight() - Collections::basePosition.y < tmpDistance){
				tmpDistance = Broodwar->mapHeight() - Collections::basePosition.y;
				Collections::rootBuildPosition = Position(Collections::basePosition.x, Collections::basePosition.y - 250);
			}

			isInitPostion = true;	armyOrder = new ArmyOrder(Broodwar->self());

			staticOrderQueue = StaticOrder::getInstance();
			mainOrderQueue = OrderQueue::getInstance();
			buildingManager = BuidingManager::newInstance();
			workerManager = WorkerManager::newInstance();
			jonSnow = LordCommander::getInstance();
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
			break;
		}
	}
}

void ExampleAIModule::onFrame()
{
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	if (!isInitPostion){
		onInit();
	}
	//Broodwar->sendText("Base position x %d y %d", Collections::basePosition.x, Collections::basePosition.y);
	Broodwar->drawBoxMap(Position(Collections::rootBuildPosition.x - 5, Collections::rootBuildPosition.y - 5),
		Position(Collections::rootBuildPosition.x + 5, Collections::rootBuildPosition.y + 5), Colors::Red);
	TilePosition tmpTilePosition = TilePosition(StaticOrder::getInstance()->getFirst().getApproxPos());
	Position tmpPosition = Position(tmpTilePosition);
	Broodwar->drawBoxMap(Position(tmpPosition.x - 5, tmpPosition.y - 5), Position(tmpPosition.x + 5, tmpPosition.y + 5), Colors::Green);
	/*if (StaticOrder::getInstance()->getFirst().isWithPosition())
		Broodwar->sendText("Tile : x %d y %d Position: x %d y %d", tmpTilePosition.x, tmpTilePosition.y, tmpPosition.x, tmpPosition.y);
		*/
	if (analyzed)
	{
		drawTerrainData();
		if (Broodwar->getFrameCount() % 1001 == 0){
			jonSnow->checkAttack();
		}
	}

	if (analysis_just_finished)
	{
		Broodwar << "Finished analyzing map." << std::endl;
		analysis_just_finished = false;
		if ((Broodwar->self()->getRace() != BWAPI::Races::Zerg) && (Broodwar->mapFileName() != "(4)Andromeda.scx"))
		{
			buildingManager->setCentre(TilePosition(home->getCenter()));
		}
	}

	jonSnow->onFrame();

	if (Broodwar->getFrameCount() > 15000)
		if (Broodwar->self()->minerals() > 2000)
			staticOrderQueue->spamStaticDefenseMode();

	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	supplyCounter = Broodwar->self()->supplyUsed() / 2;

	supplyTotalCounter = Broodwar->self()->supplyTotal() / 2;

	supplyAvailable = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();

	workerManager->tranferWorker();

	if (lastCheckedExpand + 300 < Broodwar->getFrameCount())
	{
		if (Broodwar->getFrameCount() > 1200 && shouldExpandNow())
			if (Collections::lastExpansion + 10000 < Broodwar->getFrameCount())
			{
				lastCheckedExpand = Broodwar->getFrameCount();

				Broodwar->printf("buildingExpand buildingExpand buildingExpand");
				//buildingManager->getBuildingWorker()
				if (buildingManager->buildingExpand())
				{
					isHavingExpand = true;
					//int checkFrame = Broodwar->getFrameCount();

				}
			}
	}

	if (isHavingExpand && lastCheckedExpand < Broodwar->getFrameCount())
	{
		buildingManager->createBuilding(buildingManager->getBuildingExpandWorker(), UnitTypes::Protoss_Pylon);
		mainOrderQueue->push(Broodwar->self()->getRace().getSupplyProvider(), Position(buildingManager->getExpansionLocation()), mainOrderQueue->PRIORITY_HIGH);
		mainOrderQueue->push(Broodwar->self()->getRace().getRefinery(), Position(buildingManager->getExpansionLocation()), mainOrderQueue->PRIORITY_HIGH);
		if (Collections::getStaticDefenseStructure(Broodwar->self()->getRace()) != UnitTypes::None){
			mainOrderQueue->push(UnitTypes::Protoss_Photon_Cannon, Position(buildingManager->getExpansionLocation()), mainOrderQueue->PRIORITY_NORMAL);
			mainOrderQueue->push(UnitTypes::Protoss_Photon_Cannon, Position(buildingManager->getExpansionLocation()), mainOrderQueue->PRIORITY_NORMAL);
			mainOrderQueue->push(UnitTypes::Protoss_Photon_Cannon, Position(buildingManager->getExpansionLocation()), mainOrderQueue->PRIORITY_NORMAL);
			mainOrderQueue->push(UnitTypes::Protoss_Photon_Cannon, Position(buildingManager->getExpansionLocation()), mainOrderQueue->PRIORITY_NORMAL);
			mainOrderQueue->push(UnitTypes::Protoss_Photon_Cannon, Position(buildingManager->getExpansionLocation()), mainOrderQueue->PRIORITY_NORMAL);
		}
		isHavingExpand = false;
	}

	//Broodwar->sendText("Build in row %d train in row %d ", Collections::buildInRow, Collections::trainInRow);
	if (Broodwar->getFrameCount() % 7 == 0)
	{

		mainOrderQueue->executeDeath();
		if (!staticOrderQueue->isEmpty()){
			if (Collections::buildInRow >= 4 || (Collections::buildInRow == 0 && Collections::trainInRow < 3 && Collections::trainInRow != 0))
				armyOrder->checkAndTrain();
			else
				staticOrderQueue->execute();
		}
		else{
			armyOrder->checkAndTrain();
			mainOrderQueue->executeDeath();
			if (staticOrderQueue->isEmpty())
				mainOrderQueue->execute();
			else
				staticOrderQueue->execute();
		}
	}

	if (Broodwar->getFrameCount() % 700 == 0){
		increaseTroop += 0.25;
		Collections::currentTroopCycle = 0;
		if (Broodwar->getFrameCount() > 4500)
			Collections::limitTroopFor700Frame += increaseTroop;
	}

	if (Broodwar->getFrameCount() % 500 == 0)
		changeDefensePoint();

	if (ScoutManager::getInstance().getScout() != nullptr)
	{
		ScoutManager::getInstance().sendScout();

	}

	if (Collections::lastBuildCall > Collections::lastBuildSuccess)
		if (Broodwar->getFrameCount() - Collections::lastBuildCall > 200)
			Collections::lastBuildSuccess = Collections::lastBuildCall + 5;

	if (Broodwar->getFrameCount() >= 3000)
	{
		workerManager->handlerNumberWorker();
	}

	if (buildingManager->getBuildingWorker() != nullptr)
		Broodwar->drawBoxMap(Position(buildingManager->getBuildingWorker()->getPosition().x - 10, buildingManager->getBuildingWorker()->getPosition().y - 10),
		Position(buildingManager->getBuildingWorker()->getPosition().x + 10, buildingManager->getBuildingWorker()->getPosition().y + 10), Colors::Orange);

	if (buildingManager->getBuildingExpandWorker() != nullptr)
		Broodwar->drawBoxMap(Position(buildingManager->getBuildingExpandWorker()->getPosition().x - 10, buildingManager->getBuildingExpandWorker()->getPosition().y - 10),
		Position(buildingManager->getBuildingExpandWorker()->getPosition().x + 10, buildingManager->getBuildingExpandWorker()->getPosition().y + 10), Colors::Orange);

	if (Broodwar->getFrameCount() % 7 == 0)
	{

		if (Broodwar->getFrameCount() > 15000)
			if (supplyTotalCounter - supplyCounter < 6)
				if (Broodwar->getFrameCount() - Collections::lastBuildPylonRequest > 200){
					buildingManager->createBuilding(buildingManager->getBuildingWorker(), Broodwar->self()->getRace().getSupplyProvider());
				}

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


				if (supplyCounter > 8 && supplyCounter < 12)
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

bool ExampleAIModule::shouldExpandNow(){
	if (buildingManager->getSizeExpansion() == 0)
		return true;
	if (workerManager->getIdleCount() >= 4)
		return true;
	if (Broodwar->getFrameCount() - Collections::lastExpansion >= 10000)
		return true;
	//kiểm tra mỏ
	return false;
}

void ExampleAIModule::onSendText(std::string text)
{

	if (!isInitPostion){
		onInit();
	}
	Broodwar->sendText("%s", text.c_str());
}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{

	if (!isInitPostion){
		onInit();
	}
	Broodwar << player->getName().c_str() << " said \"" << text.c_str() << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{

	if (!isInitPostion){
		onInit();
	}
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

	if (!isInitPostion){
		onInit();
	}

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

	if (!isInitPostion){
		onInit();
	}

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
		if (unit->exists() && unit->getType().isBuilding()){
			Collections::lastBuildSuccess = Broodwar->getFrameCount();
			Broodwar->sendText("building done");
		}

	}




}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding())
	{
		mainOrderQueue->pushDeath(unit->getType(), unit->getPosition());
		Broodwar->sendText("added to death queue");
	}
	/*if (unit->getType().isResourceDepot())
	{
	buildingManager->removeExpansion(unit);
	buildingManager->buildingExpand();
	}*/

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

	/*if (unit->getType() == Broodwar->self()->getRace().getWorker().whatBuilds().first)
		Collections::lastExpansion -= 7000;*/

}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding() && unit->exists())
		Collections::lastBuildSuccess = Broodwar->getFrameCount();

	if (!(unit->getPlayer() == Broodwar->self()))
	{
		ScoutManager::getInstance().addEnemyBase(unit);
	}
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
	LordCommander::getInstance()->removeDeadUnit(unit);
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName.c_str() << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{

	if (!isInitPostion){
		onInit();
	}
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

		//thêm các unit lính
		if (!(unit->getType().isBuilding() || unit->getType().isWorker() || unit->getType().isNeutral() || unit->getType() == UnitTypes::Protoss_Interceptor)){
			LordCommander::getInstance()->addUnit(unit);
		}
		if (unit->getType() == UnitTypes::Protoss_Nexus)
			Collections::lastExpansion = Broodwar->getFrameCount();

	}


}

void ExampleAIModule::changeDefensePoint(){
	Unitset uset = BWAPI::Broodwar->self()->getUnits();
	UnitType resourceDepot;
	resourceDepot = BWAPI::Broodwar->self()->getRace().getWorker().whatBuilds().first;
	for (Unit u : uset)
		if (u->getType() == resourceDepot){
			if (u->getUnitsInRadius(200, Filter::IsWorker).size() > 5){
				if (u->getPosition().x < Broodwar->mapWidth() - u->getPosition().x)
					Collections::defensePosition = Position(u->getPosition().x - 30, Collections::defensePosition.y);
				else
					Collections::defensePosition = Position(u->getPosition().x + 30, Collections::defensePosition.y);
			}
		}
	LordCommander::getInstance()->reform();
}