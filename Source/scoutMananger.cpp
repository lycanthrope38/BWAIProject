#include "scoutManager.h"
#include "BWAPI.h"

using namespace BWAPI;
using namespace Filter;

scoutManager::scoutManager(){
	_scout = nullptr;
	_scoutStatus = "None";
	_scoutUnderAttack = false;
	//_numOfScouts = 0;
}

void scoutManager::onUnitCreate(){ // hàm này chọn scout từ đám worker có sẵn
	if (!_scout){ // if no scout is designated for scouting
		Unit base = BWAPI::Broodwar->getClosestUnit(Position(Broodwar->self()->getStartLocation()), BWAPI::Filter::IsResourceDepot); // error might be here with getStartLocation
		Unit selectedScout = base->getClosestUnit(Filter::IsWorker && Filter::IsOwned && Filter::IsGatheringMinerals && !Filter::IsParasited);
		if (selectedScout->isCarryingMinerals()) selectedScout->returnCargo();
		_scout = selectedScout;
		_scoutStatus = "available";
		Broodwar->sendText("Scout selected");
		return;
	}
	return; // if there were a worker designated for scouting 
}

void scoutManager::moveScout(){ // move scout to enemy base locations
	if (!_scout && !_scout->exists()){
		Broodwar->sendText("scout is Unavailable");
		onUnitCreate();
		return ; 
	}
	if (_scout->isMoving()) return; // the unit is moving to destination
	Position testLocation = Position(1000, 1000); // test Location
	if (_scout->move(testLocation, false)){
		Broodwar->sendText("scout is moving");
		return;
	}
	else {
		Broodwar->sendText("can't move to (1000 , 1000)");
		return;
	}



}

bool scoutManager::isScout(Unit unit){
	if (unit->getID() ==this-> _scout->getID()) return true;
	else return false;
}

void scoutManager::setScout(Unit unit){
	_scout = unit;
	return;
}

Unit scoutManager::returnScout(){
	return _scout;
}