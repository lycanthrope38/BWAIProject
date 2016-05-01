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
	ArmyOrder(BWAPI::Player self);
	~ArmyOrder();
};

