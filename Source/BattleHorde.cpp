#include "BattleHorde.h"
#include "LordCommander.h"
#include <BWAPI.h>

using namespace BWAPI;

const int BattleHorde::INFINITY_LIFE_TIME = 999999999;
int BattleHorde::maxDefenseRange = 400;

BattleHorde::BattleHorde(UnitType type, int startFrame, int endFrame)
{
	isMoving = false;
	isOrdered = false;
	isHoldPosition = true;
	defensePosition = Positions::None;
	selfType = type;
	this->startFrame = startFrame;
	this->endFrame = endFrame;
	maxUnit = calculateMaxUnit(type);
}

//xử lý mỗi frame
bool BattleHorde::onFrame(){
	//kiểm tra nếu bên ta có loại đánh xa thì áp dụng hit and run

	if (isHoldPosition){
		if (defensePosition == Positions::None){
			for (Unit u : Broodwar->self()->getUnits()){
				if (u->getType().isBuilding())
				{
					defensePosition = u->getPosition();
					break;
				}
			}
		}
		else{
		//nhảy cha cha cha
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

	if (groundWeapon != WeaponTypes::None&&airWeapon != WeaponTypes::Unknown){
		if (groundWeapon.maxRange()<=35)
			return 3;
		else 
			return 5;
	}
}

BattleHorde::~BattleHorde()
{

}