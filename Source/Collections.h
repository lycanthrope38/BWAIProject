#pragma once
#include "BWAPI.h"
class Collections
{
private:
	BWAPI::Player self;
public:
	static int limitTroopFor700Frame;
	static int currentTroopCycle;
	static int lastExpansion;
	static BWAPI::Position basePosition;
	static BWAPI::Position rootBuildPosition;
	static BWAPI::Position defensePosition;
	static int lastBuildCall;
	static int lastBuildSuccess;
	static bool isBuildingOther;
	static int buildInRow;
	static int trainInRow;
	Collections();
	//Return list of specific unit. Parameters is (Broodwar->self(), UnitTypes::Protoss_"unitName")
	static std::vector<BWAPI::Unit> getUnitList(BWAPI::Player self, BWAPI::UnitType unitName){
		std::vector<BWAPI::Unit> result;
		for (BWAPI::Unit u : self->getUnits()){
			if (u->getType().getName() == unitName.getName())
				result.push_back(u);
		}
		return result;
	};
	//Return number of specific unit. Parameters is (Broodwar->self(), UnitTypes::Protoss_"unitName")
	static int getUnitVolume(BWAPI::Player self, BWAPI::UnitType unitName){
		int result = 0;
		for (BWAPI::Unit u : self->getUnits()){
			if (u->getType().getName() == unitName.getName())
				result++;
		}
		return result;
	};

	static double distance(BWAPI::Position p1, BWAPI::Position p2){
		return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
	}

	static BWAPI::UnitType getStaticDefenseStructure(BWAPI::Race);

	static bool shouldTrainNow();
	static bool shouldBuildNow();

	////Check if a unittype is exist 
	//static bool isExist(BWAPI::UnitType u){
	//	for (BWAPI::UnitType type : BWAPI::Broodwar->self()->getUnits())
	//}
	~Collections();
};
//
//Protoss Ground Units
//const UnitType 	Protoss_Archon
//const UnitType 	Protoss_Dark_Archon
//const UnitType 	Protoss_Dark_Templar
//const UnitType 	Protoss_Dragoon
//const UnitType 	Protoss_High_Templar
//const UnitType 	Protoss_Probe
//const UnitType 	Protoss_Reaver
//const UnitType 	Protoss_Scarab
//const UnitType 	Protoss_Zealot
//Protoss Air Units
//const UnitType 	Protoss_Arbiter
//const UnitType 	Protoss_Carrier
//const UnitType 	Protoss_Corsair
//const UnitType 	Protoss_Interceptor
//const UnitType 	Protoss_Observer
//const UnitType 	Protoss_Scout
//const UnitType 	Protoss_Shuttle
//Protoss Heroes Units
//const UnitType 	Hero_Aldaris
//const UnitType 	Hero_Artanis
//const UnitType 	Hero_Danimoth
//const UnitType 	Hero_Dark_Templar
//const UnitType 	Hero_Fenix_Dragoon
//const UnitType 	Hero_Fenix_Zealot
//const UnitType 	Hero_Gantrithor
//const UnitType 	Hero_Mojo
//const UnitType 	Hero_Raszagal
//const UnitType 	Hero_Tassadar
//const UnitType 	Hero_Tassadar_Zeratul_Archon
//const UnitType 	Hero_Warbringer
//const UnitType 	Hero_Zeratul
//Protoss Buildings
//const UnitType 	Protoss_Arbiter_Tribunal
//const UnitType 	Protoss_Assimilator
//const UnitType 	Protoss_Citadel_of_Adun
//const UnitType 	Protoss_Cybernetics_Core
//const UnitType 	Protoss_Fleet_Beacon
//const UnitType 	Protoss_Forge
//const UnitType 	Protoss_Gateway
//const UnitType 	Protoss_Nexus
//const UnitType 	Protoss_Observatory
//const UnitType 	Protoss_Photon_Cannon
//const UnitType 	Protoss_Pylon
//const UnitType 	Protoss_Robotics_Facility
//const UnitType 	Protoss_Robotics_Support_Bay
//const UnitType 	Protoss_Shield_Battery
//const UnitType 	Protoss_Stargate
//const UnitType 	Protoss_Templar_Archives
//Protoss Special Buildings
//const UnitType 	Special_Khaydarin_Crystal_Form
//const UnitType 	Special_Protoss_Temple
//const UnitType 	Special_Stasis_Cell_Prison
//const UnitType 	Special_Warp_Gate
//const UnitType 	Special_XelNaga_Temple
// 
// 
//
//
//
//Terran Ground Units
//const UnitType 	Terran_Firebat
//const UnitType 	Terran_Ghost
//const UnitType 	Terran_Goliath
//const UnitType 	Terran_Marine
//const UnitType 	Terran_Medic
//const UnitType 	Terran_SCV
//const UnitType 	Terran_Siege_Tank_Siege_Mode
//const UnitType 	Terran_Siege_Tank_Tank_Mode
//const UnitType 	Terran_Vulture
//const UnitType 	Terran_Vulture_Spider_Mine
//Terran Air Units
//const UnitType 	Terran_Battlecruiser
//const UnitType 	Terran_Dropship
//const UnitType 	Terran_Nuclear_Missile
//const UnitType 	Terran_Science_Vessel
//const UnitType 	Terran_Valkyrie
//const UnitType 	Terran_Wraith
//Terran Heroes
//const UnitType 	Hero_Alan_Schezar
//const UnitType 	Hero_Alexei_Stukov
//const UnitType 	Hero_Arcturus_Mengsk
//const UnitType 	Hero_Edmund_Duke_Tank_Mode
//const UnitType 	Hero_Edmund_Duke_Siege_Mode
//const UnitType 	Hero_Gerard_DuGalle
//const UnitType 	Hero_Gui_Montag
//const UnitType 	Hero_Hyperion
//const UnitType 	Hero_Jim_Raynor_Marine
//const UnitType 	Hero_Jim_Raynor_Vulture
//const UnitType 	Hero_Magellan
//const UnitType 	Hero_Norad_II
//const UnitType 	Hero_Samir_Duran
//const UnitType 	Hero_Sarah_Kerrigan
//const UnitType 	Hero_Tom_Kazansky
//const UnitType 	Terran_Civilian
//Terran Buildings
//const UnitType 	Terran_Academy
//const UnitType 	Terran_Armory
//const UnitType 	Terran_Barracks
//const UnitType 	Terran_Bunker
//const UnitType 	Terran_Command_Center
//const UnitType 	Terran_Engineering_Bay
//const UnitType 	Terran_Factory
//const UnitType 	Terran_Missile_Turret
//const UnitType 	Terran_Refinery
//const UnitType 	Terran_Science_Facility
//const UnitType 	Terran_Starport
//const UnitType 	Terran_Supply_Depot
//Terran Addons
//const UnitType 	Terran_Comsat_Station
//const UnitType 	Terran_Control_Tower
//const UnitType 	Terran_Covert_Ops
//const UnitType 	Terran_Machine_Shop
//const UnitType 	Terran_Nuclear_Silo
//const UnitType 	Terran_Physics_Lab
//Terran Special Buildings
//const UnitType 	Special_Crashed_Norad_II
//const UnitType 	Special_Ion_Cannon
//const UnitType 	Special_Power_Generator
//const UnitType 	Special_Psi_Disrupter
// 
// 
//Zerg Ground Units
//const UnitType 	Zerg_Broodling
//const UnitType 	Zerg_Defiler
//const UnitType 	Zerg_Drone
//const UnitType 	Zerg_Egg
//const UnitType 	Zerg_Hydralisk
//const UnitType 	Zerg_Infested_Terran
//const UnitType 	Zerg_Larva
//const UnitType 	Zerg_Lurker
//const UnitType 	Zerg_Lurker_Egg
//const UnitType 	Zerg_Ultralisk
//const UnitType 	Zerg_Zergling
//Zerg Air Units
//const UnitType 	Zerg_Cocoon
//const UnitType 	Zerg_Devourer
//const UnitType 	Zerg_Guardian
//const UnitType 	Zerg_Mutalisk
//const UnitType 	Zerg_Overlord
//const UnitType 	Zerg_Queen
//const UnitType 	Zerg_Scourge
//Zerg Heroes
//const UnitType 	Hero_Devouring_One
//const UnitType 	Hero_Hunter_Killer
//const UnitType 	Hero_Infested_Duran
//const UnitType 	Hero_Infested_Kerrigan
//const UnitType 	Hero_Kukulza_Guardian
//const UnitType 	Hero_Kukulza_Mutalisk
//const UnitType 	Hero_Matriarch
//const UnitType 	Hero_Torrasque
//const UnitType 	Hero_Unclean_One
//const UnitType 	Hero_Yggdrasill
//Zerg Buildings
//const UnitType 	Zerg_Creep_Colony
//const UnitType 	Zerg_Defiler_Mound
//const UnitType 	Zerg_Evolution_Chamber
//const UnitType 	Zerg_Extractor
//const UnitType 	Zerg_Greater_Spire
//const UnitType 	Zerg_Hatchery
//const UnitType 	Zerg_Hive
//const UnitType 	Zerg_Hydralisk_Den
//const UnitType 	Zerg_Infested_Command_Center
//const UnitType 	Zerg_Lair
//const UnitType 	Zerg_Nydus_Canal
//const UnitType 	Zerg_Queens_Nest
//const UnitType 	Zerg_Spawning_Pool
//const UnitType 	Zerg_Spire
//const UnitType 	Zerg_Spore_Colony
//const UnitType 	Zerg_Sunken_Colony
//const UnitType 	Zerg_Ultralisk_Cavern
//Zerg Special Buildings
//const UnitType 	Special_Cerebrate
//const UnitType 	Special_Cerebrate_Daggoth
//const UnitType 	Special_Mature_Chrysalis
//const UnitType 	Special_Overmind
//const UnitType 	Special_Overmind_Cocoon
//const UnitType 	Special_Overmind_With_Shell