#pragma once
//インプットクラス//
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <wrl.h>
#include "Common/WinApp/WinApp.h"
#include <array>
#include <XInput.h>
#include <list>
#include "Matrix/MatrixCalc.h"
#include "Utility/GlobalTime.h"

#define VIBRATION_MIN	0
#define VIBRATION_MAX	65535

#define MOUSE_BOTTON0	0
#define MOUSE_BOTTON1	1
#define MOUSE_BOTTON2	2
#define MOUSE_BOTTON3	3
#define MOUSE_BOTTON4	4
#define MOUSE_BOTTON5	5
#define MOUSE_BOTTON6	6
#define MOUSE_BOTTON7	7
using namespace Math;
struct ListData {
	uint32_t code;
	int frame;
	ListData(int code) {
		this->code = code;
		frame = 0;
	}
};
struct VibData {
	int leftMotor;
	int rightMotor;
	float second = 0;
	float countFrame = 0;
	VibData() {};
};

struct MousePosition {
	Vector2 Pos;
	float Scroll;
};

class Input
{
public:
	static Input* GetInstance();
	void Initialize(WinApp* winApp_);
	void Update();
	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns>bool</returns>
	bool pushKey(uint8_t keyNumber);
	/// <summary>
	/// 押している間
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns>bool</returns>
	bool TriggerKey(uint8_t keyNumber);
	/// <summary>
	/// 離した瞬間
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns>bool</returns>
	bool IsTriggerKey(uint8_t keyNumber);
	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns>bool</returns>
	bool pushPad(uint32_t buttonNumber);
	/// <summary>
	/// 押している間
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>bool</returns>
	bool TriggerPad(uint32_t buttonNumber);
	/// <summary>
	/// 押している間
	/// </summary>
	/// <param name="buttonNumber">XINPUT_GAMEPAD</param>
	/// <returns>bool</returns>
	static bool IsTriggerPad(uint32_t buttonNumber);
	/// <summary>
	/// コントローラーのバイブレーション
	/// </summary>
	/// <param name="leftMotor">低周波数ランブルモータ(大きい振動)</param>
	/// <param name="rightMotor">高周波ランブルモータ(細かい振動)</param>
	/// <param name="second">振動させる秒数</param>
	static void VibrateController(int leftMotor, int rightMotor, float second = kDeltaTime);
	/// <summary>
	/// 未完成のため使用不可
	/// </summary>
	/// <param name="Mousebutton">MOUSE_BOTTON</param>
	/// <returns>bool</returns>
	bool pushMouse(uint32_t Mousebutton);

	/// <summary>
	/// ゲームパッドを取得
	/// </summary>
	bool GetJoystickState(XINPUT_STATE& joy);

	MousePosition GetMousePosition() {
		return m_Position;
	}

	/// <summary>
	/// ゲームパッドの先行入力に対応した
	/// </summary>
	/// <param name="buttonNumber">XINPUT_GAMEPAD_</param>
	/// <param name="delayTime">何フレーム猶予区間をつくるか</param>
	/// <returns>bool</returns>
	static bool GetPadPrecede(uint32_t buttonNumber,int delayTime);

	static Vector2 GetPadTrreger() { return Vector2{ (float)joyState.Gamepad.bLeftTrigger,(float)joyState.Gamepad.bRightTrigger }; };
private:
	Input() = default;
	~Input() { VibrateController(VIBRATION_MIN, VIBRATION_MIN, kDeltaTime); };
	Input(const Input& obj) = delete;
	Input& operator=(const Input& obj) = delete;
	/// <summary>
	/// マウスの位置をVector2にする
	/// </summary>
	/// <returns></returns>
	Vector2 PositionMouse();

	float ScrollMouse();
	/// <summary>
	/// ゲームパッドの情報を取得
	/// </summary>
	bool GetJoystickState();
	/// <summary>
	/// コントローラーに関する更新をまとめたもの
	/// </summary>
	void UpdateJoyState();
	/// <summary>
	/// コントローラーのボタン入力に関する更新
	/// </summary>
	void UpdateButtan();
	/// <summary>
	/// コントローラーのバイブレーションに関する更新
	/// </summary>
	void UpdateVibration();
	//ImGui
	void ImGui();
	/// <summary>
	/// パッドで押されているボタンを識別して、押されているボタン対応の文字列を返す
	/// </summary>
	/// <param name="Number">押されているボタンの番号</param>
	/// <returns></returns>
	std::string PadButtonList(uint32_t Number);

	HRESULT hr;
	Microsoft::WRL::ComPtr<IDirectInput8>directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> divKeyboard = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> divMouse = nullptr;
	//キーボード
	std::array<BYTE, 256> key;
	std::array<BYTE, 256> keyPre;
	//マウス
	DIMOUSESTATE2 mouse_;
	DIMOUSESTATE2 mousePre_;
	MousePosition m_Position = { {0.0f,0.0f},0.0f };
	//パッド
	static XINPUT_STATE joyState;
	static std::array<int, 16> joy_frame;
	static XINPUT_STATE joyStatePre;

	//先行入力
	static std::list<ListData> joy_stack;
	static std::list<ListData> joy_stacklog;
	static const int save_frame;
	static const int log_save_frame;
	//バイブレーション
	XINPUT_VIBRATION vibration_;
	static VibData vibData_;
};

