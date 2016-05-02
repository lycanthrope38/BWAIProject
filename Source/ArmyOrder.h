#pragma once
#include <BWAPI.h>
using namespace std;
class ArmyOrder
{

private: 
	BWAPI::Player self;
public:

	//by anh
	bool trainZealot();
	std::vector<BWAPI::Unit> getGatewayList();
	//train unit by a parent unit
	bool train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit);
	//train units by a single parent unit
	bool train(BWAPI::Unit parentUnit, BWAPI::UnitType typeOfUnit,int unitVolume);
	//train units by list of parent units
	bool train(std::vector<BWAPI::Unit> parentUnit, BWAPI::UnitType typeOfUnit, int unitVolume);
	ArmyOrder(BWAPI::Player self);
	~ArmyOrder();
};

