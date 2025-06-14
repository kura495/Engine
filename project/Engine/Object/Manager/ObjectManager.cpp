#include "ObjectManager.h"

ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::Initalize()
{
	globalVariables = GlobalVariables::GetInstance();

	boxModel_.push_back(Model::CreateModelFromObj("project/resources/Cube", "Cube.obj"));

	planeModel_.push_back(Model::CreateModelFromObj("project/resources/Plane", "Plane.obj"));
	
	enemyModel_.push_back(Model::CreateModelFromObj("project/resources/NormalEnemy", "NormalEnemy.obj"));
}

void ObjectManager::Update()
{
#ifdef USE_IMGUI
	ImGui::Begin("CreateObject", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Box")) {

			if (ImGui::Button("Add Box")) {
				AddBox();
				boxObjectCount++;
				globalVariables->Updateint32_tItem("Editor", "BoxCount", boxObjectCount);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Plane")) {

			if (ImGui::Button("Add Plane")) {
				AddPlane();
				PlaneObjectCount++;
				globalVariables->Updateint32_tItem("Editor", "PlaneCount", PlaneObjectCount);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
#endif	//ImGui
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}

}

void ObjectManager::Draw()
{
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		(*ObjectIt)->Draw();
	}
}

void ObjectManager::LordFile()
{
	globalVariables->CreateGroup("Editor");
#pragma region
	globalVariables->AddItem("Editor", "BoxCount", boxObjectCount);
	boxObjectCount = globalVariables->GetIntValue("Editor", "BoxCount");
	for (int32_t boxit = 0; boxit < boxObjectCount; boxit++) {
		AddBox();
	}
	globalVariables->AddItem("Editor", "PlaneCount", PlaneObjectCount);
	PlaneObjectCount = globalVariables->GetIntValue("Editor", "PlaneCount");
	for (int32_t Pleneit = 0; Pleneit < PlaneObjectCount; Pleneit++) {
		AddPlane();
	}
#pragma endregion オブジェクト生成
}

void ObjectManager::LordBlenderScene(std::string fileName)
{
	//連結してフルパスを得る
	const std::string fullpath = /*kDefaultBaseDirectory + */ fileName + kExrension;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}
	//JSON文字列を解凍
	nlohmann::json deserialized;
	//解凍
	file >> deserialized;
	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());
	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);
	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		LoadjsonObject(object);
	}
}

void ObjectManager::LoadjsonObject(nlohmann::json& object)
{
	if (object["disabled"].contains("disabled")) {
		//有効無効フラグ
		bool disabled = object["disabled"].get<bool>();
		if (disabled) {
			//配置しない(スキップする)
			return;
		}
	}

	assert(object.contains("type"));

	std::string type = object["type"].get<std::string>();

	if (type.compare("MESH") != 0) {
		return;
	}

#pragma region Transform

	//トランスフォームのパラメータ読み込み
	ObjectData ObjTransform;
	nlohmann::json& transform = object["transform"];
	//平行移動
	ObjTransform.object.transform.translate.x = static_cast<float>(transform["translation"][0]);
	ObjTransform.object.transform.translate.y = static_cast<float>(transform["translation"][2]);
	ObjTransform.object.transform.translate.z = static_cast<float>(transform["translation"][1]);
	//回転角
	ObjTransform.object.transform.quaternion.x = -(float)(transform["rotation"][0]);
	ObjTransform.object.transform.quaternion.y = -(float)(transform["rotation"][2]);
	ObjTransform.object.transform.quaternion.z = -(float)(transform["rotation"][1]);
	ObjTransform.object.transform.quaternion.w = (float)(transform["rotation"][3]);
	//スケーリング
	ObjTransform.object.transform.scale.x = static_cast<float>(transform["scaling"][0]);
	ObjTransform.object.transform.scale.y = static_cast<float>(transform["scaling"][2]);
	ObjTransform.object.transform.scale.z = static_cast<float>(transform["scaling"][1]);

#pragma endregion 

	if (object.contains("collider")) {
		nlohmann::json& Collidertransform = object["collider"];
		//中心を変える
		ObjTransform.object.colloder.center.x = (float)Collidertransform["center"][0];
		ObjTransform.object.colloder.center.y = (float)Collidertransform["center"][2];
		ObjTransform.object.colloder.center.z = (float)Collidertransform["center"][1];
		//サイズを変える
		ObjTransform.object.colloder.size.x = (float)Collidertransform["size"][0];
		ObjTransform.object.colloder.size.y = (float)Collidertransform["size"][2];
		ObjTransform.object.colloder.size.z = (float)Collidertransform["size"][1];
	}

	//オブジェクトのトランスフォームを設定
	std::string name = object["file_name"].get<std::string>();

	if (name.compare("Cube") == 0) {
		AddBox(ObjTransform);
	}
	if (name.compare("Plane") == 0) {
		AddPlane();
	}
	if (name.compare("PlayerSpawn") == 0) {
		//TODO:授業課題で制作。今後対応予定
		//PlayerSpawnDataの情報をPlayerに反映
	}

}

void ObjectManager::AddBox()
{
	BoxObject* box = new BoxObject;
	box->Init(boxModel_);

	std::string Number = std::to_string(box->GetNumber());

	std::string Name = "Box" + Number;
	globalVariables->AddItem("Editor", Name, box->GetWorld().transform);

	object_.push_back(box);
}

void ObjectManager::AddBox(ObjectData input)
{
	BoxObject* box = new BoxObject;
	box->Init(boxModel_);

	box->SetSize(input.object.colloder.size);
	box->SetOffset(input.object.colloder.center);

	object_.push_back(box);
}

void ObjectManager::AddPlane()
{
	PlaneObject* plane = new PlaneObject;
	plane->Init(planeModel_);

	std::string Number = std::to_string(plane->GetNumber());

	std::string Name = "Plane" + Number;
	globalVariables->AddItem("Editor", Name, plane->GetWorld().transform);

	object_.push_back(plane);
}
