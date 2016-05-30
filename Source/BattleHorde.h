#pragma once
#include <set>
#include <BWAPI.h>
class BattleHorde
{

private:
	//mục tiêu
	BWAPI::Unit target;
	//danh sách quân ta trong toán quân
	BWAPI::Unitset selfTroops;
	//kiểu lính của quân ta
	BWAPI::UnitType selfType;
	//vị trí cũ của selfTroops - phục vụ cho hit and run
	BWAPI::Position troopsStartMovingPos;
	//xem selfTroops có đang di chuyển hay k
	bool isMoving;
public:

	//xử lý mỗi frame
	bool onFrame();
	//nạp nhóm quân và mục tiêu để tiến hành tấn công
	BattleHorde(BWAPI::Unitset, BWAPI::Unit);
	//kiểm tra xem mục tiêu đã chết hoặc quân ta đã chết hết hay chưa
	bool isCompleted();
	//thêm quân
	void addUnit(BWAPI::Unit);
	//lấy số quân
	int getTroopSize();
	//lấy danh sách quân
	std::set<BWAPI::Unit> getCurrentList();
	//thêm một nhóm quân
	void addUnits(BWAPI::Unitset);
	//thêm mục tiêu
	void addTarget(BWAPI::Unit);
	//
	int getSelfSize(){
		return selfTroops.size();
	}

	friend bool operator < (const BattleHorde &left, const BattleHorde &right)
	{
		return !((left.target == right.target) && (left.selfTroops == right.selfTroops));
	}
	~BattleHorde();
};

