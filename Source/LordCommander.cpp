#include "LordCommander.h"
#include "map"
#include <BWAPI.h>
#include "OrderQueue.h"
#include "UnitTimeManager.h"

#define ENEMY_TARGET_CONST 50
#define START_ATTACK_RANGE 25000

using namespace BWAPI;
using namespace std;

bool LordCommander::isInitedInstance = false;
LordCommander* LordCommander::instance = nullptr;
//Unitset LordCommander::freeUnits = Unitset();
std::map<BWAPI::Unit, bool> LordCommander::isUsedUnit;


LordCommander::LordCommander()
{
	onFrameCounter = 0;
	initArmy();
	hordeManager = set<BattleHorde*>();
	enemyAttackedBy = map<Unit, set<Unit>>();
	isStartAttack = false;
	//defenders.insert(initMainBaseDefense());
}

void LordCommander::checkAttack(){
	int destroyScoreSum = 0;
	for (BattleHorde *horde : getInstance()->hordeManager){
		destroyScoreSum += horde->getUnitType().destroyScore() * horde->getSelfSize();
	}
	if (destroyScoreSum > START_ATTACK_RANGE)
		isStartAttack = true;
}

void LordCommander::initArmy(){

}

void LordCommander::onFrame(){
	if (!isStartAttack)
		for (BattleHorde* horde : instance->hordeManager)
			horde->onFrame();
	else{
		totalAttack();
	}



	/*set<BattleHorde*> hordeCopy = hordeManager;

	for (BattleHorde* horde : hordeCopy)
	if (horde->getSelfSize() == (horde->getMaxUnit() / 2))
	if (isReforcable(horde))
	reforce(horde);*/

}

void LordCommander::totalAttack(){

	//Broodwar->sendText("Total war!");

	/*for (BattleHorde* horde : hordeManager)
	{
	for (Unit u : horde->getCurrentList()){
	if (Collections::distance(u->getPosition(), p) > 1000){
	horde->isHoldPosition = false;
	horde->move(p);
	}
	}
	}*/
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
	BattleHorde* tmpHorde = new BattleHorde(u->getType(), UnitTimeManager::getEndFrame(u->getType()));
	if (tmpHorde->isMelee()){
		Broodwar->sendText("Added to melee attacker");
		getInstance()->groundMeleeSet.insert(tmpHorde);
	}
	else if (tmpHorde->isGroundFarAttack()){
		Broodwar->sendText("Added to far ground attackers");
		getInstance()->groundFarAttackSet.insert(tmpHorde);
	}
	else if (tmpHorde->isFlyAttack()){
		Broodwar->sendText("Added to air attackers");
		getInstance()->airAttackSet.insert(tmpHorde);
	}
	else if (tmpHorde->isDetector()){
		Broodwar->sendText("Added to detector");
		getInstance()->detectorSet.insert(tmpHorde);
	}
	getInstance()->hordeManager.insert(tmpHorde);
	Broodwar->sendText("Horde size: %d ", getInstance()->hordeManager.size());
}

void LordCommander::reforce(BattleHorde* b){
	LordCommander* ins = getInstance();
	Broodwar->sendText("Reforced");
	Unitset battleCurrentList = b->getCurrentList();
	for (Unit u : battleCurrentList)
		ins->removeDeadUnit(u);

	ins->hordeManager.erase(b);

	for (Unit u : battleCurrentList)
		ins->addUnit(u);
}

bool LordCommander::isReforcable(BattleHorde* horde){
	LordCommander* ins = getInstance();
	int hordeSize = horde->getSelfSize();
	for (BattleHorde* b : ins->hordeManager)
		if (b != horde){
			if (b->getMaxUnit() - b->getSelfSize() >= hordeSize)
				return true;
		}
	return false;
}

void LordCommander::regTarget(Unit target, Unit selfUnit){

	Broodwar->sendText("Target registered ");

	LordCommander* ins = getInstance();
	set<Unit> tmpSet = set<Unit>();
	map<Unit, set<Unit>>::iterator it = enemyAttackedBy.find(target);
	if (it == enemyAttackedBy.end()){
		//add new target
		tmpSet.insert(selfUnit);
		ins->enemyAttackedBy.insert(make_pair(target, tmpSet));
	}
	else{
		it->second.insert(selfUnit);
	}


	//if (selfHorde->getUnitType() == UnitTypes::Protoss_Carrier)
	//	return;

	//LordCommander* ins = getInstance();
	//set<BattleHorde*>tmpSet = set<BattleHorde*>();
	//map<Unit, set<BattleHorde*>>::iterator it = enemyAttackedBy.find(target);
	//if (it == enemyAttackedBy.end()){
	//	//add new target
	//	tmpSet.insert(selfHorde);
	//	ins->enemyAttackedBy.insert(make_pair(target, tmpSet));
	//}
	//else{
	//	(it->second).insert(selfHorde);
	//}
}

void LordCommander::removeTarget(Unit u, Unit selfUnit){

	Broodwar->sendText("Target removed ");
	LordCommander* ins = getInstance();
	map<Unit, set<Unit>>::iterator it = ins->enemyAttackedBy.find(u);
	if (it != ins->enemyAttackedBy.end())
		(it->second).erase(selfUnit);
	/*LordCommander* ins = getInstance();
	map<Unit, set<BattleHorde*>>::iterator it = ins->enemyAttackedBy.find(u);
	if (it != ins->enemyAttackedBy.end())
	(it->second).erase(horde);*/
}

//bool LordCommander::shouldAttackThis(BattleHorde* selfHorde, Unit enemyUnit){
//	LordCommander* ins = getInstance();
//	map<Unit, set<Unit>>::iterator it = ins->enemyAttackedBy.find(enemyUnit);
//	if (it == enemyAttackedBy.end())
//		return true;
//	else{
//		int selfScore = selfHorde->getSelfSize() * selfHorde->getUnitType().destroyScore();
//		set<Unit>tmpSet = it->second;
//		for (Unit b : tmpSet){
//			//selfScore += b->getSelfSize()*b->getUnitType().destroyScore();
//			selfScore = tmpSet.size()*b->getType().destroyScore();
//		}
//		if (selfScore < enemyUnit->getType().destroyScore() + ENEMY_TARGET_CONST)
//			return true;
//	}
//	return false;
//}

int LordCommander::getSelfScoreOnTarget(Unit enemy){
	LordCommander* ins = getInstance();
	map<Unit, set<Unit>>::iterator it = ins->enemyAttackedBy.find(enemy);
	int selfScore = 0;
	if (it == enemyAttackedBy.end())
		return 0;
	else{
		set<Unit>tmpSet = it->second;
		for (Unit b : tmpSet){
			//selfScore += (b->getSelfSize())*(b->getUnitType().destroyScore());
			return tmpSet.size()*b->getType().destroyScore();
		}
	}

	Broodwar->sendText("Score %d", selfScore);

	return selfScore;
}

void LordCommander::reform(){
	Broodwar->sendText("reformed");
	for (BattleHorde *horde : getInstance()->hordeManager)
		if (!(horde->isAttacking()))
		{
			if (Collections::distance(horde->getApproxPosition(), Collections::defensePosition) > 100)
				horde->move(Collections::defensePosition);
		}
}

LordCommander::~LordCommander()
{
}
