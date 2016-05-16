#pragma once
<<<<<<< HEAD
=======
<<<<<<< HEAD
#include "BuidingManager.h"
//xử lí một nhóm quân trong một trận chiến
=======
>>>>>>> 9009a9f1c437d822bdc485c5b05a3c97727a9101
#include <BWAPI.h>
/*Xử lí một nhóm quân trong một trận chiến
 *Việc chia mục tiêu sẽ chi tiết đến từng quân lính một
 *Áp dụng tư tưởng chia để trị.
 */
<<<<<<< HEAD
=======
>>>>>>> master
>>>>>>> 9009a9f1c437d822bdc485c5b05a3c97727a9101
class BattleSquad
{
private:
	//tổng quân số của đơn vị
	std::set<BWAPI::Unit*> squad;
	//những đơn vị đang bận
	std::set<BWAPI::Unit*> busySquad;
	//những đơn vị rảnh rỗi
	std::set<BWAPI::Unit*> freeSquad;
	//những squad nhỏ hơn sau khi chia nhỏ để trị các mục tiêu riêng lẻ
	std::set<BattleSquad> dutySquad;
	/*số unit trong mỗi nhóm nhỏ sẽ được chia nhỏ(nếu có) trong giao tranh
	 *mục đích của việc chia nhỏ theo một tỉ lệ là dùng lợi thế về số lượng để diệt một unit nguy hiểm của đối phương 
	 *nhanh nhất có thế, tỉ lệ này sẽ được tính dựa trên độ nguy hiểm, số lượng máu và một số thuộc tính khác của đối phương
	 */
	int devider;
	//mục tiêu của đơn vị trong giao tranh
	std::vector<BWAPI::Unit*> targets;
	//có đang làm nhiệu vụ hay ko
	bool isOnDuty;

public:
	BattleSquad();
	//Cho squad tấn công mục tiêu
	void attack(BWAPI::Unitset targets, int devidedBy);
	//Cho squad tấn công mục tiêu
	void attack(std::vector<BWAPI::Unit*> targets, int devidedBy);
	//Thêm Unit vào squad
	void addUnit(BWAPI::Unit* u);
	//Di chuyển nhóm quân đến địa điểm nào đó
	void move(BWAPI::Position position);
	//Thêm một Squad vào squad
	void addSquad(BWAPI::Unitset unitSet);
	//Thêm một Squad vào squad
	void addSquad(std::vector<BWAPI::Unit*> unitSet);
	//xóa hết danh sách quân
	void clearSquad();
	//Tính toán tỉ lệ chia
	int calculateSplitRatio(BWAPI::Unit u, int enemyVol);
	//Kiểm tra xem đã hoàn thành nhiệm vụ hay chưa
	bool isCompleted();
	//Kiểm tra xem nhóm quân đã bị quét sạch(chết sạch) hay chưa
	bool isWiped();
	~BattleSquad();
};

