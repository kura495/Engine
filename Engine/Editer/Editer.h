#pragma once
//エディタクラス//
#include "ImGuiManager.h"
#include "ViewProj/ViewProjection.h"
#include "WorldTransform/WorldTransform.h"

#include "Common/WinApp/WinApp.h"
#include "Renderer/Renderer.h"
class IObject;
class Editer {
public:
	static Editer* GetInstance();

	void Initalize();
	void Update();
	void Draw();
	//使うかどうか
	void IsEnable(bool Flag) {
		IsEnableFlag = Flag;
	}


#pragma region Setter
	/// <summary>
	/// ビュープロジェクションをセット
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクションのポインタ</param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	/// <summary>
	/// IObjectをセット
	/// </summary>
	/// <param name="object">オブジェクトのポインタ</param>
	void SetObject(IObject* object);
#pragma endregion
private:
	Editer() = default;
	~Editer() = default;
	Editer(const Editer& obj) = delete;
	Editer& operator=(const Editer& obj) = delete;

	void GuizmoOption();
	void Manipulator();
	void Grid();

	int ObjectCount = 0;
	std::vector<IObject*> object_;
	const ViewProjection* viewProjection_ = nullptr;
	std::string textrueName;

	bool IsEnableFlag = false;
	bool IsManipulatorFlag = true;
	bool IsGridFlag = false;

	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
};