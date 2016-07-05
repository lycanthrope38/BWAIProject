#pragma once
#include <BWAPI.h>
#include "BattleHorde.h"

using namespace std;
using namespace BWAPI;

//chỉ huy của tất cả các trận chiến trên bản đồ
class LordCommander
{

private:
	static bool isInitedInstance;
	static LordCommander* instance;

	//lưu danh sách quân lính đang tấn công lên từng quân lính địch trong trận
	map<BWAPI::Unit, set<Unit>> enemyAttackedBy;
	map<BWAPI::Unit, int> meleeAttackersOnTarget;
	//tổng destroy score của lính bên ta
	/*int selfFighterScore;*/

	bool isStartAttack;

	int onFrameCounter;
	//which Horde an Unit are located
	map<BWAPI::Unit, BattleHorde*> unitManager;
	
	set<BattleHorde*> hordeManager;

	set<BattleHorde*> groundMeleeSet;
	set<BattleHorde*> groundFarAttackSet;
	set<BattleHorde*> airAttackSet;
	set<BattleHorde*> detectorSet;

	//Constructor. Vì dùng Singleton Pattern nên constructor private
	LordCommander();


public:
	static LordCommander* getInstance(){
		if (isInitedInstance)
		{
			return instance;
		}
		else
		{
			instance = new LordCommander();
			isInitedInstance = true;
			return instance;
		}
	}

	//tình trạng hiện tại của unit đã được sử dụng hay chưa
	static std::map<BWAPI::Unit, bool> isUsedUnit;
	void onFrame();
	void onDefend();
	void onAttack();
	bool addUnit(BWAPI::Unit u);
	void addHorde(BWAPI::Unit u);
	void removeDeadUnit(Unit u);
	void requireUnit(BattleHorde*, UnitType, int);
	void initArmy();
	void checkAttack();

	// khi giao tranh ta thấy có các trường hợp như:
	// lính mục tiêu có lượng máu và giáp ít. ta cần số ít quân để tiêu diệt mục tiêu này.
	// đối với các mục tiêu có sức chịu đựng lớn, ta sẽ dồn nhiều quân để tiêu diệt nó. 
	// hàm này dùng để check xem có nên dồn quân tiếp vào unit địch không
	//bool shouldAttackThis(BattleHorde* selfHorde, Unit enemy);

	//tính toán giá trị lược lượng của ta đã dồn vào mục tiêu
	int getSelfScoreOnTarget(Unit enemy);

	/*int getSelfScore(){
		return selfFighterScore;
	}*/

	int getMeleeAttackersOnTarget(BWAPI::Unit);
	void regTarget(BWAPI::Unit enemy, Unit selfUnit);
	void removeTarget(BWAPI::Unit enemy, Unit selfUnit);
	void reforce(BattleHorde* b);
	void totalAttack();
	void reform();
	//check xem có thể reforce được hay không
	bool isReforcable(BattleHorde*b);
	//thêm các đơn vị quân lính chưa có trong danh sách
	//void addFreeUnit();
	//khởi tạo cho lực lượng 
	//BattleField initMainBaseDefense();
	~LordCommander();
};

