#pragma once
//class này dùng để wrap Unit và Upgrade lại đễ dễ xử lí trong queue
#include <BWAPI.h>
class OrderType
{
private:
	//true nếu là unit. false nếu là upgrade
	bool isUnitType;
<<<<<<< HEAD
	int frame;
=======
	int countWorker;
>>>>>>> 9009a9f1c437d822bdc485c5b05a3c97727a9101
	BWAPI::UnitType unit;
	BWAPI::UpgradeType upgrade;
public:
	int supplyRequire;
	int  volume/*number of unit to order*/, failed/*số lần execute thất bại*/;
	BWAPI::UnitType parent;
	//order nhà
	OrderType(BWAPI::UnitType unitType);
<<<<<<< HEAD
	//order nhà kèm ràng buộc số dân
	OrderType(BWAPI::UnitType unitType, int supplyRequired);
	//order lính
=======
<<<<<<< HEAD
	OrderType(BWAPI::UnitType unitType,int worker);
=======
	//order nhà kèm ràng buộc số dân
	OrderType(BWAPI::UnitType unitType, int supplyRequired);
	//order lính
>>>>>>> master
>>>>>>> 9009a9f1c437d822bdc485c5b05a3c97727a9101
	OrderType(BWAPI::UnitType unitType, BWAPI::UnitType parentUnit, int vol);
	//order upgrades
	OrderType(BWAPI::UpgradeType upgradeType);
	BWAPI::UnitType getUnit(){
		return unit;
	};
<<<<<<< HEAD
=======
	int getCountWorker()
	{
		return countWorker;
	}
>>>>>>> 9009a9f1c437d822bdc485c5b05a3c97727a9101
	BWAPI::UpgradeType getUpgrade(){return upgrade;};
	bool isUnit(){return isUnitType;}
	bool isBuilding(){
		if (!isUnitType)
			return false;
		if (unit.isBuilding())
			return true;
		return false;
	}
	~OrderType();
};

