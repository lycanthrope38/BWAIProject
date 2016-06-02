#ifndef BattleSquad_h
#define BattleSquad_h
#pragma once
#include "BattleHorde.h"
#include "BattleSquad.h"
#include "BuidingManager.h"
#include <BWAPI.h>

class BattleHorde;
class BattleField;
//Class này là một nhóm quân cùng UnitType thường sẽ có 9-12 unit

//xử lí một nhóm quân trong một trận chiến
/*Xử lí một nhóm quân trong một trận chiến
 *Việc chia mục tiêu sẽ chi tiết đến từng quân lính một
 *Áp dụng tư tưởng chia để trị.
 */
class BattleSquad
{
private:
	//tổng quân số của đơn vị
	BWAPI::Unitset squad;
	//những đơn vị đang bận
	BWAPI::Unitset busySquad;
	//những đơn vị rảnh rỗi
	BWAPI::Unitset freeSquad;
	//những squad nhỏ hơn sau khi chia nhỏ để trị các mục tiêu riêng lẻ
	std::set<BattleHorde*> hordes;
	/*số unit trong mỗi nhóm nhỏ sẽ được chia nhỏ(nếu có) trong giao tranh
	 *mục đích của việc chia nhỏ theo một tỉ lệ là dùng lợi thế về số lượng để diệt một unit nguy hiểm của đối phương
	 *nhanh nhất có thế, tỉ lệ này sẽ được tính dựa trên độ nguy hiểm, số lượng máu và một số thuộc tính khác của đối phương
	 */
	int devider, counter;
	//mục tiêu của đơn vị trong giao tranh
	BWAPI::Unitset targets;
	//các mục tiên đã được phát lệnh tấn công
	BWAPI::Unitset targetsOnRisk;
	//có đang làm nhiệu vụ hay ko
	bool isOnDuty;
	BWAPI::UnitType squadUnitType;
	//owner
	BattleField* owner;

public:
	BattleSquad(BWAPI::Unitset selfUnitSet);
	BattleSquad(BWAPI::Unitset selfUnitSet, BWAPI::Unitset enemyUnitSet);
	//xử lý mỗi frame
	bool onFrame();
	//xử lý mỗi frame nhưng riêng cho mục tiêu phòng thủ
	bool onDefend();
	//ghép cặp đánh nhau
	bool matching();
	//gọi các hàm onFrame của các BattleHorde
	void callBelowOnFrames();
	//Cho squad tấn công mục tiêu
	void attack(BWAPI::Unitset targets, int devidedBy);
	//Thêm Unit vào squad
	void addUnit(BWAPI::Unit u);
	//Di chuyển nhóm quân đến địa điểm nào đó
	void move(BWAPI::Position position);
	//Thêm một Squad vào squad
	void addSquad(BWAPI::Unitset unitSet);
	//xóa hết danh sách quân
	void clearSquad();
	//Tính toán tỉ lệ chia
	int calculateSplitRatio(BWAPI::Unit u, int enemyVol);
	//Kiểm tra xem đã hoàn thành nhiệm vụ hay chưa
	bool isCompleted();
	//Kiểm tra xem nhóm quân đã bị quét sạch(chết sạch) hay chưa
	bool isWiped();
	//Lấy danh sách quân ta hiện tại
	std::vector<BWAPI::Unit> getCurrentSelfList();
	//Lấy danh sách quân địch hiện tại
	std::vector<BWAPI::Unit> getCurrentEnemyList();
	//clear dead unit
	void clearDeadUnit(BattleHorde*,BWAPI::Unit);
	BWAPI::UnitType getSelfType(){
		for (BWAPI::Unit u:squad)
		{
			return u->getType();
		}
	}

	int getSelfSize(){
		return squad.size();
	}

	friend bool operator < (const BattleSquad &left, const BattleSquad &right)
	{
		return !((left.squad == right.squad) &&(left.targets == right.targets));
	}
	~BattleSquad();
};

#endif