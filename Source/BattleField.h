#pragma once
#include <vector>
#include <BWAPI.h>
#include "BattleSquad.h"
//xử lý một trận chiến trên bản đồ
class BattleField
{
private:
	std::vector<BattleSquad> selfForces;
	std::vector<BattleSquad> enemyForces;


public:
	BattleField();
	//phân tích và phân loại quân ta trong một trận chiến
	void analyzeSelfForces(BWAPI::Unitset selfUnitSet);
	//phân tích và phân loại quân địch trong một trận chiến
	void analyzeEnemyForces(BWAPI::Unitset enemyUnitSet);
	/*tối ưu việc chọn mục tiêu 
	 *tập trung đánh vào những quân nguy hiểm nhất trước
	 *tập trung tiêu diệt mỗi đơn vị càng nhanh càng tốt
	 */
	void optimizeTarget();
	~BattleField();
};

