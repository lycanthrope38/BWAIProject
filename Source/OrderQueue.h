#pragma once
/*Class này dùng để chứa hàng đợi các lệnh order như xây nhà, mua lính và nâng cấp
 *Khi cần thực thi một lệnh thì chỉ cần push vào hàng đợi.  
 *Nếu là một lệnh bình thường thì dùng độ ưu tiên là PRIORITY_NORMAL
 *Nếu trong tình trạng khẩn cấp thì mới dùng PRIORITY_HIGH
 **/
#include <BWAPI.h>
#include "OrderType.h"3

class OrderQueue
{
private:
	//hàng đợi thực thi các lệnh như xây nhà và mua lính
	std::vector<OrderType> queue;
public:
	
	//high thì order sẽ được chèn vào giữa hàng đợi
	const int PRIORITY_HIGH = 0;
	//normal thì order sẽ được chèn vào cuối hàng đợi
	const int PRIORITY_NORMAL = 1;

	OrderQueue();
	//hàm thực thi order
	bool excutive();
	//hàm đẩy order vào hàng đợi. sử dụng các PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
	bool push(BWAPI::UnitType* unitType, int priority);
	//hàm đẩy upgrade vào hàng đợi. sử dụng các PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
	bool push(BWAPI::UpgradeType* unitType, int priority);
	//hủy yêu cầu
	bool cancel(int queueIndex);
	//lấy số phần tử đang được chờ thực thi
	int getSize();
	//xử lý các yêu cầu xây dựng
	bool build(BWAPI::UnitType* buildingType);
	//xử lí các yêu cầu mua quân lính
	bool train(BWAPI::UnitType* forceType);
	//xử lý các yêu cầu nâng cấp
	bool upgrade(BWAPI::UpgradeType* upgradeType);
	~OrderQueue();
};
