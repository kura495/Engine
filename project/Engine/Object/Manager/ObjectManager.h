#pragma once
//IObjectを管理するクラス//
#include "Object/IObject.h"
#include "Object/Box/BoxObject.h"
#include "Object/Plane/PlaneObject.h"
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
	/// <summary>
	/// jsonファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LordFile(std::string fileName);
	/// <summary>
	/// ブレンダーで作ったシーンファイルを読み込み
	/// </summary>
	/// <param name="fileName">ファイル</param>
	void LordBlenderScene(std::string fileName);

private:
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager& obj) = delete;
	ObjectManager& operator=(const ObjectManager& obj) = delete;

	void LoadjsonObject(nlohmann::json& object);
#pragma region
	void AddBox();
	void AddBox(ObjectData input);
	void AddPlane();
	void addModel(ObjectData input, std::string path);
#pragma endregion 読み込んだjson

	std::list<IObject*> object_;
	int32_t boxObjectCount;
	int32_t PlaneObjectCount;
	GlobalVariables* globalVariables;

	std::vector<Model*> boxModel_;
	std::vector<Model*> planeModel_;
	std::vector<Model*> enemyModel_;
	std::string kExrension = ".json";
};