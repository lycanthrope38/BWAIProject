#include "InformationManager.h"


InformationManager::InformationManager(BWAPI::Unit unit)
{
	selfMapInfor = new char*[BWAPI::Broodwar->mapHeight()];
	enemyMapInfor = new char*[BWAPI::Broodwar->mapHeight()];
	for (int i = 0; i < BWAPI::Broodwar->mapHeight(); i++){
		selfMapInfor[i] = new char[BWAPI::Broodwar->mapWidth()];
		enemyMapInfor[i] = new char[BWAPI::Broodwar->mapWidth()];
	}
	for (int i = 0; i < BWAPI::Broodwar->mapHeight(); i++)
		for (int j = 0; j < BWAPI::Broodwar->mapWidth(); j++){
			selfMapInfor[i][j] = 0;
			enemyMapInfor[i][j] = 0;
		}
	BWAPI::Position tmpPos;
	//set base position
	basePosition = unit->getPosition();
	//put my Unit to mapInfor
	
	
}

// Phân tích và lưu thông tin tình hình xung quanh một Unit
// Nên hạn chế dùng thường xuyên vì số lượng unit lớn có thể gây lag
void InformationManager::updateMapAll(){
	updateMapSelf();
	updateMapEnemy();
};


// Cập nhật lại bản đồ bên mình
// Nên hạn chế dùng thường xuyên vì số lượng unit lớn có thể gây lag
void InformationManager::updateMapSelf(){
	BWAPI::Position tmpPos;
	for (BWAPI::Unit u : BWAPI::Broodwar->self()->getUnits()){
		if (u->getType().isBuilding()){
			tmpPos = u->getPosition();
			selfMapInfor[(int)u->getPosition().y][(int)u->getPosition().x] = FLAG_MY_CONSTRUCTS;
		}
		else if (u->getType() == BWAPI::UnitTypes::Resource_Mineral_Field || u->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser){
			selfMapInfor[(int)u->getPosition().y][(int)u->getPosition().x] = FLAG_RESOURCES;
		}
		else{
			selfMapInfor[(int)u->getPosition().y][(int)u->getPosition().x] = FLAG_MY_TROOP;
		}
	}
}

// Cập nhật lại bản đồ bên địch
// Nên hạn chế dùng thường xuyên vì số lượng unit lớn có thể gây lag
void InformationManager::updateMapEnemy(){
	BWAPI::Position tmpPos;
	for (BWAPI::Unit u : BWAPI::Broodwar->enemies ().getUnits()){
		if (u->getType().isBuilding()){
			tmpPos = u->getPosition();
			selfMapInfor[(int)u->getPosition().y][(int)u->getPosition().x] = FLAG_MY_CONSTRUCTS;
		}
		else if (u->getType() == BWAPI::UnitTypes::Resource_Mineral_Field || u->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser){
			selfMapInfor[(int)u->getPosition().y][(int)u->getPosition().x] = FLAG_RESOURCES;
		}
		else{
			selfMapInfor[(int)u->getPosition().y][(int)u->getPosition().x] = FLAG_MY_TROOP;
		}
	}
}

//lấy tọa độ base - căn cứ lớn nhất đầu game
BWAPI::Position InformationManager::getBasePosition(){
	return basePosition;
};

//lấy tọa độ các thuộc địa - các địa điểm sau khi mở rộng gần đó
std::vector<BWAPI::Position> InformationManager::getColonyPositions(){
};

//lấy tất cả các tọa độ có hiện diện nhà của ta
std::vector<BWAPI::Position> InformationManager::getSelfConstructsPositions(){
	std::vector<BWAPI::Position> result;
	for (BWAPI::Unit u : BWAPI::Broodwar->self()->getUnits())
		if (u->getType().isBuilding())
			result.push_back(u->getPosition());
	return result;
};

//lấy tất cả các tọa độ có sự hiện diện của lính hoặc nhà mình
std::vector<BWAPI::Position> InformationManager::getOursPositions(){
	std::vector<BWAPI::Position> result;
	for (BWAPI::Unit u : BWAPI::Broodwar->self()->getUnits())
		result.push_back(u->getPosition());
};

//lấy tất cả tọa độ có hiện diện tài nguyên mineral
std::vector<BWAPI::Position> InformationManager::getMineralsPositions(){
	std::vector<BWAPI::Position> result;
	for (BWAPI::Unit u : BWAPI::Broodwar->getStaticMinerals())
		result.push_back(u->getPosition());
	return result;
};

//lấy tất cả các tọa độ có hiện diện tài nguyên gas
std::vector<BWAPI::Position> InformationManager::getGasPositions(){
	std::vector<BWAPI::Position> result;
	for (BWAPI::Unit u : BWAPI::Broodwar->getStaticGeysers())
		result.push_back(u->getPosition());
	return result;
};

//lấy tất cả các tọa độ mà gần đó từng xuất hiện lính địch
std::vector<BWAPI::Position> InformationManager::getEnemyTroopPositions(){
	std::vector<BWAPI::Position> result;
	for (BWAPI::Unit u : BWAPI::Broodwar->enemies().getUnits())
		if (!u->getType().isBuilding())
			result.push_back(u->getPosition());
	return result;
};

//lấy danh sách các tọa độ địch trong bán kính 1 màn hình game
std::vector<BWAPI::Position> InformationManager::getNearestEnemyPositions(BWAPI::Position inputPosition){
	
}

//lấy số lượng lính địch đã phát hiện
int InformationManager::getEnemyTroopVolume(){
	return BWAPI::Broodwar->enemies().getUnits().size();
};

//lấy danh sách quân lính/worker của địch đã được phát hiện
std::vector<BWAPI::Unit> InformationManager::getEnemyTroopList(){
	std::vector<BWAPI::Unit> result;
	for (BWAPI::Unit u : BWAPI::Broodwar->enemies().getUnits())
		if (!u->getType().isBuilding())
			result.push_back(u);
	return result;
};

//kiểm tra xem xung quanh có mối đe dọa hay không
bool InformationManager::isThreatsNear(BWAPI::Position pos){

}

//kiểm tra xem xung quanh 1 màn hình có tài nguyên hay không
bool InformationManager::isResourcesNear(BWAPI::Position pos){
	
}

void InformationManager::flagEnemy(int flag, BWAPI::Position tilePosition){
	enemyMapInfor[tilePosition.y][tilePosition.x] = flag;
}

InformationManager::~InformationManager()
{
	for (int i = 0; i < BWAPI::Broodwar->mapHeight(); i++)
		delete [] selfMapInfor[i];
	delete[] selfMapInfor;
}

