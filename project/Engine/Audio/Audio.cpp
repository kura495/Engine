#include"Audio.h"
const int Audio::kMaxAudio;
IXAudio2SourceVoice* Audio::pSourceVoice[kMaxAudio];
std::array<SoundData, Audio::kMaxAudio> Audio::soundData_;
Audio* Audio::GetInstance()
{
	static Audio instance;
	return &instance;
}
void Audio::Initialize() {
#pragma region
	// COMの初期化
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));
	// Madia Foundationの初期化
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
#pragma endregion Madia Foundation

	hr = XAudio2Create(&XAudioInterface, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));
	hr = XAudioInterface->CreateMasteringVoice(&pMasteringVoice);
	assert(SUCCEEDED(hr));
	for (int i = 0; i < kMaxAudio; i++) {
		pSourceVoice[i] = nullptr;
		IsusedAudioIndex[i] = false;
	}
}

uint32_t Audio::LoadAudio(const std::string& filePath, bool LoopFlag) {

#pragma region Index
	uint32_t index = 0;
	//同じファイルを読み込んでいた場合、同じファイルのインデックス番号を返す
	for (uint32_t index_i = 0; index_i < kMaxAudio; index_i++) {
		if (soundData_.at(index_i).IsUsed) {
			if (filePath == soundData_.at(index_i).name) {
				return index_i;
			}
		}
	}
	//使われていないインデックス番号を検索
	for (uint32_t index_i = 0; index_i < kMaxAudio; index_i++) {
		if (soundData_.at(index_i).IsUsed == false) {
			index = index_i;
			break;
		}
	}
	//SoundLoadWaveの戻り値がSoundDataなので、読み込んでから名前と使用済みを記入
	soundData_[index] = SoundLoadWave(filePath);


	//名前としてファイルのパスを登録
	soundData_.at(index).name = filePath;

	soundData_.at(index).IsUsed = true;
#pragma endregion 位置決め

	if (FAILED(GetInstance()->GetIXAudio2()->CreateSourceVoice(&pSourceVoice[index], &soundData_[index].wfex))) {
		SoundUnload(index);
		assert(false);
	}
	//バッファを作成
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = soundData_[index].mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = soundData_[index].bufferSize;
	buffer.LoopBegin = 0;
	buffer.LoopLength = 0;
	buffer.LoopCount = LoopFlag ? XAUDIO2_LOOP_INFINITE : 0;

	pSourceVoice[index]->SubmitSourceBuffer(&buffer);

	return index;
}

uint32_t Audio::LoadAudioMP3(const std::string& filePath, bool LoopFlag)
{

#pragma region Index
	uint32_t index = 0;
	//同じファイルを読み込んでいた場合、同じファイルのインデックス番号を返す
	for (uint32_t index_i = 0; index_i < kMaxAudio; index_i++) {
		if (soundData_.at(index_i).IsUsed) {
			if (filePath == soundData_.at(index_i).name) {
				Stop(index_i,true,false);
				return index_i;
			}
		}
	}
	//使われていないインデックス番号を検索
	for (uint32_t index_i = 0; index_i < kMaxAudio; index_i++) {
		if (soundData_.at(index_i).IsUsed == false) {
			index = index_i;
			break;
		}
	}
	//SoundLoadWaveの戻り値がSoundDataなので、読み込んでから名前と使用済みを記入
	soundData_[index] = SoundLoadMP3(filePath);
	//名前としてファイルのパスを登録
	soundData_.at(index).name = filePath;
	soundData_.at(index).IsUsed = true;
#pragma endregion 位置決め

	if (FAILED(GetInstance()->GetIXAudio2()->CreateSourceVoice(&pSourceVoice[index], &soundData_[index].wfex))) {
		SoundUnload(index);
		assert(false);
	}
	//バッファを作成
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = soundData_[index].mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = soundData_[index].bufferSize;
	buffer.LoopBegin = 0;
	buffer.LoopLength = 0;
	buffer.LoopCount = LoopFlag ? XAUDIO2_LOOP_INFINITE : 0;

	pSourceVoice[index]->SubmitSourceBuffer(&buffer);

	return index;
}

void Audio::Release() {

	for (int i = 0; i < kMaxAudio; i++) {
		if (pSourceVoice[i]) {
			pSourceVoice[i]->DestroyVoice();
		}
	}
	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}
	if (XAudioInterface) {
		//XAudioInterface->Release();
		//XAudioInterface = nullptr;
	}
	MFShutdown();
	CoUninitialize();
}

void Audio::Play(uint32_t AudioIndex, float AudioVolume) {
	pSourceVoice[AudioIndex]->SetVolume(AudioVolume);
	pSourceVoice[AudioIndex]->Start(0);
}

void Audio::Stop(uint32_t AudioIndex, bool PlayBegin, bool LoopFlag) {

	pSourceVoice[AudioIndex]->Stop(0);
	if (PlayBegin) {
		Reset(AudioIndex, LoopFlag);
	}
}

void Audio::ExitLoop(uint32_t AudioIndex)
{
	pSourceVoice[AudioIndex]->ExitLoop();
}

void Audio::Reset(uint32_t AudioIndex,bool LoopFlag) {
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData = soundData_[AudioIndex].mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = soundData_[AudioIndex].bufferSize;
	buffer.LoopBegin = 0;
	buffer.LoopLength = 0;
	buffer.LoopCount = LoopFlag ? XAUDIO2_LOOP_INFINITE : 0;
	pSourceVoice[AudioIndex]->FlushSourceBuffers();
	pSourceVoice[AudioIndex]->SubmitSourceBuffer(&buffer);
}

SoundData Audio::SoundLoadWave(const std::string& filePath)
{
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(filePath, std::ios_base::binary);
	//ファイルオープン失敗を検出
	assert(file.is_open());

	RiffHeader riff;
	FormatChunk format = {};
	ChunkHeader data;
	//RIFFチャンク読み込み

	//チャンクがRIFFかチェック
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//ファイルタイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	//formatチャンク読み込み

	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	//JUNKチャンクの場合
	if (strncmp(format.chunk.id, "JUNK", 4) == 0) {
		//JUNKチャンクの終わりまで進める
		file.seekg(format.chunk.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&format, sizeof(data));
	}

	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンク読み込み

	file.read((char*)&data, sizeof(data));
	//bextチャンクの場合
	if (strncmp(data.id, "bext", 4) == 0) {
		//bextチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	//LISTチャンクの場合
	if (strncmp(data.id, "LIST", 4) == 0) {
		//LISTチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	//JUNKチャンクの場合
	if (strncmp(data.id, "junk", 4) == 0) {
		//JUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}
	//Dataチャンクの波形データを読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.bufferSize = sizeof(BYTE) * static_cast<UINT32>(soundData.mediaData.size());

	return soundData;
}

SoundData Audio::SoundLoadMP3(const std::string& filePath)
{
	// SourceLeaderを作る
	IMFSourceReader* pMFSourceReader{ nullptr };
	MFCreateSourceReaderFromURL(ConvertMultiBype(filePath), NULL, &pMFSourceReader);
#pragma region
	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	// メジャータイプをオーディオにする
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	// サブタイプをPCMにする　
	// PCM = オーディオのフォーマット
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	pMFSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	pMFSourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);
#pragma endregion MadiaType

	WAVEFORMATEX* waveFormat{ nullptr };
	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);

	SoundData soundData;
	BYTE* pMFBuffer{ nullptr };
	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		pMFSourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);


		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pMFBuffer, nullptr, &cbCurrentLength);

		soundData.mediaData.resize(soundData.mediaData.size() + cbCurrentLength);
		memcpy(soundData.mediaData.data() + soundData.mediaData.size() - cbCurrentLength, pMFBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}

	soundData.wfex = *waveFormat;
	soundData.bufferSize = sizeof(BYTE) * static_cast<UINT32>(soundData.mediaData.size());


	CoTaskMemFree(waveFormat);
	pMFMediaType->Release();
	pMFSourceReader->Release();

	return soundData;
}

void Audio::SoundUnload(uint32_t AudioIndex)
{
	soundData_[AudioIndex].mediaData.clear();
	soundData_[AudioIndex].wfex = {};
}