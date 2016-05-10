#include"BWAPI.h"
using namespace BWAPI;

class scoutManager{
	Unit _scout;
	std::string _scoutStatus;
	bool _scoutUnderAttack;
	//std::vector<Position> _enemyRegionVertices;
	int _numOfScouts;



public:
	
	scoutManager();
	bool isScout(Unit unit); // sai o day
	void setScout(Unit unit);
	void moveScout();
	Unit returnScout();
	void onUnitCreate(); // chon scout tu dong worker
	void onSendText(std::string text);

};