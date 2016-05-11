﻿/*Class này dùng để chứa hàng đợi các lệnh order như xây nhà, mua lính và nâng cấp
*Khi cần thực thi một lệnh thì chỉ cần push vào hàng đợi.
*Nếu là một lệnh bình thường thì dùng độ ưu tiên là PRIORITY_NORMAL
*Nếu trong tình trạng khẩn cấp thì mới dùng PRIORITY_HIGH
**/
#include "OrderQueue.h"
#include "ArmyOrder.h"
#include <BWAPI.h>

OrderQueue::OrderQueue() :ArmyOrder(BWAPI::Broodwar->self()){
}
//hàm thực thi order
bool OrderQueue::execute(){
	bool result;
	BWAPI::Broodwar->sendText("Executed. Queue size %d", queue.size());
	if ((this->queue.size()) == 0)
		return false;
	//kiểm tra xem có phải là nhà hoặc quân lính hay không
	if (this->queue.at(0).isUnit()){
		//nếu là nhà thì xây
		if (this->queue.at(0).isBuilding()){

			//truyền vào this->queue.at(0) sai
			BWAPI::UnitType unitType = this->queue.at(0).getUnit();
			return build(unitType);

		}
		//nếu là lính thì train
		else
			return training();
	}
	//nếu là upgrade thì upgrade
	else
		return upgrade(queue.at(0).getUpgrade());
}

//hàm đẩy order nhà vào hàng đợi. sử dụng các biến static PRIORITY_VERY_HIGH, PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UnitType unitType, int priority){
	BWAPI::Broodwar->sendText("PUSED %d",unitType.getName());
	switch (priority)
	{
	case 0:
		this->queue.insert(queue.begin(),OrderType(unitType));
		return true;
	case 1:
		this->queue.push_back(OrderType(unitType));
		return true;
	default:
		return false;
	}
}
//hàm đẩy order lính vào hàng đợi. sử dụng các biến static PRIORITY_VERY_HIGH, PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UnitType unitType, BWAPI::UnitType parentUnit,int vol, int priority){
	BWAPI::Broodwar->sendText("PUSED %d", unitType.getName());
	switch (priority)
	{
	case 0:
		this->queue.insert(queue.begin(), OrderType(unitType, parentUnit,vol));
		return true;
	case 1:
		this->queue.push_back(OrderType(unitType, parentUnit, vol));
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

//xử lý các yêu cầu xây dựng
bool OrderQueue::build(BWAPI::UnitType buildingType){
	for (BWAPI::Unit u : BWAPI::Broodwar->self()->getUnits()){
		if (u->getType().isWorker()){
			//BWAPI::UnitType buildingType = BWAPI::UnitTypes::Protoss_Gateway;
			BWAPI::Broodwar->sendText("Building");
			BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(buildingType, u->getTilePosition());
			if (targetBuildLocation)
			{
				// Order the builder to construct the supply structure
				if (BWAPI::Broodwar->self()->minerals() >= buildingType.mineralPrice() && BWAPI::Broodwar->self()->gas() >= buildingType.gasPrice()){
					u->build(buildingType, targetBuildLocation);
					// Register an event that draws the target build location
					BWAPI::Broodwar->registerEvent([targetBuildLocation, buildingType](BWAPI::Game*)
					{
						BWAPI::Broodwar->drawBoxMap(BWAPI::Position(targetBuildLocation),
							BWAPI::Position(targetBuildLocation + buildingType.tileSize()),
							BWAPI::Colors::Red);
					},
						nullptr,  // condition
						buildingType.buildTime() + 100);  // frames to run

					queue.erase(queue.begin());
					return true;
				}
			}
		}
	}
	return false;
}

//xử lí các yêu cầu mua quân lính
bool OrderQueue::training(){
	//nếu train được thì return true và xóa phần tử đầu tiên trong hàng đợi
	//this->queue.erase(queue.begin());
	//nếu upgrade không được thì tăng số lần failed lên và kiểm tra số lần failed. 
	// nếu failed 3 lần thì đẩy phần tử này xuống cuối hàng đợi
	if (queue.size() == 0)
		return true;
	if (train(queue.at(0))){
		queue.erase(queue.begin());
		return true;
	}
	else{
		queue.at(0).failed++;
		BWAPI::Broodwar->sendText("Training failed %d", queue.at(0).failed);
		if (queue.at(0).failed > 2){
			queue.at(0).failed = 0;
			OrderType tmp = OrderType(queue.at(0));
			queue.erase(queue.begin());
			queue.push_back(tmp);
		}
		return false;
	}
}

//xử lý các yêu cầu nâng cấp
bool OrderQueue::upgrade(BWAPI::UpgradeType upgradeType){
	//nếu upgrade được thì return true và xóa phần tử đầu tiên trong hàng đợi
	//this->queue.erase(queue.begin());
	//nếu upgrade không được thì tăng số lần failed lên và kiểm tra số lần failed. 
	// nếu failed 3 lần thì đẩy phần tử này xuống cuối hàng đợi
	return false;
}

OrderQueue::~OrderQueue()
{
}