#include "BattleHorde.h"

using namespace BWAPI;

BattleHorde::BattleHorde(BWAPI::Unitset troops, BWAPI::Unit target)
{
	selfTroops = troops;
	this->target = target;
	selfTroops.attack(target);
	for (Unit u : troops){
		selfType = u->getType;
		break;
	}
	isMoving = false;
	
}

//xử lý mỗi lần gọi
void BattleHorde::onFrame(){
	//kiểm tra nếu bên ta có loại đánh xa thì áp dụng hit and run
	
	

}

//kiểm tra xem mục tiêu đã chết hoặc quân ta đã chết hết hay chưa
bool BattleHorde::isCompleted(){
	if (target->exists())
		return false;
	return true;
}

//thêm quân
void BattleHorde::addUnit(BWAPI::Unit u){
	selfTroops.insert(u);
}

//lấy số quân
int BattleHorde::getTroopSize(){}

//lấy danh sách quân
std::set<BWAPI::Unit> BattleHorde::getCurrentList(){}

//thêm một nhóm quân
void BattleHorde::addUnits(BWAPI::Unitset troops){}

//thêm mục tiêu
void BattleHorde::addTarget(BWAPI::Unit targetInp){
	this->target = targetInp;
}

BattleHorde::~BattleHorde()
{

}
