#pragma once
#include <BWAPI.h>
using namespace std;
class ArmyOrder
{

private: 
	//current game AIModule
	BWAPI::AIModule* root;
	//current GameWrapper::Broodwar
	BWAPI::GameWrapper* Broodwar;
	BWAPI::Player self;
public:

	//by anh
	void trainZealot();
	std::vector<BWAPI::Unit> getGatewayList();
	ArmyOrder(BWAPI::Player self);
	~ArmyOrder();
};

