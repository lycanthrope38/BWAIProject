#ifndef BattleHorde_h
#define BattleHorde_h
#pragma once
#include <set>
#include <BWAPI.h>
#include "TargetManager.h"

using namespace BWAPI;


class BattleHorde
{

private:

	static vector<Color> squadColorList;
	static bool isInited;

	bool isAirAttackable;

	Color squadColor;

	//mục tiêu
	BWAPI::Unit target;
	//danh sách quân ta trong toán quân
	BWAPI::Unitset selfTroops;
	//kiểu lính của quân ta
	BWAPI::UnitType selfType;
	//xem selfTroops có đang di chuyển hay k
	bool isMoving;
	//xem đã Order lượng lính thiếu hay chưa
	bool isOrdered;
	//số lượng unit tối đa trong Horde
	int maxUnit;
	//thời gian tồn tại tối đa của Horde
	int endFrame;
	//vị trí phòng thủ
	Position defensePosition;

	int lastAttackCall;

	TargetManager targetManager;

	static int maxDefenseRange;
	int calculateMaxUnit(UnitType);
public:
	//xem có phải Unit này đang giữ vị trí/thủ hay không
	bool isHoldPosition;
	bool isAttacked;
	int lastOrderFrame;
	static const int INFINITY_LIFE_TIME;
	//xử lý mỗi frame
	bool onFrame();
	//nạp nhóm quân và mục tiêu để tiến hành tấn công, startFrame là thời gian bắt đầu việc Order lính, endFrame là thời gian kết thúc OrderLính 
	BattleHorde(UnitType type, int endFrame);
	//thêm quân
	void addUnit(BWAPI::Unit);
	//lấy danh sách quân
	Unitset getCurrentList();
	//thêm mục tiêu
	void addTarget(BWAPI::Unit);
	//clear dead unit
	void clearDeadUnit(Unit u);

	int getMaxUnit(){
		return maxUnit;
	}

	int getSelfSize(){
		return selfTroops.size();
	}
	
	UnitType getUnitType(){
		return selfType;
	}

	bool isFullUnit(){
		return (selfTroops.size() >= (size_t)maxUnit);
	}

	void move(Position p);

	void drawPosition(Position p);

	void drawSquad();

	void attack(Unit u);

	void buyInterceptor();

	void runBack();

	friend bool operator < (const BattleHorde &left, const BattleHorde &right)
	{
		return !((left.target == right.target) && (left.selfTroops == right.selfTroops));
	}
	~BattleHorde();
};
#endif