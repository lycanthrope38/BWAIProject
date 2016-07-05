#include "ArmyOrder.h"
#include <iostream>
#include <string> 
#include "Collections.h"
#include "OrderQueue.h"

using namespace BWAPI;
using namespace Filter;

ArmyOrder::ArmyOrder(BWAPI::Player main_self)
{
	self = main_self;
	if (main_self->getRace() == Races::Protoss)
		initProtoss();
	else if (main_self->getRace() == Races::Zerg)
		initZerg();
	else initTerran();
}

std::vector<Unit> ArmyOrder::getGatewayList(){
	std::vector<Unit> result;
	for (Unit u : self->getUnits()){
		if (u->getType() == UnitTypes::Protoss_Gateway)
			result.push_back(u);
	}
	return result;
}

void ArmyOrder::checkAndTrain(){
	int currentNum;
	//Broodwar->sendText("lastBuildCall %d lastBuildSuccess %d ", Collections::lastBuildCall, Collections::lastBuildSuccess);
	for (pair<UnitType, int> p : troopRequired){
		currentNum = Collections::getUnitList(Broodwar->self(), p.first).size();
		if (p.second > currentNum)
			if (Collections::getUnitList(Broodwar->self(), (p.first.whatBuilds()).first).size() > 0){
				train(OrderType(p.first, p.first.whatBuilds().first, 1));
			}
	}
}

void ArmyOrder::initZerg(){
}

void ArmyOrder::initProtoss(){
	troopRequired.insert(make_pair(UnitTypes::Protoss_Zealot, 20));
	troopRequired.insert(make_pair(UnitTypes::Protoss_Dragoon, 30));
	troopRequired.insert(make_pair(UnitTypes::Protoss_Observer, 2));
	troopRequired.insert(make_pair(UnitTypes::Protoss_Carrier, 10));
}

void ArmyOrder::initTerran(){
}

bool ArmyOrder::train(OrderType& orderType){

	if (!(Collections::lastBuildSuccess >= Collections::lastBuildCall && Broodwar->getFrameCount() > Collections::lastBuildSuccess + 13)){
		Broodwar->sendText("Can't train because other are training!");
		return false;
	}

	/*if (orderType.getUnitType() != Broodwar->self()->getRace().getWorker())
		if (Collections::currentTroopCycle >= Collections::limitTroopFor700Frame){
			Broodwar->sendText("CurrentTroopCycle %d CurrentLimit %d ", Collections::currentTroopCycle, Collections::limitTroopFor700Frame);
			return false;
		}*/
	//Broodwar->sendText("CurrentTroopCycle %d CurrentLimit %d ", Collections::currentTroopCycle, Collections::limitTroopFor700Frame);

	Broodwar->sendText("1 CurrentTroopCycle %d CurrentLimit %d ", Collections::currentTroopCycle, Collections::limitTroopFor700Frame);
	int supplyAvailable = Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();
	if (supplyAvailable < (2 * (orderType.unit.supplyRequired()))){
		//Broodwar->sendText("Prepare to build pylon");
		if (Broodwar->self()->supplyTotal() < 200){
			//Broodwar->sendText("Build Pylon calledddddddddddddddddddddddddddddddd");
			OrderQueue::getInstance()->build(Broodwar->self()->getRace().getSupplyProvider());
			if (supplyAvailable < orderType.supplyRequire){
				Broodwar->sendText("supplyError1");
				return false;
			}
		}
		/*else
			Broodwar->sendText("Supply eo be hon 200");*/
	}
	int successed = 0, minUnit = -1, minQueue = 10;// Number of successed training / unit not training now
	std::vector<Unit> parentList = Collections::getUnitList(Broodwar->self(), orderType.parent);
	Unit tmpUnit;
	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType->volume, minUnit);
	//tìm nhà có lượng order nhỏ nhất để train lính

	for (int i = 0; i < parentList.size(); i++){
		if ((parentList.at(i)->getTrainingQueue().size()) < minQueue){
			minQueue = parentList.at(i)->getTrainingQueue().size();
			minUnit = i;
		}
	}

	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	if (minUnit != -1){
		tmpUnit = parentList.at(minUnit);
		for (int i = 0; i < orderType.volume; i++)
			if ((orderType.volume - successed>0) /*&& (Collections::currentTroopCycle < Collections::limitTroopFor700Frame)*/){
				if ((tmpUnit->train(orderType.getUnitType()))){
					successed++;
					if ((orderType.getUnitType() != (Broodwar->self()->getRace().getWorker())
						&& (orderType.getUnitType()) != UnitTypes::Protoss_Interceptor)){
						Collections::currentTroopCycle++;
						Collections::trainInRow++;
						Collections::buildInRow = 0;
						Broodwar->sendText("Train non Worker successed!");
					}
					//BWAPI::Broodwar->sendText("train successed %d ", successed);
				}
				else{
					Broodwar->sendText("just cannot train");
					break;
				}
			}
			else
				Broodwar->sendText("error number limit");
	}

	orderType.volume -= successed;
	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	if (orderType.volume <= 0)
		return true;

	Broodwar->sendText("train failed eo hieu vi sao");
	return false;
}

bool ArmyOrder::train(OrderType* orderType){


	if (!(Collections::lastBuildSuccess >= Collections::lastBuildCall && Broodwar->getFrameCount() > Collections::lastBuildSuccess + 7)){
		Broodwar->sendText("Can't train because other are training!");
		return false;
	}
	Broodwar->sendText("1 CurrentTroopCycle %d CurrentLimit %d ", Collections::currentTroopCycle, Collections::limitTroopFor700Frame);

	/*if (orderType->getUnitType() != Broodwar->self()->getRace().getWorker())
		if (Collections::currentTroopCycle >= Collections::limitTroopFor700Frame){
			Broodwar->sendText("CurrentTroopCycle %d CurrentLimit %d ", Collections::currentTroopCycle, Collections::limitTroopFor700Frame);
			return false;
		}*/
	//Broodwar->sendText("CurrentTroopCycle %d CurrentLimit %d ", Collections::currentTroopCycle, Collections::limitTroopFor700Frame);

	int supplyAvailable = Broodwar->self()->supplyTotal() / 2 - Broodwar->self()->supplyUsed() / 2;
	//Broodwar->sendText("Supply available %d ", supplyAvailable);
	if (supplyAvailable < 2 * (orderType->unit.supplyRequired())){
		//Broodwar->sendText("Prepare to build pylon");
		if (Broodwar->self()->supplyTotal() < 200){
			//Broodwar->sendText("Build Pylon calledddddddddddddddddddddddddddddddd");
			OrderQueue::getInstance()->build(Broodwar->self()->getRace().getSupplyProvider());
			if (supplyAvailable < orderType->supplyRequire)
				return false;
		}
	}
	int successed = 0, minUnit = -1, minQueue = 10;// Number of successed training / unit not training now
	std::vector<Unit> parentList = Collections::getUnitList(Broodwar->self(), orderType->parent);
	Unit tmpUnit;
	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType->volume, minUnit);
	//tìm nhà có lượng order nhỏ nhất để train lính

	for (int i = 0; i < parentList.size(); i++){
		if ((parentList.at(i)->getTrainingQueue().size()) < minQueue){
			minQueue = parentList.at(i)->getTrainingQueue().size();
			minUnit = i;
		}
	}

	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	if (minUnit != -1){
		tmpUnit = parentList.at(minUnit);
		for (int i = 0; i < orderType->volume; i++)
			if ((orderType->volume - successed>0) /*&& Collections::currentTroopCycle < Collections::limitTroopFor700Frame*/){
				if ((tmpUnit->train(orderType->getUnitType()))){
					successed++;
					if ((orderType->getUnitType() != (Broodwar->self()->getRace().getWorker()))
						&& ((orderType->getUnitType()) != UnitTypes::Protoss_Interceptor)){
						Collections::currentTroopCycle++;
						Collections::trainInRow++;
						Collections::buildInRow = 0;
						Broodwar->sendText("Train non Worker successed!");
					}
					//BWAPI::Broodwar->sendText("train successed %d ", successed);
				}
				else
					break;
			}
	}

	orderType->volume -= successed;
	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	if (orderType->volume <= 0)
		return true;
	return false;
}

ArmyOrder::~ArmyOrder()
{

}
