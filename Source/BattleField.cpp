#include "BattleField.h"
#include "Collections.h"

#define DEFEND_RADIUS 60
#define MOVE_TIME_LIMIT 60
#define DEFENSE_POSITIONS 4

using namespace BWAPI;

const float BattleField::xDefensePosition[] = { -1, 0, 1, 0 };
const float BattleField::yDefensePosition[] = { 0, -1, 0, 1 };

BattleField::BattleField()
{
	isDefend = false;
}

BattleField::BattleField(BWAPI::Unitset selfUnitset, BWAPI::Position defendRootP){
	BattleSquad tmpSquad = BattleSquad(selfUnitset);
	selfForces.insert(tmpSquad);
	isDefend = true;

	selfUnit = selfUnitset;

	currentPositionState = 0;


	this->defendRoot = defendRootP;
}

void BattleField::onDefend(){
	//test defend code
	//Broodwar->printf("onDefend BattleField called! ");
	//Unitset enemyInRadius = Broodwar->getUnitsInRadius(defendRoot, DEFEND_RADIUS, Filter::IsEnemy);
	//if (enemyInRadius.size() != 0){
	//	testSelfSquad = BattleSquad(selfUnit, enemyInRadius);
	//}
	////cho chạy quanh địa điểm bảo vệ
	//else{
	//	if (Broodwar->getFrameCount() > lastCheckedFrame + MOVE_TIME_LIMIT){
	//		Broodwar->printf("Move defense called! %d size ", testSelfSquad.getSelfSize());
	//		lastCheckedFrame = Broodwar->getFrameCount();
	//		currentPositionState++;
	//		if (currentPositionState >= DEFENSE_POSITIONS)
	//			currentPositionState = 0;
	//		testSelfSquad.move(Position(defendRoot.x*xDefensePosition[currentPositionState] * DEFEND_RADIUS, defendRoot.y*yDefensePosition[currentPositionState] * DEFEND_RADIUS));
	//	}
	//}
}

void BattleField::onAttack(){

}


void BattleField::addUnits(BWAPI::Unitset uset){
	/*for (Unit u : uset)
		testSelfSquad.addUnit(u);*/
}

bool BattleField::isNearDefensePosition(Position p){
	if (Collections::distance(p, defendRoot) < 32){
		return true;
	}
}

void BattleField::checkRequirements(){
	
}

int BattleField::getSelfSize(){
	return this->selfForces.size();
}

BattleField::~BattleField()
{
}
