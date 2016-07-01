/*Class này dùng để chứa hàng đợi các lệnh order như xây nhà, mua lính và nâng cấp
*Khi cần thực thi một lệnh thì chỉ cần push vào hàng đợi.
*Nếu là một lệnh bình thường thì dùng độ ưu tiên là PRIORITY_NORMAL
*Nếu trong tình trạng khẩn cấp thì mới dùng PRIORITY_HIGH
**/
#include "OrderQueue.h"
#include "ArmyOrder.h"
#include "BuidingManager.h"

#include <BWAPI.h>
#include <queue>

#define FAILURE_LIMIT 0

using namespace BWAPI;

OrderQueue* OrderQueue::instance = nullptr;
bool OrderQueue::initedInstance = false;

OrderQueue::OrderQueue() :ArmyOrder(BWAPI::Broodwar->self()){
}
//hàm thực thi order
bool OrderQueue::execute(){
	//bool result;

	OrderQueue* ins = getInstance();

	//BWAPI::Broodwar->sendText("Executed. Queue size %d", queue.size());
	if ((ins->queue.size()) == 0)
		return false;
	//kiểm tra xem có phải là nhà hoặc quân lính hay không
	if (ins->queue.at(0).isUnit()){
		//nếu là nhà thì xây
		if (ins->queue.at(0).isBuilding()){

			//nếu nhà có yêu cầu số dân thì kiểm tra xem số dân đã đủ hay chưa
			if (ins->queue.at(0).supplyRequire != -1){
				if (BWAPI::Broodwar->self()->supplyTotal() / 2 >= ins->queue.at(0).supplyRequire){
					//truyền vào this->queue.at(0) sai
					BWAPI::UnitType unitType = ins->queue.at(0).getUnitType();
					return ins->resultAnalyze(build(unitType));

				}
				else{
					//BWAPI::Broodwar->sendText("SupplyTotal %d is not enough to build! Required %d", (BWAPI::Broodwar->self()->supplyTotal() / 2), queue.at(0).supplyRequire);
					return ins->resultAnalyze(false);
				}
			}
			//nếu không yêu cầu số dân thì xây luôn
			else{
				BWAPI::UnitType unitType = ins->queue.at(0).getUnitType();
				return ins->resultAnalyze(build(unitType));
			}
		}
		//nếu là lính thì train
		else
			return ins->resultAnalyze(training());
	}
	//nếu là upgrade thì upgrade
	return ins->resultAnalyze(ins->upgrade(ins->queue.at(0).getUpgrade()));
}

bool OrderQueue::execute(OrderType* orderType){
	//bool result;

	OrderQueue* ins = getInstance();

	//kiểm tra xem có phải là nhà hoặc quân lính hay không
	if (orderType->isUnit()){
		//nếu là nhà thì xây
		if (orderType->isBuilding()){

			//nếu nhà có yêu cầu số dân thì kiểm tra xem số dân đã đủ hay chưa
			if (orderType->supplyRequire != -1){
				if (BWAPI::Broodwar->self()->supplyTotal() / 2 >= orderType->supplyRequire){
					//truyền vào this->queue.at(0) sai
					BWAPI::UnitType unitType = orderType->getUnitType();
					return build(unitType);
				}
				else{
					//BWAPI::Broodwar->sendText("SupplyTotal %d is not enough to build! Required %d", (BWAPI::Broodwar->self()->supplyTotal() / 2), queue.at(0).supplyRequire);
					return false;
				}
			}
			//nếu không yêu cầu số dân thì xây luôn
			else{
				BWAPI::UnitType unitType = orderType->getUnitType();
				return build(unitType);
			}
		}
		//nếu là lính thì train
		else
			return training(orderType);
	}
	//nếu là upgrade thì upgrade
	return ins->upgrade(orderType->getUpgrade());
}
bool OrderQueue::resultAnalyze(bool result){
	OrderQueue* ins = getInstance();
	if (result){
		ins->queue.erase(queue.begin());
		return true;
	}
	else{
		ins->queue.at(0).failed++;
	//	BWAPI::Broodwar->sendText("Training failed %d", queue.at(0).failed);
		if (ins->queue.at(0).failed > FAILURE_LIMIT){
			ins->queue.at(0).failed = 0;
			OrderType tmp = OrderType(ins->queue.at(0));
			ins->queue.erase(queue.begin());
			ins->queue.push_back(tmp);
			//BWAPI::Broodwar->sendText("Moved to the end of queue!");
		}
		return false;
	}
}

//hàm đẩy order nhà vào hàng đợi. sử dụng các biến static PRIORITY_VERY_HIGH, PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UnitType unitType, int priority){
	OrderQueue* ins = getInstance();
	//BWAPI::Broodwar->sendText("PUSED %d", unitType.getName());
	switch (priority)
	{
	case 0:
		ins->queue.insert(ins->queue.begin(), OrderType(unitType));
		return true;
	case 1:
		ins->queue.push_back(OrderType(unitType));
		return true;
	default:
		return false;
	}
}

//hàm đẩy order nhà vào hàng đợi có ràng buộc số dân. sử dụng các biến static PRIORITY_VERY_HIGH, PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UnitType unitType, int priority, int supplyRequired){
	//BWAPI::Broodwar->sendText("PUSED %d", unitType.getName());
	OrderQueue* ins = getInstance();
	switch (priority)
	{
	case 0:
		ins->queue.insert(ins->queue.begin(), OrderType(unitType, supplyRequired));
		return true;
	case 1:
		ins->queue.push_back(OrderType(unitType, supplyRequired));
		return true;
	default:
		return false;
	}
}
//hàm đẩy order lính vào hàng đợi. sử dụng các biến static PRIORITY_VERY_HIGH, PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UnitType unitType, BWAPI::UnitType parentUnit, int vol, int priority){
	//BWAPI::Broodwar->sendText("PUSED %d", unitType.getName());
	OrderQueue* ins = getInstance();
	switch (priority)
	{
	case 0:
		ins->queue.insert(ins->queue.begin(), OrderType(unitType, parentUnit, vol));
		return true;
	case 1:
		ins->queue.push_back(OrderType(unitType, parentUnit, vol));
		return true;
	default:
		return false;
	}
}
//hàm đẩy upgrade vào hàng đợi. sử dụng các biến static PRIORITY_HIGH và PRIORITY_NORMAL để đánh giá độ ưu tiên
bool OrderQueue::push(BWAPI::UpgradeType upgradeType, int priority){
	OrderQueue* ins = getInstance();
	switch (priority)
	{
	case 0:
		ins->queue.insert(ins->queue.begin(), OrderType(upgradeType));
		return true;
	case 1:
		ins->queue.push_back(OrderType(upgradeType));
		return true;
	default:
		return false;
	}
}


//hủy yêu cầu	
bool OrderQueue::cancel(int queueIndex){
	OrderQueue* ins = getInstance();
	if (queueIndex < static_cast<int>(queue.size())){
		ins->queue.erase(ins->queue.begin() + queueIndex);
		return true;
	}
	return false;
}

//lấy số phần tử đang được chờ thực thi
int OrderQueue::getSize(){
	return getInstance()->queue.size();
}

//xử lý các yêu cầu xây dựng
bool OrderQueue::build(BWAPI::UnitType buildingType){

	//Broodwar->sendText("ID   %d", buildingType.getID());

	OrderQueue* ins = getInstance();

	BuidingManager* manager = BuidingManager::newInstance();
	//BWAPI::Unit worker = manager.getBuildingWorker();

	Unit u = manager->getBuildingWorker();

	if (u!=nullptr)
	{

	//	BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(buildingType, u->getTilePosition());
	//	if (targetBuildLocation)
	//	{
			// Order the builder to construct the supply structure
			if (BWAPI::Broodwar->self()->minerals() >= buildingType.mineralPrice() && BWAPI::Broodwar->self()->gas() >= buildingType.gasPrice()){
				static int lastChecked = 0;


				if (lastChecked + 500 < BWAPI::Broodwar->getFrameCount())
				{

				//	if (u->build(buildingType, targetBuildLocation))
					//{

					if (buildingType==UnitTypes::Protoss_Pylon)
					{
						if (manager->createBuilding(u, buildingType))
						{
							lastChecked = BWAPI::Broodwar->getFrameCount();
							return true;
						}
					}
					else if (buildingType == UnitTypes::Protoss_Nexus)
					{
						manager->buildingExpand();
						return true;
						
					}
					else
					{
						BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(buildingType, u->getTilePosition());
						if (u->build(buildingType, targetBuildLocation))
						{
							lastChecked = BWAPI::Broodwar->getFrameCount();
							if (buildingType == BWAPI::UnitTypes::Protoss_Assimilator)
							{
								isAssimilatorBuilt = true;
							}
							return true;
						}
						
					}
						

						//// Register an event that draws the target build location
						//BWAPI::Broodwar->registerEvent([targetBuildLocation, buildingType](BWAPI::Game*)
						//{
						//	BWAPI::Broodwar->drawBoxMap(BWAPI::Position(targetBuildLocation),
						//		BWAPI::Position(targetBuildLocation + buildingType.tileSize()),
						//		BWAPI::Colors::Red);
						//},
						//	nullptr,  // condition
						//	buildingType.buildTime() + 100);  // frames to run

					

					//}

				}
			}
		//}

	}

	return false;

	//BuidingManager manager = BuidingManager();
	//BWAPI::Unit worker = manager.getBuildingWorker();

	//BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(buildingType, worker->getTilePosition());
	//		if (targetBuildLocation)
	//		{
	//			// Order the builder to construct the supply structure
	//			if (BWAPI::Broodwar->self()->minerals() >= buildingType.mineralPrice() && BWAPI::Broodwar->self()->gas() >= buildingType.gasPrice()){
	//				if (worker->build(buildingType, targetBuildLocation))
	//				{
	//					// Register an event that draws the target build location
	//					BWAPI::Broodwar->registerEvent([targetBuildLocation, buildingType](BWAPI::Game*)
	//					{
	//						BWAPI::Broodwar->drawBoxMap(BWAPI::Position(targetBuildLocation),
	//							BWAPI::Position(targetBuildLocation + buildingType.tileSize()),
	//							BWAPI::Colors::Red);
	//					},
	//						nullptr,  // condition
	//						buildingType.buildTime() + 100);  // frames to run

	//					return true;
	//				}
	//				
	//			}
	//		}

	//
	//			
	//return false;

}
//xử lí các yêu cầu mua quân lính
bool OrderQueue::training(){
	OrderQueue* ins = getInstance();
	////nếu train được thì return true và xóa phần tử đầu tiên trong hàng đợi
	////this->queue.erase(queue.begin());
	////nếu upgrade không được thì tăng số lần failed lên và kiểm tra số lần failed. 
	//// nếu failed 3 lần thì đẩy phần tử này xuống cuối hàng đợi
	if (ins->queue.size() == 0)
		return true;
	if (ins->train(ins->queue.at(0))){
		//queue.erase(queue.begin());
		return true;
	}
	return false;
}
//xử lí các yêu cầu mua quân lính
bool OrderQueue::training(OrderType* orderType){
	OrderQueue* ins = getInstance();

	if (ins->train(orderType)){
		return true;
	}
	return false;
}
//xử lý các yêu cầu nâng cấp
bool OrderQueue::upgrade(BWAPI::UpgradeType upgradeType){
	OrderQueue* ins = getInstance();

	int successed = 0, minUnit = -1;// Number of successed training / unit not training now
	std::vector<Unit> parentList = Collections::getUnitList(Broodwar->self(), upgradeType.whatUpgrades());
	//định tìm nhà có số lính đang train nhỏ nhất nhưng không thấy hàm đó nên tìm nhà chưa train lính để order lính trước
	for (int i = 0; i < parentList.size(); i++){
		if ((parentList.at(i))->upgrade(upgradeType)){
			return true;
		}
	}
	return false;
}

void OrderQueue::buildRequiredFor(BWAPI::UnitType u){
	map<UnitType, int> requireds = u.requiredUnits();

	//UnitType requiredUnit = (u.whatBuilds()).first;

	//check if a unittype is exist
	/*if (getInstance()->checkExist(requiredUnit)){
		return;
	}
	else{
		getInstance()->buildRequiredFor(requiredUnit);
		getInstance()->push(requiredUnit, PRIORITY_HIGH);
	}	*/

	for (pair<UnitType, int> typePair : requireds){
		if ((getInstance()->checkExist(typePair.first))||(getInstance()->checkExistInQueue(typePair.first)))
			break;
		else{
			getInstance()->push(typePair.first, PRIORITY_HIGH);
			getInstance()->buildRequiredFor(typePair.first);
		}
	}
}

bool OrderQueue::checkExistInQueue(UnitType type){
	for (OrderType qType : getInstance()->queue){
		if (qType.getUnitType() == type)
			return true;
	}
	return false;
}

bool OrderQueue::checkExist(BWAPI::UnitType unitType){
	UnitType type;
	for (BWAPI::Unit u : BWAPI::Broodwar->self()->getUnits()){
		type = u->getType();
		if (type.isBuilding()){
			if (type == unitType){
				return true;
			}
		}
	}
	return false;
}
OrderQueue::~OrderQueue()
{
}