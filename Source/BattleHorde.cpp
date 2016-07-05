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
int BattleHorde::maxDefenseRange = 400;

BattleHorde::BattleHorde(UnitType type)
{
	isMoving = false;
	isOrdered = false;
	isHoldPosition = true;
	defensePosition = Positions::None;
	selfType = type;
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

	drawSquad();
	checkTarget();
	return true;
}

void BattleHorde::checkTarget(){

	Unitset selfSet, enemySet;
	int selfTroopValue = 0, enemyTroopValue = 0;

	for (Unit u : selfTroops){

		selfSet = u->getUnitsInRadius((u->getType().seekRange() / 2), Filter::IsOwned);
		enemySet = u->getUnitsInRadius((u->getType().seekRange() / 2), Filter::IsEnemy);

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
			if ((target[u])->exists() && !((target[u])->isBurrowed() || target[u]->isInvincible() || target[u]->isCloaked())){
				if (runBackToDefense()){
					//if (Collections::getMaxAttackRange(target[u]) < Collections::getMaxAttackRange(bestDefenseNeared) - 20)
					if (Collections::distance(bestDefenseNeared->getPosition(), target[u]->getPosition()) < Collections::getMaxAttackRange(bestDefenseNeared) - 20)
						attack(u, target[u]);
					else
						move(bestDefenseNeared->getPosition());
				}
				else
					attack(u, target[u]);
			}
			else{
				target[u] = targetManager.getTarget(u);
				//if ()
				if (runBackToDefense()){
					//if (Collections::getMaxAttackRange(target[u]) < Collections::getMaxAttackRange(bestDefenseNeared) - 20)
					if (Collections::distance(bestDefenseNeared->getPosition(), target[u]->getPosition()) < Collections::getMaxAttackRange(bestDefenseNeared) - 20)
						attack(u, target[u]);
					else
						move(bestDefenseNeared->getPosition());
				}
				else
					attack(u, target[u]);
			}
		}
		else{
			target[u] = targetManager.getTarget(u);
			if (runBackToDefense()){
				//if (Collections::getMaxAttackRange(target[u]) < Collections::getMaxAttackRange(bestDefenseNeared) - 20)
				if (Collections::distance(bestDefenseNeared->getPosition(), target[u]->getPosition()) < Collections::getMaxAttackRange(bestDefenseNeared) - 20)
					attack(u, target[u]);
				else
					move(bestDefenseNeared->getPosition());
			}
			else
				attack(u, target[u]);
			/*target[u] = targetManager.getTarget(u);
			attack(u, target[u]);*/
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

	if (selfType == UnitTypes::Protoss_Carrier){
		lastAttackCommand.insert(make_pair(u, 0));
	}

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
	/*if (endFrame > Broodwar->getFrameCount()){
		LordCommander::getInstance()->requireUnit(this, selfType, 1);
		}*/
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
			if (selfType == UnitTypes::Protoss_Carrier){
				if (Broodwar->getFrameCount() - lastAttackCommand[selfUnit] > 5){
					carrierAttack(selfUnit, enemyUnit);
					checkAndEvade(selfUnit, enemyUnit);
				}
			}
			else
				selfUnit->attack(enemyUnit);
		}
		else{
			checkAndEvade(selfUnit, enemyUnit);
		}
		return;
	}
	else{
		if (selfUnit->getGroundWeaponCooldown() == 0){
			selfUnit->attack(enemyUnit);
		}
		else
			checkAndEvade(selfUnit, enemyUnit);
	}
}

void BattleHorde::checkAndEvade(Unit selfUnit, Unit target){
	if (!(selfUnit && target))
		return;
	if (!(selfUnit->exists() && target->exists()))
		return;
	if (Collections::distance(selfUnit->getPosition(), target->getPosition()) < Collections::getMaxAttackRange(target)){
		Position backVector = Position(target->getPosition().x - selfUnit->getPosition().x, target->getPosition().y - selfUnit->getPosition().y);
		selfUnit->move(Position(selfUnit->getPosition().x - backVector.x, selfUnit->getPosition().y - backVector.y));
	}
}

void BattleHorde::carrierAttack(Unit selfCarrier, Unit target){
	Unitset interceptors = selfCarrier->getInterceptors();
	for (Unit u : interceptors)
		if (u->getAirWeaponCooldown() == 0)
			u->attack(target);
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

bool BattleHorde::runBackToDefense(){
	if (Broodwar->getFrameCount() > 7000)
		return false;
	UnitType defense = Collections::getStaticDefenseStructure(Broodwar->self()->getRace());
	if (defense == UnitTypes::None)
		return false;
	Unitset enemy;
	Unitset selfBuildings;
	Unit bestDefense;
	int mostEnemyNeared = 0;
	for (Unit u : selfTroops)
		if (u->exists()){
			enemy = u->getUnitsInRadius(300, Filter::IsEnemy);
			selfBuildings = u->getUnitsInRadius(300, Filter::IsBuilding);
			for (Unit building : selfBuildings)
				if (Broodwar->self()->isEnemy(building->getPlayer()))
					selfBuildings.erase(building);
		}
	if (enemy.size() <= 0)
		return false;

	for (Unit u : selfBuildings)
		if (u->getType() == defense){
			if (u->getUnitsInRadius(300, Filter::IsEnemy).size() > mostEnemyNeared){
				mostEnemyNeared = u->getUnitsInRadius(300, Filter::IsEnemy).size();
				bestDefense = u;
			}
		}
	if (mostEnemyNeared > 0){
		if (enemy.size() > selfTroops.size()){
			if (bestDefense->getType().groundWeapon() != WeaponTypes::None && bestDefense->getType().airWeapon() != WeaponTypes::None){
				int maxDefenseBuildingRange = bestDefense->getType().groundWeapon().maxRange();
				if (maxDefenseBuildingRange < bestDefense->getType().airWeapon().maxRange())
					maxDefenseBuildingRange = bestDefense->getType().airWeapon().maxRange();
				if (maxDefenseBuildingRange >= 150)
					if (Collections::distance(bestDefense->getPosition(), getApproxPosition()) >= 10){
						this->bestDefenseNeared = bestDefense;
						return true;
					}
			}
		}
	}
	return false;
}

BattleHorde::~BattleHorde()
{
	//(LordCommander::getInstance())->removeTarget(target, this);
}