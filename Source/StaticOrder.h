#pragma once
#include "deque"
#include "OrderType.h"
#include <BWAPI.h>

using namespace std;
using namespace BWAPI;

class StaticOrder
{

private:
	static StaticOrder* instance;
	static deque<OrderType*>  orderQueue;
	StaticOrder();
	static bool isInited;
	//dùng để tránh việc xây nhà nhưng bị lính chen ngang làm hỏng
	static bool isBuildingBefore;
	void initProtoss();
	void initZerg();
	void initTerran();
	void protoss_twoGateways();
	void protoss_aftertwoGateways();

public:
	static StaticOrder* getInstance(){
		if (isInited)
			return instance;
		else{
			isInited = true;
			StaticOrder();
		}
		return instance;
	}

	bool execute();

	bool isEmpty(){
		if (orderQueue.size()==0)
			return true;
	
		return false;
	}

	~StaticOrder();
};

