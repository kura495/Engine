#pragma once
#include "Object/IObject.h"
#include "Game/Object/Box/BoxObject.h"
#include "Game/Object/Plane/PlaneObject.h"
struct ColloderData {
	Vector3 center;/*当たり判定の中心 {0,0,0}でオブジェクトの中心*/
	Vector3 size;/*当たり判定のサイズ*/
};
struct Object {

	TransformQua transform;/*オブジェクトの位置情報*/
	ColloderData colloder;/*当たり判定のデータ*/
};
struct ObjectData {
	Object object;
	Object childlen;
};

class ObjectManager
{
public:

	static ObjectManager* GetInstance();

	void Initalize();
	void Update();
	void Draw();

	void LordFile(std::string fileName);
	void LordBlenderScene(std::string fileName);

private:
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager& obj) = delete;
	ObjectManager& operator=(const ObjectManager& obj) = delete;

	void LoadjsonObject(nlohmann::json& object);
	void AddBox();
	void AddBox(ObjectData input);
	void AddPlane();
	void AddPlane(ObjectData input);
	void AddEnemy(ObjectData input);
	void addModel(ObjectData input, std::string path);

	std::list<IObject*> object_;
	int32_t boxObjectCount;
	int32_t PlaneObjectCount;
	GlobalVariables* globalVariables;

	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> enemyModel_;
	std::string kExrension = ".json";
};