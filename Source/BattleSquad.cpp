#include "BattleSquad.h"


BattleSquad::BattleSquad(std::set<BWAPI::Unit> selfUnitSet, std::set<BWAPI::Unit> enemyUnitSet)
{
	isOnDuty = true;
	squad = selfUnitSet;
	targets = enemyUnitSet;
	freeSquad = squad;
	targetsOnRisk = targets;
}


bool BattleSquad::onFrame(){

	if (isCompleted())
		return false;

	if (freeSquad.size() != 0)
		matching();
	else
		callBelowOnFrames();
	return true;
}

bool BattleSquad::matching(){

}

void BattleSquad::callBelowOnFrames(){
	for (BattleHorde* b : this->hordes)
		b->onFrame();
}

void BattleSquad::addUnit(BWAPI::Unit u){
	squad.insert(u);
	freeSquad.insert(u);
}

void BattleSquad::clearSquad(){
	squad.clear();
}

void BattleSquad::addSquad(BWAPI::Unitset unitSet){
	for (BWAPI::Unit u : unitSet){
		squad.insert(u);
		freeSquad.insert(u);
	}
}
void BattleSquad::addSquad(std::vector<BWAPI::Unit> unitSet){
	for (BWAPI::Unit u : unitSet){
		squad.insert(u);
		freeSquad.insert(u);
	}
}
bool BattleSquad::isCompleted(){

	if (isWiped()||!isOnDuty)
		return true;
	return false;
}

bool BattleSquad::isWiped(){
	if (squad.size() == 0)
		return true;
	return false;
}

BattleSquad::~BattleSquad()
{
}
