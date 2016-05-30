#pragma once
#include <vector>
#include <BWAPI.h>
#include "BattleSquad.h"
#include "UnitNumber.h"

//xử lý một trận chiến trên bản đồ
using namespace BWAPI;
class BattleField
{
private:


	int currentPositionState;
	int lastCheckedFrame;

	std::set<UnitNumber> requiredUnit;

	Unitset selfUnit;

	std::set<BattleSquad> selfForces;
	std::set<BattleSquad> enemyForces;
	//địa điểm chính để phòng thủ
	BWAPI::Position defendRoot;
	
public:
	const static float  xDefensePosition[];
	const static float  yDefensePosition[];
	BattleField();
	BattleField(BWAPI::Unitset uset, BWAPI::Position defendRoot);
	bool isDefend;
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
	void onAttack();

	//chỉ huy các đơn vị phòng thủ
	void onDefend();

	//tổng tấn công
	void allAttack();

	//thêm unit
	void addUnits(BWAPI::Unitset);

	//kiểm tra tình hình quân số
	void checkRequirements();

	int getSelfSize();

	bool isNearDefensePosition(Position);

	friend bool operator < (const BattleField &left, const BattleField &right)
	{
		return ((left.selfForces < right.selfForces) && (left.enemyForces < right.enemyForces));
	}
	~BattleField();
};

