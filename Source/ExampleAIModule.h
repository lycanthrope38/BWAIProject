#pragma once
#include <BWAPI.h>
#include <map>

#include "ArmyOrder.h"

// Remember not to use "Broodwar" in any global class constructor!

class ExampleAIModule : public BWAPI::AIModule
{
	bool pool;

	int supplyCounter, supplyTotalCounter, counterGateway, supplyAvailable;
	BWAPI::Unit supplyBuilderTemp;
	ArmyOrder* armyOrder;

public:
	// Virtual functions for callbacks, leave these as they are.
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit unit);
	virtual void onUnitEvade(BWAPI::Unit unit);
	virtual void onUnitShow(BWAPI::Unit unit);
	virtual void onUnitHide(BWAPI::Unit unit);
	virtual void onUnitCreate(BWAPI::Unit unit);
	virtual void onUnitDestroy(BWAPI::Unit unit);
	virtual void onUnitMorph(BWAPI::Unit unit);
	virtual void onUnitRenegade(BWAPI::Unit unit);
	virtual void onSaveGame(std::string gameName);
	virtual void onUnitComplete(BWAPI::Unit unit);
	// Everything below this line is safe to modify.

	void createGateWay(BWAPI::Unit u);
	void createPylon(BWAPI::Unit u);
	void createAssimulator(BWAPI::Unit u);
	void createCubemetics(BWAPI::Unit u);
	void createAssimilator(BWAPI::Unit u);
	void createCybernetics(BWAPI::Unit u);
	void built(BWAPI::Unit u, BWAPI::UnitType unitType);

};

