//class này dùng để wrap Unit và Upgrade lại đễ dễ xử lí trong queue
#include "OrderType.h"
#include "BWAPI.h"

//dành cho việc order nhà
OrderType::OrderType(BWAPI::UnitType unitType){
	this->unit = unitType;
	this->isUnitType = true;
	this->failed = 0;
};
//dành cho việc order lính
OrderType::OrderType(BWAPI::UnitType unitType, BWAPI::UnitType parentUnit, int vol){
	this->unit = unitType;
	this->isUnitType = true;
	this->failed = 0;
	this->volume = vol;
	this->parent = parentUnit;
};
//dành cho việc order upgrade
OrderType::OrderType(BWAPI::UpgradeType upgradeType){
	this->upgrade = upgradeType;
	this->isUnitType = false;
	this->failed = 0;
};

OrderType::~OrderType()
{
}