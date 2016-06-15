#include "StaticOrder.h"
#include "OrderQueue.h"

bool StaticOrder::isInited = false;
StaticOrder* StaticOrder::instance = nullptr;
deque<OrderType*>  StaticOrder::orderQueue = deque<OrderType*>();
bool StaticOrder::isBuildingBefore = false;
bool StaticOrder::isBuildedGas = false;
const int StaticOrder::INFINITY_LIFE_TIME = 999999999;
int StaticOrder::skipBefore = 0;
int StaticOrder::buildedGasFrame = -999999;
OrderType* StaticOrder::lastOrder = nullptr;

StaticOrder::StaticOrder()
{
	Race r = Broodwar->self()->getRace();

	if (r == Races::Protoss){
		isInited = true;
		initProtoss();
	}
	else if (r == Races::Zerg){
		isInited = true;
		initZerg();
	}
	else if (r == Races::Terran){
		isInited = true;
		initTerran();
	}
	else{
		return;
	}
}

bool StaticOrder::execute(){
	StaticOrder* ins = getInstance();

	//error handler
	/*Error isResourceError = Broodwar->getLastError();
	if (isResourceError == Errors::Insufficient_Gas || isResourceError == Errors::Insufficient_Minerals)
	{
	Broodwar->sendText("Pushed last issued order");
	Broodwar->printf("Pushed last issued order");
	ins->orderQueue.push_front(ins->lastOrder);
	}*/

	if (skipBefore > 0)
	{
		Broodwar->sendText("Building before! Skip this execute");
		skipBefore--;
		return false;
	}

	if (ins->orderQueue.size() != 0)
		if (OrderQueue::getInstance()->execute(ins->orderQueue.front())){
			/*isBuildingBefore = true;*/



			if (ins->orderQueue.front()->isBuilding()){
				skipBefore = 4;
				if (ins->orderQueue.front()->getUnitType() == Broodwar->self()->getRace().getRefinery()){
					isBuildedGas = true;
					buildedGasFrame = Broodwar->getFrameCount();
				}

			}
			//lastOrder = ins->orderQueue.front();
			ins->orderQueue.pop_front();
			return true;
		}
	return false;
}

void StaticOrder::initProtoss(){
	protoss_twoGateways();
	//protoss_aftertwoGateways();
}

void StaticOrder::protoss_twoGateways(){
	/*www.youtube.com/watch?v=w_SKXc22Pmg*/
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Assimilator));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 4));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 5));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 5));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Forge));	
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 4));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	////orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	////orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 2));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 4));
	///*orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	//orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));*/


	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Assimilator));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Forge));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UpgradeTypes::Protoss_Ground_Weapons));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	orderQueue.push_back(new OrderType(UpgradeTypes::Protoss_Ground_Armor));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));


	//Zealot rush

	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Photon_Cannon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 1));



	/*orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
*/
}

void StaticOrder::protoss_aftertwoGateways(){


	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Cybernetics_Core));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UpgradeTypes::Protoss_Ground_Armor));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UpgradeTypes::Singularity_Charge));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UpgradeTypes::Protoss_Air_Weapons));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	
	//air

	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Stargate));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Fleet_Beacon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Stargate));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Stargate));
	orderQueue.push_back(new OrderType(UpgradeTypes::Carrier_Capacity));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UpgradeTypes::Protoss_Air_Weapons));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UpgradeTypes::Protoss_Air_Weapons));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Carrier, UnitTypes::Protoss_Carrier.whatBuilds().first, 1));


}

void StaticOrder::initTerran(){
}

void StaticOrder::initZerg(){
}

StaticOrder::~StaticOrder()
{
}
