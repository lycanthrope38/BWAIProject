#pragma once
#include <BWAPI.h>

using namespace std;
using namespace BWAPI;

class TargetManager
{
private:
	Unit rootUnit;
	Unitset enemyList;
	int lastGetEnemyFrame;
public:
	TargetManager();
	//lấy mục tiêu tốt nhất mà một unit nên tấn công. selfScore = selfSize * selfDestroyScore
	Unit getTarget(Unit);
	~TargetManager();
};

