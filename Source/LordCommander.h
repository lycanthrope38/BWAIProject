#pragma once
#include "BattleField.h"
//chỉ huy của tất cả các trận chiến trên bản đồ
class LordCommander
{

private:
	static bool initedInstance;
	static LordCommander* instance;

	//BattleField testDefender;
	bool isInited;
	int onFrameCounter;
	
	//danh sách phân bố phòng thủ
	std::set<BattleField> defenders;
	//danh sách phân bố quân tấn công
	std::set<BattleField> attackers;
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
	//lưu các unit chưa được biên chế 
	static BWAPI::Unitset freeUnits;
	void onFrame();
	void onDefend();
	void onAttack();
	//thêm các đơn vị quân lính chưa có trong danh sách
	void addFreeUnit();
	//khởi tạo cho lực lượng 
	BattleField initMainBaseDefense();
	~LordCommander();
};

