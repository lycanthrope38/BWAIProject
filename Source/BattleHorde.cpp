#include "BattleHorde.h"
#include "LordCommander.h"
#include <BWAPI.h>
#include <random>
#include "Collections.h"
#include "ArmyOrder.h"

using namespace BWAPI;

vector<Color> BattleHorde::squadColorList = vector<Color>();
bool BattleHorde::isInited = false;

const int BattleHorde::INFINITY_LIFE_TIME = 999999999;
int BattleHorde::maxDefenseRange = 1000;

BattleHorde::BattleHorde(UnitType type, int endFrame)
{
	isMoving = false;
	isOrdered = false;
	isHoldPosition = true;
	defensePosition = Positions::None;
	selfType = type;
	this->endFrame = endFrame;
	maxUnit = calculateMaxUnit(type);
	targetManager = TargetManager();
	target = map<Unit, Unit>();
	isAttacked = false;

	isAirAttackable = false;

	this->squadColor = Color(rand() % 256, rand() % 256, rand() % 256);

	lastAttackCall = 0;
}

//xử lý mỗi frame
bool BattleHorde::onFrame(){
	//kiểm tra nếu bên ta có loại đánh xa thì áp dụng hit and run

	if (selfType == UnitTypes::Protoss_Carrier)
		buyInterceptor();

	//Unit oldTarget = target;

	drawSquad();
	checkTarget();
	return true;
}

void BattleHorde::checkTarget(){

	Unitset selfSet, enemySet;
	int selfTroopValue = 0, enemyTroopValue = 0;

	for (Unit u : selfTroops){

		selfSet = u->getUnitsInRadius((u->getType().seekRange() / 2), Filter::IsOwned);
		enemySet = u->getUnitsInRadius((u->getType().seekRange() / 2),Filter::IsEnemy);

		if (enemySet.size() > 0){
			for (Unit u : selfSet)
				selfTroopValue += u->getType().destroyScore();
			for (Unit u : enemySet)
				enemyTroopValue += u->getType().destroyScore();
			if (enemyTroopValue > selfTroopValue){
				runBack(u);
				//Broodwar->sendText("Runback called");
				continue;
			}
		}

		if (target[u]){
			if ((target[u])->exists() && !((target[u])->isBurrowed() || target[u]->isInvincible() || target[u]->isCloaked()))
				attack(u, target[u]);
			else{
				target[u] = targetManager.getTarget(u);
				attack(u, target[u]);
			}
		}
		else{
			target[u] = targetManager.getTarget(u);
			attack(u, target[u]);
		}
	}
}

void BattleHorde::runBack(Unit u){

	Unitset enemiesInRange;
	WeaponType selfWeapon;
	Unit selfUnit;

	Position runBackVector = Position(0, 0);

	if (u->exists()){
		//uset = u->getUnitsInWeaponRange()
		selfUnit = u;
		selfWeapon = u->getType().groundWeapon();
		if (selfWeapon == WeaponTypes::None || selfWeapon == WeaponTypes::Unknown)
			selfWeapon = u->getType().airWeapon();
		enemiesInRange = Broodwar->getUnitsInRadius(u->getPosition(), selfWeapon.maxRange(), Filter::IsEnemy);
		for (Unit enemy : enemiesInRange){
			if ((enemy->canAttack(u))){
				runBackVector.x += (enemy->getPosition().x - u->getPosition().x);
				runBackVector.y += (enemy->getPosition().y - u->getPosition().y);
			}
		}
	}

	if (runBackVector != Position(0, 0))
		selfTroops.move(Position(selfUnit->getPosition().x - runBackVector.x, selfUnit->getPosition().y - runBackVector.y));
}

//thêm quân
void BattleHorde::addUnit(BWAPI::Unit u){
	selfTroops.insert(u);
	if (defensePosition == Positions::None || Positions::Unknown)
		defensePosition = u->getPosition();
	if (u->getType().airWeapon() == WeaponTypes::None || u->getType().airWeapon() || WeaponTypes::Unknown)
		isAirAttackable = false;
	else
		isAirAttackable = true;
	target.insert(make_pair(u, nullptr));

}

//lấy danh sách quân
Unitset BattleHorde::getCurrentList(){
	return selfTroops;
}

//thêm mục tiêu
void BattleHorde::addTarget(Unit selfUnit, BWAPI::Unit targetInp){
	this->target[selfUnit] = targetInp;
}

void BattleHorde::clearDeadUnit(Unit u){
	//hàm này sẽ được gọi từ LordCommander
	selfTroops.erase(u);
	if (endFrame > Broodwar->getFrameCount()){
		LordCommander::getInstance()->requireUnit(this, selfType, 1);
	}
}

int BattleHorde::calculateMaxUnit(UnitType type){
	WeaponType groundWeapon = type.groundWeapon();
	WeaponType airWeapon = type.airWeapon();
	/*if (airWeapon != WeaponTypes::None&&airWeapon != WeaponTypes::Unknown){
		if (type.destroyScore() < 900)
		return 5;
		else
		return 3;
		}

		if (groundWeapon != WeaponTypes::None&&groundWeapon != WeaponTypes::Unknown){
		if (groundWeapon.maxRange() <= 35)
		return 2;
		else
		return 5;
		}*/
	return 1;
}

void BattleHorde::attack(Unit selfUnit, Unit enemyUnit){
	if (selfUnit->isFlying()){
		if (selfUnit->getAirWeaponCooldown() == 0){
			selfUnit->attack(enemyUnit);
		}
		return;
	}
	else{
		if (selfUnit->getGroundWeaponCooldown() == 0){
			selfUnit->attack(enemyUnit);
		}
	}
}

void BattleHorde::move(Position p){
	Unitset stuckers;
	selfTroops.move(p);
	for (Unit u : selfTroops){
		if (u->isStuck())
		{
			stuckers = u->getUnitsInRadius(50, Filter::IsBuilding);
			for (Unit stuck : stuckers)
				if (stuck->getType().destroyScore() < 200){
					u->attack(stuck);
					continue;
				}
		}
	}
}

void BattleHorde::drawPosition(Position p){
	Broodwar->drawBoxMap(Position(p.x - 10, p.y - 10), Position(p.x + 10, p.y + 10), this->squadColor);
}

void BattleHorde::drawSquad(){
	for (Unit u : selfTroops){
		drawPosition(u->getPosition());
	}
}

void BattleHorde::buyInterceptor(){
	for (Unit u : selfTroops)
		if (!(u->train(UnitTypes::Protoss_Interceptor)))
			continue;
}

BattleHorde::~BattleHorde()
{
	//(LordCommander::getInstance())->removeTarget(target, this);
}