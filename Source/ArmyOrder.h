#pragma once
#include <BWAPI.h>
#include "OrderType.h"
#include "Collections.h"

using namespace std;
class ArmyOrder
{

private:
	BWAPI::Player self;
public:

	//by anh
	bool trainZealot();
	std::vector<BWAPI::Unit> getGatewayList();
	//train unit
	bool train(OrderType& orderType);
	//train unit by a parent unit
	//bool train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit);
	////train units by a single parent unit
	//bool train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit,int unitVolume);
	////train unit by list of parents unit
	//bool train(std::vector<BWAPI::Unit> parentUnit, BWAPI::UnitType typeOfUnit);
	ArmyOrder(BWAPI::Player self);
	~ArmyOrder();
};