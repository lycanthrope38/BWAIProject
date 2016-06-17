#include "UnitNumber.h"


UnitNumber::UnitNumber(BWAPI::UnitType type, int numberOfUnit)
{
	this->unitType = type;
	this->number = numberOfUnit;
}

UnitNumber::~UnitNumber()
{
}
