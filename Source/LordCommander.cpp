#include "LordCommander.h"
#include "map"
#include <BWAPI.h>
#include "OrderQueue.h"
#include "UnitTimeManager.h"

using namespace BWAPI;
using namespace std;

bool LordCommander::initedInstance = false;
LordCommander* LordCommander::instance = nullptr;
//Unitset LordCommander::freeUnits = Unitset();
std::map<BWAPI::Unit, bool> LordCommander::isUsedUnit;


LordCommander::LordCommander()
{
	onFrameCounter = 0;
	initArmy();
	hordeManager = vector<BattleHorde*>();
	//defenders.insert(initMainBaseDefense());
}

void LordCommander::initArmy(){
	
}

void LordCommander::onFrame(){
	for (BattleHorde* horde : instance->hordeManager)
		horde->onFrame();
}

void LordCommander::requireUnit(BattleHorde* childHorde, UnitType type, int soluong){
	OrderQueue::getInstance()->push(type, type.whatBuilds().first, soluong);
}

void LordCommander::removeDeadUnit(Unit u){
	map<Unit, BattleHorde*>::iterator it = getInstance()->unitManager.find(u);
	if (it == getInstance()->unitManager.end()){
		Broodwar->sendText("Unit not exist!");
	}
	else{
		it->second->clearDeadUnit(u);
		getInstance()->unitManager.erase(it);
	}
}

void LordCommander::onDefend(){

	LordCommander* ins = getInstance();
	
}

void LordCommander::onAttack(){
}

bool LordCommander::addUnit(Unit u){
	UnitType tmpType = u->getType();
	for (BattleHorde* horde : hordeManager){
		if (horde->getUnitType() == tmpType)
			if (!(horde->isFullUnit())){
				horde->addUnit(u);
				getInstance()->unitManager.insert(make_pair(u, horde));
				Broodwar->sendText("Added to a Horde");
				return true;
			}
	}
	addHorde(u);
	return addUnit(u);
}

void LordCommander::addHorde(Unit u){
	getInstance()->hordeManager.push_back(new BattleHorde(u->getType(), UnitTimeManager::getEndFrame(u->getType())));
	Broodwar->sendText("Horde size: %d ", getInstance()->hordeManager.size());
}

LordCommander::~LordCommander()
{
}
