//class này dùng để wrap Unit và Upgrade lại đễ dễ xử lí trong queue
#include "OrderType.h"
#include "BWAPI.h"

//trả về UnitType nếu là UnitType
OrderType::OrderType(BWAPI::UnitType unitType, int frame){
	this->unit = unit;
	this->isUnitType = true;
	this->frame = frame; 
};

//trả về UnitType nếu là UnitType
OrderType::OrderType(BWAPI::UnitType unitType){
	this->unit = unit;
	this->isUnitType = true;
};

//trả về OrderType nếu là OrderType
OrderType::OrderType(BWAPI::UpgradeType upgradeType){
	this->upgrade = upgradeType;
	this->isUnitType = false;
};

OrderType::~OrderType()
{

}
