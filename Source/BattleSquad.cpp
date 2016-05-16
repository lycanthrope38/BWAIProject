#include "BattleSquad.h"


BattleSquad::BattleSquad()
{
}


void BattleSquad::addUnit(BWAPI::Unit u){
	squad.insert(u);
	freeSquad.insert(u);
}

void BattleSquad::attack(BWAPI::Unitset targets, int devidedBy){
	//chưa xong
}


void BattleSquad::attack(std::vector<BWAPI::Unit> targets, int devidedBy){
	//chưa xong
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
	if (isOnDuty)
		return false;
	return true;
}

bool BattleSquad::isWiped(){
	if (squad.size() == 0)
		return true;
	return false;
}

BattleSquad::~BattleSquad()
{
}
