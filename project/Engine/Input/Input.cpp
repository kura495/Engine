#include"Input.h"

std::list<ListData> Input::joy_stack;
std::list<ListData> Input::joy_stacklog;
const int Input::save_frame = 20;
const int Input::log_save_frame = 120;
std::array<int, 16> Input::joy_frame;
XINPUT_STATE Input::joyState;
XINPUT_STATE Input::joyStatePre;
VibData Input::vibData_;

#define XINPUT_GAMEPAD_LEFT_TRIGER    0x0400
#define XINPUT_GAMEPAD_RIGHT_TRIGER   0x0800

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

	m_Position.Pos = PositionMouse();
	m_Position.Scroll += ScrollMouse();

	//前フレームの結果を代入
	joyStatePre = joyState;
	GetJoystickState();
	UpdateJoyState();
#ifdef _DEBUG
	ImGui();
#endif
}

bool Input::pushKey(uint8_t keyNumber)
{
	if (key[keyNumber] != 0 && keyPre[keyNumber] == 0){
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

void Input::VibrateController(int leftMotor, int rightMotor,float second)
{
	vibData_.leftMotor = leftMotor;
	vibData_.rightMotor = rightMotor;
	vibData_.second = second;
	vibData_.countFrame = 0.0f;
}

bool Input::pushMouse(uint32_t Mousebutton)
{
	if (mouse_.rgbButtons[Mousebutton] != 0) {
		return true;
	}
	return false;
}

bool Input::GetJoystickState(XINPUT_STATE& joy) {
	DWORD dwResult = XInputGetState(0, &joy);
	Vector3 move = { (float)joy.Gamepad.sThumbLX, 0.0f, (float)joy.Gamepad.sThumbLY };

	if (move.Length() < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		joy.Gamepad.sThumbLX = 0;
		joy.Gamepad.sThumbLY = 0;
	}
	move = { (float)joy.Gamepad.sThumbRX, 0.0f, (float)joy.Gamepad.sThumbRY };
	if (move.Length() < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		joy.Gamepad.sThumbRX = 0;
		joy.Gamepad.sThumbRY = 0;
	}
	return dwResult == ERROR_SUCCESS;

}

bool Input::GetJoystickState()
{
	DWORD dwResult = XInputGetState(0,&joyState);
	Vector3 move = { (float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};

	if (move.Length() < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		joyState.Gamepad.sThumbLX = 0;
		joyState.Gamepad.sThumbLY = 0;
	}
	move = { (float)joyState.Gamepad.sThumbRX, 0.0f, (float)joyState.Gamepad.sThumbRY };
	if (move.Length() < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		joyState.Gamepad.sThumbRX = 0;
		joyState.Gamepad.sThumbRY = 0;
	}
	return dwResult == ERROR_SUCCESS;

}

void Input::UpdateJoyState()
{
	//ボタン入力情報の更新
	UpdateButtan();
	//バイブレーション情報の更新
	UpdateVibration();
}

bool Input::GetPadPrecede(uint32_t buttonNumber, int delayTime)
{
	//押されたボタンがbuttonNumberと同じで、
	//経過フレームがdelayTimeより小さいならtrueを返す
	auto result = find_if(joy_stack.begin(), joy_stack.end(),
		[buttonNumber, delayTime](ListData data){
	return data.code == buttonNumber && data.frame < delayTime;
	});
	if (result != joy_stack.end()) {
		joy_stack.erase(result);
		return true;
	}
	else {
		return false;
	}
}

Vector2 Input::PositionMouse() {
	return { (float)mouse_.lX,(float)mouse_.lY };
}

float Input::ScrollMouse()
{
	return (float)mouse_.lZ;
}

void Input::UpdateVibration()
{
	if (vibData_.countFrame > vibData_.second) {
		vibData_.leftMotor = 0;
		vibData_.rightMotor = 0;
	}
	else {
		//情報の、経過フレームを計算
		vibData_.countFrame += kDeltaTime;
	}

	vibration_.wLeftMotorSpeed = WORD(vibData_.leftMotor);
	vibration_.wRightMotorSpeed = WORD(vibData_.rightMotor);
	//バイブレーションの情報
	XInputSetState(0, &vibration_);
}

void Input::ImGui()
{
	ImGui::Begin("Input");
	ImGui::Text("ImPut");
	for (ListData& data : joy_stacklog) {
			ImGui::Text(PadButtonList(data.code).c_str());
			data.frame++;
	}
	// 一定フレーム経った入力情報を削除
	// data.frameがsave_frameを超えたら削除
	joy_stacklog.erase(remove_if(joy_stacklog.begin(), joy_stacklog.end(),
			[](ListData data) { return data.frame >= log_save_frame; }),
		joy_stacklog.end());

	ImGui::End();
}

void Input::UpdateButtan()
{
	// スタックされた入力情報の、経過フレームを計算
	for (auto& data : joy_stack) {
		data.frame++;
	}
	//2のi乗して押されていたらスタック
	for (int i = 0; i < 16; i++) {
		if (joy_frame[i] == -1) joy_frame[i]++;
		int buttonNum = (int)std::pow(2, i);
		if (joyState.Gamepad.wButtons & buttonNum) {
			joy_frame[i]++;
			// 押された瞬間に入力情報をスタック
			if (joy_frame[i] == 1) {
				joy_stack.emplace_back(ListData(buttonNum));
				joy_stacklog.emplace_back(ListData(buttonNum));
			}
		}
		else if (joy_frame[i] > 0) {
			joy_frame[i] = -1;
		}
	}
	// 一定フレーム経った入力情報を削除
	// data.frameがsave_frameを超えたら削除
	joy_stack.erase(
		remove_if(joy_stack.begin(), joy_stack.end(),
			[](ListData data) { return data.frame >= save_frame; }),
		joy_stack.end());
}

std::string Input::PadButtonList(uint32_t Number)
{
	if (Number & 0x0001) {
		return "DPAD_UP";
	}
	else if (Number & 0x0002) {
		return "DPAD_DOWN";
	}	
	else if (Number & 0x0004) {
		return "DPAD_LEFT";
	}	
	else if (Number & 0x0008) {
		return "DPAD_RIGHT";
	}	
	else if (Number & 0x0010) {
		return "GAMEPAD_START";
	}	
	else if (Number & 0x0020) {
		return "GAMEPAD_BACK";
	}	
	else if (Number & 0x0040) {
		return "LEFT_THUMB";
	}	
	else if (Number & 0x0080) {
		return "RIGHT_THUMB";
	}	
	else if (Number & 0x0100) {
		return "LEFT_SHOULDER";
	}	
	else if (Number & 0x0200) {
		return "RIGHT_SHOULDER ";
	}	
	else if (Number & 0x0400) {
		return "LEFT_TRIGER";
	}	
	else if (Number & 0x0800) {
		return "RIGHT_TRIGER";
	}	
	else if (Number & 0x1000) {
		return "GAMEPAD_A";
	}	
	else if (Number & 0x2000) {
		return "GAMEPAD_B";
	}	
	else if (Number & 0x4000) {
		return "GAMEPAD_X";
	}
	else if (Number & 0x8000) {
		return "GAMEPAD_Y";
	}
	return "";
}


