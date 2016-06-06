#include "TargetManager.h"
#include "Collections.h"

#define MAX_PROCESS_RANGE 450
#define ATTACKABLE_CONST 3000
#define RELOAD_FRAME_COUNT 5

TargetManager::TargetManager()
{
	lastGetEnemyFrame = - RELOAD_FRAME_COUNT;
}
//hàm heuristic tính độ ưu tiên để tiêu diệt mục tiêu, lấy ra mục tiêu cần tiêu diệt sớm nhất
Unit TargetManager::getTarget(Unit myunit){

	//công thức tính độ ưu tiên của mỗi Unit
	//ratio = (DestroyScore+AttachableConst)/(Health+Distance)
	// ta sẽ tiêu diệt các mục tiêu có ratio cao trước
	// AttackableConst sẽ được cộng vào nếu enemy unit có khả năng tấn công unit bên mình
	
	Unit bestUnit = nullptr;
	double bestRatio = 0.0, tmpRatio = 0.0;
	UnitType enemyUnitType;
	if (Broodwar->getFrameCount() - lastGetEnemyFrame > RELOAD_FRAME_COUNT){
		enemyList = Broodwar->getUnitsInRadius(myunit->getPosition(), MAX_PROCESS_RANGE, Filter::IsEnemy);
		lastGetEnemyFrame = Broodwar->getFrameCount();
	}
	if (enemyList.size() != 0){
		for (Unit u : enemyList){
			if (u->exists())
				if (myunit->canAttackUnit(u)){
					enemyUnitType = u->getType();
					if (u->canAttack(myunit)){
						tmpRatio = (enemyUnitType.destroyScore() + ATTACKABLE_CONST) / (enemyUnitType.maxHitPoints() + Collections::distance(u->getPosition(), myunit->getPosition()));
						if (tmpRatio > bestRatio){
							bestRatio = tmpRatio;
							bestUnit = u;
						}
					}
					else{
						tmpRatio = (enemyUnitType.destroyScore()) / (enemyUnitType.maxHitPoints() + Collections::distance(u->getPosition(), myunit->getPosition()));
						if (tmpRatio > bestRatio){
							bestRatio = tmpRatio;
							bestUnit = u;
						}
					}
				}
			}
	}
	return bestUnit;
}

TargetManager::~TargetManager()
{
}
