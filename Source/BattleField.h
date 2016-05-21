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

	//cập nhật lại tình hình của quân ta và địch
	void update();

	//thực hiện các hành động
	void performActions();

	//chỉ huy các đơn vị đi tấn công
	void attack();

	//chỉ huy các đơn vị phòng thủ
	void defend();

	//tổng tấn công
	void allAttack();

	~BattleField();
};

