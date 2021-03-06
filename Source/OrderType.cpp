﻿//class này dùng để wrap Unit và Upgrade lại đễ dễ xử lí trong queue
#include "OrderType.h"
#include "BWAPI.h"

using namespace BWAPI;

//dành cho việc order nhà
OrderType::OrderType(BWAPI::UnitType unitType){
	this->unit = unitType;
	this->isUnitType = true;
	this->failed = 0;
	this->supplyRequire = -1;
	this->approxPos = Positions::None;
};//dành cho việc order nhà
OrderType::OrderType(BWAPI::UnitType unitType, Position approxPosition){
	this->unit = unitType;
	this->isUnitType = true;
	this->failed = 0;
	this->supplyRequire = -1;
	this->approxPos = approxPosition;
};
//dành cho việc order nhà kèm ràng buộc supply
OrderType::OrderType(BWAPI::UnitType unitType, int supplyRequired){
	this->unit = unitType;
	this->isUnitType = true;
	this->failed = 0;
	this->supplyRequire = supplyRequired;
	this->approxPos = Positions::None;
};
//dành cho việc order lính
OrderType::OrderType(BWAPI::UnitType unitType, BWAPI::UnitType parentUnit, int vol){
	this->unit = unitType;
	this->isUnitType = true;
	this->failed = 0;
	this->volume = vol;
	this->supplyRequire = -1;
	this->parent = parentUnit;
	this->approxPos = Positions::None;
};
//dành cho việc order upgrade
OrderType::OrderType(BWAPI::UpgradeType upgradeType){
	this->upgrade = upgradeType;
	this->isUnitType = false;
	this->supplyRequire = -1;
	this->failed = 0;
	this->approxPos = Positions::None;
};


OrderType::~OrderType()
{
}