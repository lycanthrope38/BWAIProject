#include "LordCommander.h"
#include "map"
#include <BWAPI.h>
#include "OrderQueue.h"

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

void LordCommander::addUnit(Unit u){
	UnitType tmpType = u->getType();
	for (BattleHorde* horde : hordeManager){
		if (horde->getUnitType() == tmpType)
			if (!(horde->isFullUnit())){
				horde->addUnit(u);
				getInstance()->unitManager.insert(make_pair(u, horde));
			}
	}
}

LordCommander::~LordCommander()
{
}
