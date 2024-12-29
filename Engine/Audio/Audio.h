#pragma once
//オーディオクラス//
#include "Common/DirectX/DirectXCommon.h"
#include "Math_Structs.h"
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <x3daudio.h>
#include <cassert>
#include <fstream>
#include <array>

#pragma region 

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#pragma endregion MicroSoft Media foundation

struct ChunkHeader {
	char id[4];//チャンクID
	int32_t size;//チャンクサイズ
};
struct RiffHeader {
	ChunkHeader chunk;//"RIFF"
	char type[4];//"WAVE"
};
struct FormatChunk {
	ChunkHeader chunk;//"fmt "
	WAVEFORMATEX fmt;//フォーマット
};
struct SoundData {
	////波形フォーマット
	WAVEFORMATEX wfex;
	////バッファの先頭
	//BYTE* pBuffer;
	////バッファのサイズ
	unsigned int bufferSize;
	// .data pAudioData

 	std::vector<BYTE> mediaData;
	// 名前
	std::string name;
	// 使っているかどうか
	bool IsUsed = false;
};
class Audio {
public:
	static Audio* GetInstance();
	void Initialize();

	void Release();
#pragma region
	/// <summary>
	/// wavファイルを読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="LoopFlag">ループ再生するかどうか</param>
	/// <returns></returns>
	uint32_t LoadAudio(const std::string& filePath, bool LoopFlag);
	/// <summary>
	/// mp3ファイルを読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="LoopFlag">ループ再生するかどうか</param>
	/// <returns></returns>
	uint32_t LoadAudioMP3(const std::string& filePath, bool LoopFlag);
#pragma endregion 読み込み

#pragma region
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="AudioIndex">オーディオファイルの番号</param>
	/// <param name="AudioVolume">音量</param>
	void Play(uint32_t AudioIndex, float AudioVolume, int pan);
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="AudioIndex">オーディオファイルの番号</param>
	/// <param name="AudioVolume">音量</param>
	void Play(uint32_t AudioIndex, float AudioVolume);
	/// <summary>
	/// 停止
	/// </summary>
	/// <param name="AudioIndex">オーディオファイルの番号</param>
	/// <param name="PlayBegin">再生位置を戻す</param>
	/// <param name="LoopFlag">ループ再生するかどうか</param>
	void Stop(uint32_t AudioIndex, bool PlayBegin, bool LoopFlag);
	/// <summary>
	/// ループを抜ける
	/// </summary>
	/// <param name="AudioIndex">オーディオファイルの番号</param>
	void ExitLoop(uint32_t AudioIndex);
	/// <summary>
	/// 再生位置を最初に戻す
	/// </summary>
	/// <param name="AudioIndex">オーディオファイルの番号</param>
	/// <param name="LoopFlag">ループ再生するかどうか</param>
	void Reset(uint32_t AudioIndex, bool LoopFlag);
	/// <summary>
	/// サウンドのアンロード
	/// </summary>
	/// <param name="AudioIndex">オーディオファイルの番号</param>
	void SoundUnload(uint32_t AudioIndex);
#pragma endregion オーディオコントロール
private:
	Audio() = default;
	~Audio() = default;
	Audio(const Audio& obj) = delete;
	Audio& operator=(const Audio& obj) = delete;

	float left = 0;
	float right = 0;
	static const int kMaxAudio = 64;
	HRESULT hr;
	Microsoft::WRL::ComPtr<IXAudio2> XAudioInterface = nullptr;
	IXAudio2MasteringVoice* pMasteringVoice = nullptr;
	//ソースボイス
	IXAudio2SourceVoice* pSourceVoice[kMaxAudio];
	bool IsusedAudioIndex[kMaxAudio];
	//生音声データ
	//再生中にぶっ飛ばすとバグるぜ！！！
	std::array<SoundData, kMaxAudio> soundData_;
	//再生中にぶっ飛ばすとバグるぜ！！！


	float outputMatrix[8];

	/// <summary>
	/// サウンドの読み込み(wav)
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	SoundData SoundLoadWave(const std::string& filePath);
	/// <summary>
	/// サウンドの読み込み(mp3)
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	SoundData SoundLoadMP3(const std::string& filePath);


};

