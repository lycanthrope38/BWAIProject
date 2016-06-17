#pragma once
#include <map>
#include <BWAPI.h>

using namespace std;
using namespace BWAPI;

class UnitTimeManager
{
	static map<UnitType, int> endFrame;
	static bool isInited;
	UnitTimeManager();
public:
	static int getEndFrame(UnitType);
	~UnitTimeManager();
};

