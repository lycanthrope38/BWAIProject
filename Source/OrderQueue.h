#pragma once
/*Class này dùng để chứa hàng đợi các lệnh order như xây nhà, mua lính và nâng cấp
*Khi cần thực thi một lệnh thì chỉ cần push vào hàng đợi.
*Nếu là một lệnh bình thường thì dùng độ ưu tiên là PRIORITY_NORMAL
*Nếu trong tình trạng khẩn cấp thì mới dùng PRIORITY_HIGH
**/
#include <BWAPI.h>
#include "OrderType.h"
#include "ArmyOrder.h"

#include "BuidingManager.h"

class StaticOrder;
using namespace BWAPI;

class OrderQueue : public ArmyOrder
{
private:

	static OrderQueue* instance;
	static bool initedInstance;

	//hàng đợi thực thi các lệnh như xây nhà và mua lính
	std::vector<OrderType> queue;
	BuidingManager* manager;
	StaticOrder* staticOrder;
	//Constructor. Singleton Pattern nên constructor private
	OrderQueue();
public:

	static OrderQueue* getInstance(){
		if (!initedInstance)
		{
			instance = new OrderQueue();
			initedInstance = true;
			return instance;
		}
		else
		{
			return instance;
		}
	}

	//high thì order sẽ được chèn vào đầu hàng đợi
	static const int PRIORITY_HIGH = 0;
	//normal thì order sẽ được chèn vào cuối hàng đợi
	static const int PRIORITY_NORMAL = 1;

	bool isAssimilatorBuilt=false;
	//hàm thực thi order
	bool execute();
	//hàm thực thi order
	bool execute(OrderType*);
	//hàm đẩy order nhà vào hàng đợi. sử dụng các PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
	bool push(BWAPI::UnitType unitType, int priority);
	//hàm đẩy order nhà vào hàng đợi có kèm ràng buộc số dân. sử dụng các PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
	bool push(BWAPI::UnitType unitType, int priority, int supplyRequired);
	//hàm đẩy order lính vào hàng đợi. sử dụng các PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
	bool push(BWAPI::UnitType unitType, BWAPI::UnitType parentUnit, int volume, int priority);
	//hàm đẩy upgrade vào hàng đợi. sử dụng các PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
	bool push(BWAPI::UpgradeType unitType, int priority);
	//hủy yêu cầu
	bool cancel(int queueIndex);
	//lấy số phần tử đang được chờ thực thi
	int getSize();
	//xử lý các yêu cầu xây dựng
	bool build(BWAPI::UnitType buildingType);
	//xử lí các yêu cầu mua quân lính
	bool training();
	//xử lí các yêu cầu mua quân lính
	bool training(OrderType* orderType);
	//xử lý các yêu cầu nâng cấp
	bool upgrade(BWAPI::UpgradeType upgradeType);
	//xem một unit đã được đẩy vô hàng đợi hay chưa
	bool checkExistInQueue(UnitType);

	//xử lý kết quả khi execute
	bool resultAnalyze(bool result);
	//tìm cách xây các nhà cần thiết để tạo ra một unit nào đó
	void buildRequiredFor(BWAPI::UnitType u);
	//kiểm tra một unittype đã tồn tại hay chưa
	bool checkExist(BWAPI::UnitType type);
	~OrderQueue();
};