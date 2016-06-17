#include "BattleSquad.h"
#include "BattleHorde.h"

BattleSquad::BattleSquad(BWAPI::Unitset selfUnitSet){
	isOnDuty = true;
	this->squad = selfUnitSet;
	freeSquad = selfUnitSet;
	counter = 0;

}

BattleSquad::BattleSquad(BWAPI::Unitset selfUnitSet, BWAPI::Unitset enemyUnitSet)
{
	isOnDuty = true;
	squad = selfUnitSet;
	targets = enemyUnitSet;
	freeSquad = squad;
	counter = 0;
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
bool BattleSquad::isCompleted(){
	if (isWiped() || !isOnDuty)
		return true;
	return false;
}

void BattleSquad::move(BWAPI::Position position){
	squad.move(position);
}

bool BattleSquad::isWiped(){
	if (squad.size() == 0)
		return true;
	return false;
}

void BattleSquad::clearDeadUnit(BattleHorde*, BWAPI::Unit){
	
}

BattleSquad::~BattleSquad()
{
}
