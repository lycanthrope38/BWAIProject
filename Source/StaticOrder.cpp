#include "StaticOrder.h"
#include "OrderQueue.h"

bool StaticOrder::isInited = false;
StaticOrder* StaticOrder::instance = nullptr;
deque<OrderType*>  StaticOrder::orderQueue = deque<OrderType*>();

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
	if (OrderQueue::getInstance()->execute(getInstance()->orderQueue.front())){
		getInstance()->orderQueue.pop_front();
		return true;
	}
	return false;
}

void StaticOrder::initProtoss(){
	protoss_twoGateways();
	protoss_aftertwoGateways();
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

}

void StaticOrder::protoss_aftertwoGateways(){

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
