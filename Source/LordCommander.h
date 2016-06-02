#pragma once
#include <BWAPI.h>
#include "BattleHorde.h"
//Class này sẽ quản lý tất cả các Unit trên bản đồ. Thường sẽ chia ra gồm 1-3 BattleField
//1 BattleField phụ trách thường trực ở nhà. 2 phụ trách việc tấn công

using namespace std;
using namespace BWAPI;

//chỉ huy của tất cả các trận chiến trên bản đồ
class LordCommander
{

private:
	static bool initedInstance;
	static LordCommander* instance;

	//BattleField testDefender;
	bool isInited;
	int onFrameCounter;
	
	map<Unit, BattleHorde*> unitManager;

	vector<BattleHorde*> hordeManager;

	//Constructor. Vì dùng Singleton Pattern nên constructor private
	LordCommander();

public:
	static LordCommander* getInstance(){
		if (!initedInstance)
		{
			instance = new LordCommander();
			initedInstance = true;
			return instance;
		}
		else
		{
			return instance;
		}
	}

	//tình trạng hiện tại của unit đã được sử dụng hay chưa
	static std::map<BWAPI::Unit, bool> isUsedUnit;
	void onFrame();
	void onDefend();
	void onAttack();
	void addUnit(BWAPI::Unit u);
	void removeDeadUnit(Unit u);
	void requireUnit(BattleHorde*, UnitType, int);
	void initArmy();
	//thêm các đơn vị quân lính chưa có trong danh sách
	//void addFreeUnit();
	//khởi tạo cho lực lượng 
	//BattleField initMainBaseDefense();
	~LordCommander();
};

