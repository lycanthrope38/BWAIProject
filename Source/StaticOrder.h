﻿#pragma once
#include "deque"
#include "OrderType.h"
#include <BWAPI.h>
#include "BuidingManager.h"

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
	static int skipBefore;
	static OrderType* lastOrder;
	void initProtoss();
	void initZerg();
	void initTerran();
	void protoss_twoGateways();
	void protoss_aftertwoGateways();

	BuidingManager* buildingManager;

public:
	static bool isBuildedGas;
	static int buildedGasFrame;
	static const int INFINITY_LIFE_TIME;
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

