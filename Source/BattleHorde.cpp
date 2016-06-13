#include "BattleHorde.h"
#include "LordCommander.h"
#include <BWAPI.h>
#include <random>
#include "Collections.h"

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
	target = nullptr;
	isAttacked = false;


	
	this->squadColor = Color(rand() % 256, rand() % 256, rand() % 256);
}

//xử lý mỗi frame
bool BattleHorde::onFrame(){
	//kiểm tra nếu bên ta có loại đánh xa thì áp dụng hit and run

	if (selfType == UnitTypes::Protoss_Carrier)
		buyInterceptor();

	Unit oldTarget = target;

	drawSquad();
	if (target!=nullptr)
		if (target->exists())
		{
			drawPosition(target->getPosition());
			return true;
		}

	for (Unit u : selfTroops){
		if (u->exists()){
			target = targetManager.getTarget(u);
			if (target == nullptr){
				//Broodwar->sendText("Target == nullptr");
				break;
			}

			if (target == oldTarget){
				if (isHoldPosition)
					if (Collections::distance(target->getPosition(), defensePosition) > maxDefenseRange)
						selfTroops.move(defensePosition);
				attack(target);
				drawPosition(target->getPosition());
				//Broodwar->sendText("Old target. Skipped");
				break;
			}
			if (isHoldPosition)
				if (Collections::distance(target->getPosition(), defensePosition) > maxDefenseRange){
					selfTroops.move(defensePosition);
					break;
				}

			LordCommander::getInstance()->removeTarget(oldTarget, this);
			LordCommander::getInstance()->regTarget(target, this);
			attack(target);
			drawPosition(target->getPosition());
			isAttacked = true;
			//Broodwar->sendText("Enemy detected at %d  %d", target->getPosition().x, target->getPosition().y);
			break;
		}
	}
	return true;
}

void BattleHorde::runBack(){

	Unitset uset;
	for (Unit u : selfTroops){
		if (u->exists()){
			//uset = u->getUnitsInWeaponRange()
		}
	}
}

//thêm quân
void BattleHorde::addUnit(BWAPI::Unit u){
	selfTroops.insert(u);
	if (defensePosition == Positions::None || Positions::Unknown)
		defensePosition = u->getPosition();
}

//lấy danh sách quân
Unitset BattleHorde::getCurrentList(){
	return selfTroops;
}

//thêm mục tiêu
void BattleHorde::addTarget(BWAPI::Unit targetInp){
	this->target = targetInp;
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
	return 2;
}

void BattleHorde::attack(Unit u){
	if (selfTroops.attack(u))
		selfTroops.attack(u->getPosition());
}

void BattleHorde::drawPosition(Position p){
	Broodwar->drawBoxMap(Position(p.x-10,p.y-10), Position(p.x + 10, p.y + 10), this->squadColor);
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
	(LordCommander::getInstance())->removeTarget(target, this);
}