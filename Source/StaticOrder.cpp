#include "StaticOrder.h"
#include "OrderQueue.h"

bool StaticOrder::isInited = false;
StaticOrder* StaticOrder::instance = nullptr;
deque<OrderType*>  StaticOrder::orderQueue = deque<OrderType*>();
bool StaticOrder::isBuildingBefore = false;
const int StaticOrder::INFINITY_LIFE_TIME = 999999999;

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
	if (isBuildingBefore){
		Broodwar->sendText("Building before! Skip this execute");
		isBuildingBefore = false;
		return false;
	}

	if (OrderQueue::getInstance()->execute(ins->orderQueue.front())){
		if (ins->orderQueue.front()->isBuilding())
			isBuildingBefore = true;
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
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 5));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 2));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 2));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 4));

	/*origin
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Gateway));
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 5));
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	 orderQueue.push_back(new OrderType(UnitTypes::Protoss_Zealot, UnitTypes::Protoss_Zealot.whatBuilds().first, 4));
	 
	 **/

}

void StaticOrder::protoss_aftertwoGateways(){

	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 2));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Assimilator));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Probe, UnitTypes::Protoss_Probe.whatBuilds().first, 3));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Cybernetics_Core));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 4));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 3));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Pylon));
	orderQueue.push_back(new OrderType(UnitTypes::Protoss_Dragoon, UnitTypes::Protoss_Dragoon.whatBuilds().first, 3));

}

void StaticOrder::initTerran(){
}

void StaticOrder::initZerg(){
}

StaticOrder::~StaticOrder()
{
}
