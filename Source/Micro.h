#include <BWAPI.h>

namespace Micro
{
	void attackUnit(BWAPI::Unit attacker, BWAPI::Unit target);
	void attackMove(BWAPI::Unit attacker, const BWAPI::Position& targetPosition);
	void move(BWAPI::Unit unit, const BWAPI::Position& targetPosition);
}