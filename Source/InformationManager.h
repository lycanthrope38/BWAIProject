#pragma once
#include <BWAPI.h>

class InformationManager
{
	/*private:
	void analyzeResources(BWAPI::Unit u);
	void analyzeTroop(BWAPI::Unit u);
	void analyzeConstruct(BWAPI::Unit u);*/
protected:
	char** selfMapInfor;
	char** enemyMapInfor;
	int mapWidth, mapHeight;
	int enemyTroopCounter;
	BWAPI::Position basePosition;
	std::vector<BWAPI::Unit> enemyTroops;
	std::vector<BWAPI::Unit> enemyConstructs;
	std::vector<BWAPI::Unit> minerals;
	std::vector<BWAPI::Unit> geysers;
public:
	static const char FLAG_BLANK_SPACE_TAYLOR_SWIFT = 0;
	static const char FLAG_MY_CONSTRUCTS = 1;
	static const char FLAG_ENEMY_CONSTRUCTS = 2;
	static const char FLAG_RESOURCES = 3;
	static const char FLAG_MY_TROOP = 4;
	static const char FLAG_ENEMY_TROOP = 5;
	struct  Unit_N_Volume
	{
		BWAPI::Unit unit;
		int volume;
	};
	//Truyền một unit ở ban đầu game để khởi tạo
	InformationManager(BWAPI::Unit u);

	// Cập nhật lại toàn bộ thông tin trên bản đồ cả bên mình lẫn địch. 
	// Nên hạn chế dùng thường xuyên vì số lượng unit lớn có thể gây lag
	void updateMapAll();

	// Cập nhật lại bản đồ bên mình
	// Nên hạn chế dùng thường xuyên vì số lượng unit lớn có thể gây lag
	void updateMapSelf();

	// Cập nhật lại bản đồ bên địch
	// Nên hạn chế dùng thường xuyên vì số lượng unit lớn có thể gây lag
	void updateMapEnemy();

	//đánh dấu tạm thời vị trí địch trên bản đồ. Dùng các const FLAG_* để đánh dấu
	void flagEnemy(int flag, BWAPI::Position position);

	//lấy tọa độ base - căn cứ lớn nhất đầu game
	BWAPI::Position getBasePosition();

	//lấy tọa độ các thuộc địa - các địa điểm sau khi mở rộng gần đó
	std::vector<BWAPI::Position> getColonyPositions();

	//lấy tất cả các tọa độ có hiện diện nhà của ta
	std::vector<BWAPI::Position> getSelfConstructsPositions();

	//lấy tất cả các tọa độ có sự hiện diện của lính hoặc nhà mình
	std::vector<BWAPI::Position> getOursPositions();

	//lấy tất cả tọa độ có hiện diện tài nguyên mineral
	std::vector<BWAPI::Position> getMineralsPositions();

	//lấy tất cả các tọa độ có hiện diện tài nguyên gas
	std::vector<BWAPI::Position> getGasPositions();

	//lấy tất cả các tọa độ mà gần đó từng xuất hiện lính địch
	std::vector<BWAPI::Position> getEnemyTroopPositions();

	//lấy danh sách các tọa độ địch trong bán kính 1 màn hình game
	std::vector<BWAPI::Position> getNearestEnemyPosition(BWAPI::TilePosition inputPosition);

	//lấy số lượng lính địch đã phát hiện
	int getEnemyTroopVolume();

	//lấy danh sách quân lính/worker của địch đã được phát hiện
	std::vector<BWAPI::Unit> getEnemyTroopList();

	//kiểm tra xem xung quanh có mối đe dọa hay không
	bool isThreatsNear(BWAPI::Position pos);

	//kiểm tra xem xung quanh có tài nguyên hay không
	bool isResourcesNear(BWAPI::Position pos);

	//lấy vị trí địch gần nhất
	std::vector<BWAPI::Position> InformationManager::getNearestEnemyPositions(BWAPI::Position inputPosition);

	//lấy bản đồ địch
	char** getEnemyMap(){
		return enemyMapInfor;
	}

	~InformationManager();
};

