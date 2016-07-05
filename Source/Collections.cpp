#include "Collections.h"

using namespace BWAPI;
int Collections::limitTroopFor700Frame = 1;
int Collections::currentTroopCycle = 0;
int Collections::lastExpansion = 0;
int Collections::buildInRow = 0;
int Collections::trainInRow = 0;
Position Collections::basePosition = Positions::None;
Position Collections::rootBuildPosition = Positions::None;
Position Collections::defensePosition = Positions::None;
bool Collections::isBuildingOther = false;

int Collections::lastBuildCall = 0;
int Collections::lastBuildSuccess = 0;
Collections::Collections()
{
}

UnitType Collections::getStaticDefenseStructure(Race race){
	if (race == Races::Protoss)
		return UnitTypes::Protoss_Photon_Cannon;
}

bool Collections::shouldTrainNow(){
	if (Collections::lastBuildSuccess >= Collections::lastBuildCall && Broodwar->getFrameCount() > Collections::lastBuildSuccess + 5)
		return true;
}

bool Collections::shouldBuildNow(){
	if (Collections::lastBuildSuccess >= Collections::lastBuildCall && Broodwar->getFrameCount() > Collections::lastBuildSuccess + 5)
		return false;
}

Collections::~Collections()
{
}
