#include "ObjectManager.h"

ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::Initalize()
{
	globalVariables = GlobalVariables::GetInstance();

	boxModel_.push_back(Model::CreateModelFromObj("resources/Cube", "Cube.obj"));

	planeModel_.push_back(Model::CreateModelFromObj("resources/Plane", "Plane.obj"));
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
				globalVariables->Updateint32_tItem("Editer", "BoxCount", boxObjectCount);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Plane")) {

			if (ImGui::Button("Add Plane")) {
				AddPlane();
				PlaneObjectCount++;
				globalVariables->Updateint32_tItem("Editer", "PlaneCount", PlaneObjectCount);
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

void ObjectManager::LordFile(std::string fileName)
{
	globalVariables->CreateGroup("Editer");
#pragma region
	globalVariables->AddItem("Editer", "BoxCount", boxObjectCount);
	boxObjectCount = globalVariables->GetIntValue("Editer", "BoxCount");
	for (int32_t boxit = 0; boxit < boxObjectCount; boxit++) {
		AddBox();
	}
	globalVariables->AddItem("Editer", "PlaneCount", PlaneObjectCount);
	PlaneObjectCount = globalVariables->GetIntValue("Editer", "PlaneCount");
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

void ObjectManager::AddBox()
{
	BoxObject* box = new BoxObject;
	box->Initalize(boxModel_);

	std::string Number = std::to_string(box->GetNumber());

	std::string Name = "Box" + Number;
	globalVariables->AddItem("Editer", Name, box->GetWorld().transform_);

	box->SetTransform(globalVariables->GetTransformQuaValue("Editer", Name));

	object_.push_back(box);
}

void ObjectManager::AddBox(ObjectData input)
{
	BoxObject* box = new BoxObject;
	box->Initalize(boxModel_);

	box->SetTransform(input.object.transform);
	//TODO : コライダーがあるかどうか判別するようにしたい
	//if (input.object.colloder == ) {

	//}
	box->SetSize(input.object.colloder.size);
	box->SetCenter(input.object.colloder.center);

	object_.push_back(box);
}

void ObjectManager::LoadjsonObject(nlohmann::json& object)
{
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
	ObjTransform.object.transform.translate.x = (float)transform["translation"][0];
	ObjTransform.object.transform.translate.y = (float)transform["translation"][2];
	ObjTransform.object.transform.translate.z = (float)transform["translation"][1];
	//回転角
	Vector3 Euler;
	Euler.x = -(float)transform["rotation"][0];
	Euler.y = -(float)transform["rotation"][2];
	Euler.z = -(float)transform["rotation"][1];
	//TODO :　正しい姿勢にはなっていないので変更をする
	ObjTransform.object.transform.quaternion = Quaternion::EulerToQuaterion(Euler);
	//スケーリング
	ObjTransform.object.transform.scale.x = (float)transform["scaling"][0];
	ObjTransform.object.transform.scale.y = (float)transform["scaling"][2];
	ObjTransform.object.transform.scale.z = (float)transform["scaling"][1];

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
	AddBox(ObjTransform);

	// TODO : 読み込むことが出来ない
	if (object.contains("children")) {
		nlohmann::json& children = object["children"];
		LoadjsonObject(children);
	}
}

void ObjectManager::AddPlane()
{
	PlaneObject* plane = new PlaneObject;
	plane->Initalize(planeModel_);

	std::string Number = std::to_string(plane->GetNumber());

	std::string Name = "Plane" + Number;
	globalVariables->AddItem("Editer", Name, plane->GetWorld().transform_);

	plane->SetTransform(globalVariables->GetTransformQuaValue("Editer", Name));

	object_.push_back(plane);
}
