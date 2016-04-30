#include "ArmyOrder.h"
#include <iostream>

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
	for (Unit u : gatewayList){
		if (self->minerals() >= 100)
			u->train(UnitTypes::Protoss_Zealot);
	}
}

ArmyOrder::~ArmyOrder()
{

}
