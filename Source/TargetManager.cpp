#include "TargetManager.h"
#include "Collections.h"
#include "LordCommander.h"
#include "BattleHorde.h"

#define ENEMY_TARGET_CONST 70
#define MAX_PROCESS_RANGE 1000
#define ATTACKABLE_CONST 5000
#define RELOAD_FRAME_COUNT 5
#define NEAR_RANGE 35
#define SELF_ENEMY_TROOP_RATIO 5

TargetManager::TargetManager()
{
	lastGetEnemyFrame = -RELOAD_FRAME_COUNT;
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
	UnitType selfUnitType;
	bool isNearRange = false;
	float distance;
	//if (Broodwar->getFrameCount() - lastGetEnemyFrame > RELOAD_FRAME_COUNT){
	enemyList = Broodwar->getUnitsInRadius(myunit->getPosition(), MAX_PROCESS_RANGE, Filter::IsEnemy);
	//lastGetEnemyFrame = Broodwar->getFrameCount();
	//}

	if (enemyList.size() != 0){
		for (Unit u : enemyList){
			if (u->exists())
				if (myunit->canAttackUnit(u)){
					selfUnitType = myunit->getType();
					if (selfUnitType.groundWeapon() != WeaponTypes::None)
						if (selfUnitType.groundWeapon().maxRange() < NEAR_RANGE){
							isNearRange = true;
							if (u->isBurrowed() || u->isCloaked())
								continue;
						}

					enemyUnitType = u->getType();
					distance = (Collections::distance(u->getPosition(), myunit->getPosition()));
					//trường hợp không phải lính đánh gần thì áp dụng SELF_ENEMY_TROOP_RATIO
					if (!isNearRange){
						if (LordCommander::getInstance()->getSelfScoreOnTarget(u) > u->getType().destroyScore() * SELF_ENEMY_TROOP_RATIO){
							//Broodwar->sendText("Too much force on this target");
							continue;
						}
					}
					else{

						if ((u->canAttack(myunit))){
							tmpRatio = (enemyUnitType.destroyScore() + ATTACKABLE_CONST) / (distance*distance);
							if (tmpRatio > bestRatio){
								bestRatio = tmpRatio;
								bestUnit = u;
							}
						}
						else{
							tmpRatio = (enemyUnitType.destroyScore()) / (distance*distance);// / ( Collections::distance(u->getPosition(), myunit->getPosition()))
							if (tmpRatio > bestRatio){
								bestRatio = tmpRatio;
								bestUnit = u;
							}
						}
						continue;
					}
					if ((u->canAttack(myunit))){
						tmpRatio = (enemyUnitType.destroyScore() + ATTACKABLE_CONST) / (u->getHitPoints() + (distance*distance));
						if (tmpRatio > bestRatio){
							bestRatio = tmpRatio;
							bestUnit = u;
						}
					}
					else{
						tmpRatio = (enemyUnitType.destroyScore()) / (u->getHitPoints() + (distance*distance));
						if (tmpRatio > bestRatio){
							bestRatio = tmpRatio;
							bestUnit = u;
						}
					}
				}
		}
	}
	if (bestUnit == nullptr)
		for (Unit u : enemyList)
			if (!(u->isCloaked()) && !(u->isBurrowed()))
				bestUnit = u;
	return bestUnit;
}

TargetManager::~TargetManager()
{
}
