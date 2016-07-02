#pragma once
#include <BWAPI.h>
#include "OrderType.h"
#include "Collections.h"

using namespace std;
using namespace BWAPI;
class ArmyOrder
{

private:
	BWAPI::Player self;
	set<pair<UnitType, int>> troopRequired;
public:

	//by anh
	bool trainZealot();
	std::vector<BWAPI::Unit> getGatewayList();
	//train unit
	bool train(OrderType& orderType);
	//train unit
	bool train(OrderType* orderType);
	void checkAndTrain();
	void initZerg();
	void initProtoss();
	void initTerran();

	//train unit by a parent unit
	//bool train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit);
	////train units by a single parent unit
	//bool train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit,int unitVolume);
	////train unit by list of parents unit
	//bool train(std::vector<BWAPI::Unit> parentUnit, BWAPI::UnitType typeOfUnit);
	ArmyOrder(BWAPI::Player self);
	~ArmyOrder();
};