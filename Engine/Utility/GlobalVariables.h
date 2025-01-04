#pragma once
/*データの外部出力(json形式)を行うクラス*/
#include "Math_Structs.h"
#include "Transform.h"
#include "ImGuiManager.h"
#include "externals/nlohmann/json.hpp"
#include <variant>
#include <map>
#include <string>
#include <fstream>
using namespace Math;
struct Item {
	//項目の値
	std::variant<int32_t, float, Vector3,Matrix4x4, TransformQua> value;
};
struct Group {
	std::map<std::string, Item> items;
};
using json = nlohmann::json;
class GlobalVariables
{
public:
	static GlobalVariables* GetInstance();
	
	
	/// <summary>
	/// グループ名からグループを作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);



	//毎フレーム処理
	void Update();
	//値がなければ、SetValueを呼ぶ関数たち(すでに値があるときは呼ばない)
	#pragma region AddItem
	//

	/// <summary>
	///	アイテムの追加(int)(アイテムが追加済みなら何もしない)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">ImGui表示名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	/// <summary>
	///	アイテムの追加(float)(アイテムが追加済みなら何もしない)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">ImGui表示名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, float value);
	/// <summary>
	///	アイテムの追加(Vector3)(アイテムが追加済みなら何もしない)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">ImGui表示名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);
	/// <summary>
	///	アイテムの追加(Matrix4x4)(アイテムが追加済みなら何もしない)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">ImGui表示名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const Matrix4x4& value);
	/// <summary>
	///	アイテムの追加(TransformQua)(アイテムが追加済みなら何もしない)
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">ImGui表示名</param>
	/// <param name="value">値</param>
	void AddItem(const std::string& groupName, const std::string& key, const TransformQua& value);
	//アイテムの追加(bool)(アイテムが追加済みなら何もしない)
	//void AddItem(const std::string& groupName, const std::string& key, const bool& value);
#pragma endregion AddItem
	//値を取得する関数たち(Getter)
	#pragma region Getter
	int32_t GetIntValue(const std::string& groupName, const std::string& key)const;
	float GetfloatValue(const std::string& groupName, const std::string& key)const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key)const;
	TransformQua GetTransformQuaValue(const std::string& groupName, const std::string& key)const;
	//bool GetboolValue(const std::string& groupName, const std::string& key)const;
#pragma endregion Getter
	void Updateint32_tItem(const std::string& groupName, const std::string& key, const int32_t& value);
	void UpdateTransformQuaItem(const std::string& groupName, const std::string& key, const TransformQua& value);
	/// <summary>
	/// ファイルにデータを保存
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void SaveFile(const std::string& groupName);
	//ディレクトリの全ファイルを読み込む(LoadFileを呼び出す)
	void LoadFiles();
private:
	GlobalVariables()=default;
	~GlobalVariables()=default;
	GlobalVariables(const GlobalVariables& ch) = delete;
	GlobalVariables& operator=(const GlobalVariables& ch) = delete;
	//実際に値を入れる関数たち
	#pragma region SetValue
	//値のセット(int)
	void SetValue(const std::string& groupName, const std:: string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std:: string& key, float value);
	//値のセット(Vector3)
	void SetValue(const std::string& groupName, const std:: string& key, const Vector3& value);
	//値のセット(Vector3)
	void SetValue(const std::string& groupName, const std:: string& key, const Matrix4x4& value);
	//値のセット(TransformQua)
	void SetValue(const std::string& groupName, const std:: string& key, const TransformQua& value);
	//値のセット(bool)
	//void SetValue(const std::string& groupName, const std:: string& key, const bool& value);
#pragma endregion SetValue

	void LoadFile(const std::string& groupName);
	
	//全データ
	std::map<std::string, Group> datas_;
	//jsonファイルの保存先のパス
	const std::string kDirectoryPath = "resources/GlobalVariables/";

};

