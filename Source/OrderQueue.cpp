﻿/*Class này dùng để chứa hàng đợi các lệnh order như xây nhà, mua lính và nâng cấp
*Khi cần thực thi một lệnh thì chỉ cần push vào hàng đợi.
*Nếu là một lệnh bình thường thì dùng độ ưu tiên là PRIORITY_NORMAL
*Nếu trong tình trạng khẩn cấp thì mới dùng PRIORITY_HIGH
**/
#include "OrderQueue.h"
#include "ArmyOrder.h"

using namespace std;
OrderQueue::OrderQueue()
{
}

//hàm thực thi order
bool OrderQueue::excutive(BWAPI::Unit unitBuilder){
	if ((this->queue.size()) == 0)
		return false;
	//kiểm tra xem có phải là nhà hoặc quân lính hay không
	if (this->queue.at(0).isUnit()){
		//nếu là nhà thì xây
		if (this->queue.at(0).isBuilding())
			return build(this->queue.at(0).getUnit(), unitBuilder);
		//nếu là lính thì train
		else
			return train(this->queue.at(0).getUnit());
	}
	//nếu là upgrade thì upgrade
	else
		return upgrade(queue.at(0).getUpgrade());
}

//hàm đẩy order vào hàng đợi. sử dụng các biến static PRIORITY_VERY_HIGH, PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UnitType unitType, int priority){
	switch (priority)
	{
	case 0:
		this->queue.insert(queue.begin(),OrderType(unitType));
		return true;
	case 1:
		this->queue.push_back(OrderType(unitType));//chuột phải goto 
		return true;
	default:
		return false;
	}
}

//hàm đẩy upgrade vào hàng đợi. sử dụng các biến static PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UpgradeType upgradeType, int priority){
	switch (priority)
	{
	case 0:
		this->queue.insert(queue.begin(), OrderType(upgradeType));
		return true;
	case 1:
		this->queue.push_back(OrderType(upgradeType));
		return true;
	default:
		return false;
	}
}
//hủy yêu cầu
bool OrderQueue::cancel(int queueIndex){
	if (queueIndex < queue.size()){
		this->queue.erase(queue.begin() + queueIndex);
		return true;
	}
	return false;
}

//lấy số phần tử đang được chờ thực thi
int OrderQueue::getSize(){
	return queue.size();
}

//xử lý các yêu cầu xây dựng
bool OrderQueue::build(BWAPI::UnitType buildingType,BWAPI::Unit unit){
	//nếu xây được thì return true

	BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(buildingType, unit->getTilePosition());
	if (targetBuildLocation)
	{
		// Order the builder to construct the supply structure
		if (BWAPI::Broodwar->self()->minerals() >= buildingType.mineralPrice() && BWAPI::Broodwar->self()->gas() >= buildingType.gasPrice()){
			unit->build(buildingType, targetBuildLocation);
			// Register an event that draws the target build location
			BWAPI::Broodwar->registerEvent([targetBuildLocation, buildingType](BWAPI::Game*)
			{
				BWAPI::Broodwar->drawBoxMap(BWAPI::Position(targetBuildLocation),
					BWAPI::Position(targetBuildLocation + buildingType.tileSize()),
					BWAPI::Colors::Red);
			},
				nullptr,  // condition
				buildingType.buildTime() + 100);  // frames to run

			return true;
		}
	}
	return false;
}

//xử lí các yêu cầu mua quân lính
bool OrderQueue::train(BWAPI::UnitType forceType){
	//nếu train được thì return true và xóa phần tử đầu tiên trong hàng đợi
	//this->queue.erase(queue.begin());
	bool result = trainZealot();
	if (result)
		queue.erase(queue.begin());
	return result;
}

//xử lý các yêu cầu nâng cấp
bool OrderQueue::upgrade(BWAPI::UpgradeType upgradeType){
	//nếu upgrade được thì return true và xóa phần tử đầu tiên trong hàng đợi
	//this->queue.erase(queue.begin());

	return false;
}

OrderQueue::~OrderQueue()
{
}