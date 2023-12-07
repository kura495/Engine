#pragma once
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include <wrl.h>
#include "WinApp.h"
#include <array>

#include <XInput.h>

#include "Base/Math/VectorCalc.h"

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
	/// <returns></returns>
	bool pushKey(uint8_t keyNumber);
	/// <summary>
	/// 押している間
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool TriggerKey(uint8_t keyNumber);
	/// <summary>
	/// 離した瞬間
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool IsTriggerKey(uint8_t keyNumber);
	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool pushPad(uint32_t buttonNumber);
	/// <summary>
	/// 押している間
	/// </summary>
	/// <param name="buttonNumber">XINPUT_GAMEPAD</param>
	/// <returns></returns>
	bool IsTriggerPad(uint32_t buttonNumber);

private:
	Input() = default;
	~Input() = default;
	Input(const Input& obj) = delete;
	Input& operator=(const Input& obj) = delete;

	/// <summary>
	/// ゲームパッドを取得
	/// </summary>
	bool GetJoystickState();
	/// <summary>
	/// マウスの情報を取得
	/// </summary>
	/// <returns></returns>
	bool GetMouse();
	
	HRESULT hr;
	Microsoft::WRL::ComPtr<IDirectInput8>directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse = nullptr;
	//キーボード
	std::array<BYTE, 256> key;
	std::array<BYTE, 256> keyPre;
	//マウス
	DIMOUSESTATE2 mouse_;
	DIMOUSESTATE2 mousePre_;
	//パッド
	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;
};

