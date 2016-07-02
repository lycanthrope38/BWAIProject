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

bool ArmyOrder::trainZealot(){
	std::vector<Unit> gatewayList = getGatewayList();
	if (gatewayList.size() == 0)
		return false;
	Broodwar->sendText(std::to_string(gatewayList.size()).c_str());
	//trường hợp lúc đầu ít tiền thì chọn cái nào đang rảnh rỗi để train lính
	for (Unit u : gatewayList){
		if (!(u->isTraining()))
			if (self->minerals() >= 100){
				return u->train(UnitTypes::Protoss_Zealot);
			}
	}
	for (Unit u : gatewayList)
		if (self->minerals() >= 100){
			u->train(UnitTypes::Protoss_Zealot);
		}

}

void ArmyOrder::checkAndTrain(){
	int currentNum;
	if (Collections::lastBuildCall < Collections::lastBuildSuccess){
		for (pair<UnitType, int> p : troopRequired){
			currentNum = Collections::getUnitList(Broodwar->self(), p.first).size();
			if (p.second > currentNum)
				if (Collections::getUnitList(Broodwar->self(), (p.first.whatBuilds()).first).size() > 0){
					train(OrderType(p.first, p.first.whatBuilds().first, p.second - currentNum));
				}
		}
	}
}

void ArmyOrder::initZerg(){
}

void ArmyOrder::initProtoss(){
	troopRequired.insert(make_pair(UnitTypes::Protoss_Zealot, 15));
	troopRequired.insert(make_pair(UnitTypes::Protoss_Dragoon, 15));
	troopRequired.insert(make_pair(UnitTypes::Protoss_Observer, 1));
	troopRequired.insert(make_pair(UnitTypes::Protoss_Carrier, 3));
}

void ArmyOrder::initTerran(){
}

bool ArmyOrder::train(OrderType& orderType){
	int supplyAvailable = Broodwar->self()->supplyTotal()  - Broodwar->self()->supplyUsed();
	if (supplyAvailable < (2 * (orderType.unit.supplyRequired()))){
		Broodwar->sendText("Prepare to build pylon");
		if (Broodwar->self()->supplyTotal() < 200){
			Broodwar->sendText("Build Pylon calledddddddddddddddddddddddddddddddd");
			OrderQueue::getInstance()->build(Broodwar->self()->getRace().getSupplyProvider());
			if (supplyAvailable < orderType.supplyRequire)
				return false;
		}
		else
			Broodwar->sendText("Supply eo be hon 200");
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
			if ((orderType.volume - successed>0) && ((successed + 1)*(orderType.getUnitType().mineralPrice() < Broodwar->self()->minerals()))){
				if ((tmpUnit->train(orderType.getUnitType()))){
					successed++;
					//BWAPI::Broodwar->sendText("train successed %d ", successed);
				}
				else
					break;
			}
	}

	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	//for (int i = 0; i < parentList.size(); i++){
	//	if (orderType->volume > 0)
	//		for (int j = 0; j < orderType->volume; j++)
	//			if (orderType->volume - successed>0 && ((successed + 1)*(orderType->getUnitType().mineralPrice() < Broodwar->self()->minerals()))){
	//				tmpUnit = parentList.at(i);
	//				if ((tmpUnit->train(orderType->getUnitType()))){
	//					successed++;
	//				}
	//			}
	//			else
	//				break;
	//	else
	//		break;
	//}

	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);

	orderType.volume -= successed;
	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	if (orderType.volume <= 0)
		return true;
	return false;
}

bool ArmyOrder::train(OrderType* orderType){
	int supplyAvailable = Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2;
	Broodwar->sendText("Supply available %d ", supplyAvailable);
	if (supplyAvailable < 2 * (orderType->unit.supplyRequired())){
		Broodwar->sendText("Prepare to build pylon");
		if (Broodwar->self()->supplyTotal() < 200){
			Broodwar->sendText("Build Pylon calledddddddddddddddddddddddddddddddd");
			OrderQueue::getInstance()->build(Broodwar->self()->getRace().getSupplyProvider());
			if (supplyAvailable < orderType->supplyRequire)
				return false;
		}
		else
			Broodwar->sendText("Supply eo be hon 200");
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
			if ((orderType->volume - successed>0) && ((successed + 1)*(orderType->getUnitType().mineralPrice() < Broodwar->self()->minerals()))){
				if ((tmpUnit->train(orderType->getUnitType()))){
					successed++;
					//BWAPI::Broodwar->sendText("train successed %d ", successed);
				}
				else
					break;
			}
	}

	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	//for (int i = 0; i < parentList.size(); i++){
	//	if (orderType->volume > 0)
	//		for (int j = 0; j < orderType->volume; j++)
	//			if (orderType->volume - successed>0 && ((successed + 1)*(orderType->getUnitType().mineralPrice() < Broodwar->self()->minerals()))){
	//				tmpUnit = parentList.at(i);
	//				if ((tmpUnit->train(orderType->getUnitType()))){
	//					successed++;
	//				}
	//			}
	//			else
	//				break;
	//	else
	//		break;
	//}

	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);

	orderType->volume -= successed;
	//BWAPI::Broodwar->sendText("Training successed %d / %d _minUnit_ %d", successed, orderType.volume, minUnit);
	if (orderType->volume <= 0)
		return true;
	return false;
}
//bool train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit){
//	return parentUnit->train(typeOfUnit);
//}
//
//bool ArmyOrder::train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit, int unitVolume){
//	int counter=0;
//	for (int i = 0; i < unitVolume; i++)
//		if (parentUnit->train(typeOfUnit))
//			counter++;
//	if (counter == unitVolume)
//		return true;
//	return false;
//}
//
//bool ArmyOrder::train(std::vector<BWAPI::Unit> parentUnits, BWAPI::UnitType typeOfUnit){
//	int minIndex = -1;
//	for (int i = 0; i < parentUnits.size();i++)
//		if (!parentUnits.at(i)->isTraining()){
//			minIndex = i;
//			break;
//		}
//	if (minIndex != -1){
//		train(parentUnits.at(minIndex),typeOfUnit);
//		return true;
//	}
//
//}

ArmyOrder::~ArmyOrder()
{

}
