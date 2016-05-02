#pragma once
//class này dùng để wrap Unit và Upgrade lại đễ dễ xử lí trong queue
#include <BWAPI.h>
class OrderType
{
private:
	//true nếu là unit. false nếu là upgrade
	bool isUnitType;
	//cai bien frame nay se la frame de thuc thi order. 
	int frame;
	BWAPI::UnitType unit;
	BWAPI::UpgradeType upgrade;
public:
	OrderType(BWAPI::UnitType unitType);
	OrderType(BWAPI::UnitType unitType, int frame);
	OrderType(BWAPI::UpgradeType upgradeType);
	BWAPI::UnitType getUnit(){
		return unit;
	};
	BWAPI::UpgradeType getUpgrade(){
		return upgrade;
	};
	bool isUnit(){
		return isUnitType;
	}
	bool isBuilding(){
		if (!isUnitType)
			return false;
		if (unit.isBuilding())
			return true;
		return false;
	}
	~OrderType();
};

