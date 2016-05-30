#include "LordCommander.h"

using namespace BWAPI;

bool LordCommander::initedInstance = false;
LordCommander* LordCommander::instance = nullptr;
Unitset LordCommander::freeUnits = Unitset();
std::map<BWAPI::Unit, bool> LordCommander::isUsedUnit;


LordCommander::LordCommander()
{
	onFrameCounter = 0;
	initMainBaseDefense();
}

void LordCommander::onFrame(){

	LordCommander* ins = getInstance();

	onDefend();
	onAttack();	
	instance->onFrameCounter++;
	if (ins->onFrameCounter == 100){
		ins->addFreeUnit();
		ins->onFrameCounter = 0;
	}
}

void LordCommander::onDefend(){

	LordCommander* ins = getInstance();
	for (BattleField defender : ins->defenders)
		defender.onDefend();



	//test code
	/*
		if (!isInited){
		isInited = true;
		Position basePosition;
		for (Unit u : Broodwar->self()->getUnits())
		if (u->getType().isBuilding())
		basePosition = u->getPosition();
		testDefender = BattleField(freeUnits, basePosition);
		}
		else{
		if (freeUnits.size() != 0){
		Broodwar->printf("added %d units", freeUnits.size());
		testDefender.addUnits(freeUnits);
		freeUnits.clear();
		}
		testDefender.onDefend();
		}*/
}

void LordCommander::onAttack(){
	LordCommander* ins = getInstance();
	for (BattleField defender : ins->defenders)
		defender.onAttack();
}

void LordCommander::addFreeUnit(){
	LordCommander* ins = getInstance();
	Broodwar->printf("addFreeUnit called!");
	Unitset allSelfAttackableUnit = Unitset();
	UnitType tmpType;


	for (Unit u : Broodwar->self()->getUnits()){
		tmpType = u->getType();
		//Broodwar->printf("%s", tmpType.getName());
		//test
		/*if ((u->getType().isBuilding()))
			Broodwar->printf("Building");
			if ((u->getType().isWorker()))
			Broodwar->printf("isWorker");
			if ((u->getType().isNeutral()))
			Broodwar->printf("isNeutral");*/

		//test
		if (!(u->getType().isBuilding()) && !(u->getType().isWorker()) && !(u->getType().isNeutral())){
			//Broodwar->printf("Chuan bi them 1 unit");
			if (isUsedUnit.find(u) == isUsedUnit.end()){
				ins->isUsedUnit.insert(std::make_pair(u, false));
				ins->freeUnits.insert(u);
				Broodwar->printf("Inserted a unit to freeUnits");
			}
			/*else
				Broodwar->printf("Ko thoa man dieu kien");*/
		}
	}
	Broodwar->printf("Unit size %d ", Broodwar->self()->getUnits().size());
}

BattleField LordCommander::initMainBaseDefense(){
	BattleField result;
}

LordCommander::~LordCommander()
{
}
