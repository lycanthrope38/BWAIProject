#include "BattleField.h"
#include "Collections.h"
#include "OrderQueue.h"

#define DEFEND_RADIUS 60
#define MOVE_TIME_LIMIT 60
#define DEFENSE_POSITIONS 4

using namespace BWAPI;

const float BattleField::xDefensePosition[] = { -1, 0, 1, 0 };
const float BattleField::yDefensePosition[] = { 0, -1, 0, 1 };

BattleField::BattleField(std::map<UnitType, int> requiredUnitNum)
{
	isDefend = false;
	ordered = 0;
	this->requiredUnitNumber = requiredUnitNum;
}


//constructor for defending purpose
BattleField::BattleField(BWAPI::Unitset selfUnitset, BWAPI::Position defendRootP, std::map<UnitType, int> requiredUnitNum){
	BattleSquad* tmpSquad = new BattleSquad(selfUnitset);
	selfForces.insert(tmpSquad);
	isDefend = true;

	selfUnit = selfUnitset;

	currentPositionState = 0;

	ordered = 0;

	this->requiredUnitNumber = requiredUnitNum;

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
	if (Collections::distance(p, defendRoot) < 10){
		return true;
	}
}

void BattleField::checkRequirements(){
	UnitType tmpType;
	int vol;
	std::map<UnitType, int>::iterator it;

	for (BattleSquad* squad : selfForces){
		tmpType = squad->getSelfType();
		vol = squad->getSelfSize();
		it = requiredUnitNumber.find(tmpType);
		if (it!=requiredUnitNumber.end())
		{
			if (it->second < vol){
				OrderQueue::getInstance()->push(tmpType, tmpType.whatBuilds().first, (vol - it->second), OrderQueue::PRIORITY_HIGH);
				ordered += vol - it->second;
			}
		}
	}
}

int BattleField::getSelfSize(){
	return this->selfForces.size();
}

BattleField::~BattleField()
{
}
