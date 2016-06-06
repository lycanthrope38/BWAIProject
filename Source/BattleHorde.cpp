#include "BattleHorde.h"
#include "LordCommander.h"
#include <BWAPI.h>

using namespace BWAPI;

const int BattleHorde::INFINITY_LIFE_TIME = 999999999;
int BattleHorde::maxDefenseRange = 400;

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
}

//xử lý mỗi frame
bool BattleHorde::onFrame(){
	//kiểm tra nếu bên ta có loại đánh xa thì áp dụng hit and run

	//if (isHoldPosition){
	//	if (defensePosition == Positions::None){
	//		for (Unit u : Broodwar->self()->getUnits()){
	//			if (u->getType().isBuilding())
	//			{
	//				defensePosition = u->getPosition();
	//				break;
	//			}
	//		}
	//	}
	//	else{
	//	//nhảy cha cha cha
	//		if (!target->exists()){
	//			for (Unit u : selfTroops){
	//				if (u->exists()){
	//					target = targetManager.getTarget(u);
	//					selfTroops.attack(target);
	//					Broodwar->printf("Enemy detected at %d  %d", target->getPosition().x, target->getPosition().y);
	//					break;
	//				}
	//			}
	//		}
	//		else{
	//			selfTroops.attack(target);
	//			Broodwar->printf("Enemy detected at %d  %d", target->getPosition().x, target->getPosition().y);
	//		}

	//	}
	//}
		for (Unit u : selfTroops){
			if (u->exists()){
				target = targetManager.getTarget(u);
				if (target == nullptr){
					//Broodwar->sendText("Target == nullptr");
					break;
				}
				selfTroops.attack(target);
				Broodwar->sendText("Enemy detected at %d  %d", target->getPosition().x, target->getPosition().y);
				break;
			}
		}
	return true;
}

//thêm quân
void BattleHorde::addUnit(BWAPI::Unit u){
	selfTroops.insert(u);
}

//lấy số quân
int BattleHorde::getTroopSize(){}

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
	if (airWeapon != WeaponTypes::None&&airWeapon != WeaponTypes::Unknown){
		if (type.destroyScore() < 900)
			return 5;
		else
			return 3;
	}

	if (groundWeapon != WeaponTypes::None&&groundWeapon != WeaponTypes::Unknown){
		if (groundWeapon.maxRange()<=35)
			return 3;
		else 
			return 5;
	}
	return 3;
}

BattleHorde::~BattleHorde()
{

}