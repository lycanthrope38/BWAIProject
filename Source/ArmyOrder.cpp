#include "ArmyOrder.h"
#include <iostream>
#include <string> 

using namespace BWAPI;
using namespace Filter;

ArmyOrder::ArmyOrder(BWAPI::Player main_self)
{
	self = main_self;
}

std::vector<Unit> ArmyOrder::getGatewayList(){
	std::vector<Unit> result;
	for (Unit u : self->getUnits()){
		if (u->getType() == UnitTypes::Protoss_Gateway)
			result.push_back(u);
	}
	return result;
}

void ArmyOrder::trainZealot(){
	std::vector<Unit> gatewayList = getGatewayList();
	if (gatewayList.size() == 0)
		return;
	Broodwar->sendText(std::to_string(gatewayList.size()).c_str());
	//trường hợp lúc đầu ít tiền thì chọn cái nào đang rảnh rỗi để train lính
	for (Unit u:gatewayList){
		if (!(u->isTraining()))
			if (self->minerals() >= 100){
				u->train(UnitTypes::Protoss_Zealot);
				return;
			}
	}
	for (Unit u : gatewayList)
		if (self->minerals() >= 100){
			u->train(UnitTypes::Protoss_Zealot);
		}
		
}

ArmyOrder::~ArmyOrder()
{

}
