#include "UnitTimeManager.h"
#include "StaticOrder.h"

map<UnitType, int> UnitTimeManager::endFrame = map<UnitType, int>();
bool UnitTimeManager::isInited = false;

UnitTimeManager::UnitTimeManager(){
	endFrame.insert(make_pair(UnitTypes::Protoss_Zealot, 0));
}

int UnitTimeManager::getEndFrame(UnitType u){

	if (!isInited)
		UnitTimeManager();

	map<UnitType, int>::iterator it = endFrame.find(u);
	if (it == endFrame.end()){
		return StaticOrder::INFINITY_LIFE_TIME;
	}
	else
		return it->second;
}

UnitTimeManager::~UnitTimeManager()
{
}
