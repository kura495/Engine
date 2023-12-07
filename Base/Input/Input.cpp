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
	hr = directInput->CreateDevice(GUID_SysKeyboard, &divKeyboard, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = divKeyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = divKeyboard->SetCooperativeLevel(winApp_->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	key = {};
	keyPre = {};
	//マウスデバイスを作成
	hr = directInput->CreateDevice(GUID_SysMouse,&divMouse,NULL);
	//入力データ形式のセット
	hr = divMouse->SetDataFormat(&c_dfDIMouse2);
	//排他制御レベルのセット
	hr = divMouse->SetCooperativeLevel(winApp_->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//コントローラー
	GetJoystickState();
}

void Input::Update()
{
	//前フレームの結果を代入
	keyPre = key;
	//キーボード情報の取得開始
	divKeyboard->Acquire();
	key = {};
	//全キーの入力状態を取得する
	divKeyboard->GetDeviceState(sizeof(key), key.data());

	mousePre_ = mouse_;
	//マウス情報の取得開始
	divMouse->Acquire();
	
	divMouse->GetDeviceState(sizeof(mouse_),&mouse_);

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
bool Input::TriggerPad(uint32_t buttonNumber)
{
	if (joyState.Gamepad.wButtons & buttonNumber) {
		return true;
	}
	return false;
}

bool Input::IsTriggerPad(uint32_t buttonNumber)
{
	if (!(joyState.Gamepad.wButtons & buttonNumber) && joyStatePre.Gamepad.wButtons & buttonNumber) {
		return true;
	}
	return false;
}

bool Input::pushMouse(uint32_t Mousebutton)
{
	if (mouse_.rgbButtons[Mousebutton] != 0) {
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
