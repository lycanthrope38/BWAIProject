#pragma once
//class này dùng để wrap Unit và Upgrade lại đễ dễ xử lí trong queue
#include <BWAPI.h>
class OrderType
{
private:
	//true nếu là unit. false nếu là upgrade
	bool isUnitType;
	int frame;
	BWAPI::UpgradeType upgrade;
	BWAPI::Position approxPos;

public:
	BWAPI::UnitType unit;
	int supplyRequire;
	int  volume/*number of unit to order*/, failed/*số lần execute thất bại*/;
	BWAPI::UnitType parent;
	//order nhà
	OrderType(BWAPI::UnitType unitType);
	//order nhà kèm vị trí xấp xỉ
	OrderType(BWAPI::UnitType unitType, BWAPI::Position approxPosition);
	//order nhà kèm ràng buộc số dân
	OrderType(BWAPI::UnitType unitType, int supplyRequired);
	//order lính
	OrderType(BWAPI::UnitType unitType, BWAPI::UnitType parentUnit, int vol);
	//order upgrades
	OrderType(BWAPI::UpgradeType upgradeType);
	BWAPI::UnitType getUnitType(){
		return unit;
	};
	BWAPI::UpgradeType getUpgrade(){ return upgrade; };
	bool isUnit(){ return isUnitType; }
	bool isBuilding(){
		if (!isUnitType)
			return false;
		if (unit.isBuilding())
			return true;
		return false;
	}
	
	bool isWithPosition(){
		if (approxPos == BWAPI::Positions::None){
			//BWAPI::Broodwar->sendText("Do not with position!!!!!");
			return false;
		}
		else
			return true;
	}

	BWAPI::Position getApproxPos(){
		return approxPos;
	}

	OrderType* getInstance(){
		return this;
	}

	~OrderType();
};