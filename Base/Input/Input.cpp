#include"Input.h"

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(WinApp*winApp_){
	//DirectInputのオブジェクトを作成
	hr = DirectInput8Create(winApp_->GetHINSTANCE(),DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&directInput,nullptr);
	assert(SUCCEEDED(hr));
	//キーボードデバイスを生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(winApp_->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	key = {};
	keyPre = {};
	//マウスデバイスを作成
	hr = directInput->CreateDevice(GUID_SysMouse,&mouse,NULL);
	//入力データ形式のセット
	hr = mouse->SetDataFormat(&c_dfDIMouse);
	//排他制御レベルのセット
	hr = mouse->SetCooperativeLevel(winApp_->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	//コントローラー
	GetJoystickState();
}

void Input::Update()
{
	//前フレームの結果を代入
	keyPre = key;
	//キーボード情報の取得開始
	keyboard->Acquire();
	key = {};
	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key.data());

	//前フレームの結果を代入
	joyStatePre = joyState;

	GetJoystickState();
}

bool Input::pushKey(uint8_t keyNumber)
{
	if (key[keyNumber]!=0&& keyPre[keyNumber]==0) {
		return true;
	}
	return false;
}
bool Input::TriggerKey(uint8_t keyNumber)
{
	if (keyPre[keyNumber] != 0) {
		return true;
	}
	return false;
}

bool Input::IsTriggerKey(uint8_t keyNumber)
{
	if (key[keyNumber] == 0 && keyPre[keyNumber] != 0) {
		return true;
	}
	return false;
}

bool Input::pushPad(uint32_t buttonNumber)
{
	if (joyState.Gamepad.wButtons & buttonNumber && !(joyStatePre.Gamepad.wButtons & buttonNumber)) {
		return true;
	}
	return false;
}

bool Input::IsTriggerPad(uint32_t buttonNumber)
{
	if (joyState.Gamepad.wButtons & buttonNumber && joyStatePre.Gamepad.wButtons & buttonNumber) {
		return true;
	}
	return false;
}

bool Input::GetJoystickState()
{
	DWORD dwResult = XInputGetState(0,&joyState);
	Vector3 move = { (float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};

	if (Length(move) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		joyState.Gamepad.sThumbLX = 0;
		joyState.Gamepad.sThumbLY = 0;
	}
	move = { (float)joyState.Gamepad.sThumbRX, 0.0f, (float)joyState.Gamepad.sThumbRY };
	if (Length(move) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		joyState.Gamepad.sThumbRX = 0;
		joyState.Gamepad.sThumbRY = 0;
	}
	return dwResult == ERROR_SUCCESS;

}

bool Input::GetMouse()
{

	return false;
}
