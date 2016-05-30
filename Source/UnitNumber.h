#pragma once
#include <BWAPI.h>

class UnitNumber
{
public:
	BWAPI::UnitType unitType;
	int number;
	UnitNumber(BWAPI::UnitType, int);

	friend bool operator < (const UnitNumber &left, const UnitNumber &right)
	{
		return (left.unitType != right.unitType && left.number<right.number);
	}
	~UnitNumber();
};

