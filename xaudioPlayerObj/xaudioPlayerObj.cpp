#include "main.h"
#include "loadSound.h"
#include "xaudioPlayerObj.h"

//==============================================================
// グローバル変数
//==============================================================
IXAudio2MasteringVoice	*masteringVoice = NULL;	// マスターボイス
IXAudio2				*xAudio2 = NULL;		// XAudio2のインターフェイス
float					*channelVolume = NULL;	// ボリューム

//==============================================================
// 初期化
//==============================================================
HRESULT InitXAudio2(void)
{
	// COMライブラリの初期化
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	if (FAILED(XAudio2Create(&xAudio2)))
	{
		return E_FAIL;
	}

	// マスターボイスの作成
	if (FAILED(xAudio2->CreateMasteringVoice(&masteringVoice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==============================================================
// ロードサウンド
//==============================================================
IXAudio2SourceVoice *LoadXAudio2Voice(char *path)
{
	IXAudio2SourceVoice *sourceVoice = NULL;	// ソースボイス
	WAVEFORMATEX		pcm;					// WAVEFORMATEX構造体
	XAUDIO2_BUFFER		buffer;					// バッファ

	{// ローカル変数の初期化

		// 構造体の初期化
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// wavファイルの読み込み(WAVEFORMATEX構造体)
		pcm = LoadWavFile(path);

		// バッファの設定
		buffer.pAudioData = (BYTE*)GetWavFile()->data.waveData;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes = GetWavFile()->data.waveSize;
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// ソースボイスの作成
	if (FAILED(xAudio2->CreateSourceVoice(&sourceVoice, &pcm, NULL, XAUDIO2_MAX_FREQ_RATIO)))
	{
		return NULL;
	}

	// バッファを提出
	if (FAILED(sourceVoice->SubmitSourceBuffer(&buffer)))
	{
		return NULL;
	}

	{// ボリュームの初期化
		// メモリ確保[チャンネル数]
		channelVolume = (float *)malloc(GetWavFile()->fmt.fmtChannel * sizeof(float));
		for (int i = 0; i < GetWavFile()->fmt.fmtChannel; i++)
		{
			channelVolume[i] = DEFAULT_VOLUME;
		}
	}

	return sourceVoice;
}

//==============================================================
// 終了処理
//==============================================================
void UninitXAudio2(void)
{
	// COMライブラリの終了処理
	CoUninitialize();

	// マスターボイス
	if (masteringVoice != NULL)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = NULL;
	}

	// インターフェース
	if (xAudio2 != NULL)
	{
		xAudio2->Release();
		xAudio2 = NULL;
	}

	// LoadSoundの終了処理
	UninitLoadSound();

	// ボリュームのメモリ解放
	free(channelVolume);
}

//==============================================================
// 再生
//==============================================================
void PlayXAudio2Voice(IXAudio2SourceVoice *sourceVoice)
{
	if (sourceVoice != NULL)
	{
		// 再生可能？
		if (GetXAudio2StateEnum(sourceVoice) == STATE_READY_TO_PLAY)
		{
			// 再生
			sourceVoice->Start(0);
		}
	}
}

//==============================================================
// 停止
//==============================================================
void StopXAudio2Voice(IXAudio2SourceVoice *sourceVoice)
{
	if (sourceVoice != NULL)
	{
		// 再生中？
		if (GetXAudio2StateEnum(sourceVoice) == STATE_PLAYING)
		{
			// 停止
			sourceVoice->Stop(0);
		}
	}
}

//==============================================================
// 再生状態を取得
// 戻り値:XAUDIO2_VOICE_STATE state
//==============================================================
XAUDIO2_VOICE_STATE GetXAudio2State(IXAudio2SourceVoice *sourceVoice)
{
	XAUDIO2_VOICE_STATE state;

	if (sourceVoice != NULL)
	{
		// 構造体の初期化
		memset(&state, 0, sizeof(XAUDIO2_VOICE_STATE));

		// 再生状態を取得
		sourceVoice->GetState(&state);
	}

	return state;
}

//==============================================================
// ボリュームの変更
//==============================================================
void SetVolume(IXAudio2SourceVoice *sourceVoice, int channels, float singleVolume, float mainVolume)
{	
	if (sourceVoice != NULL)
	{
		if (mainVolume == NULL)
		{
			// 1チャンネル
			channelVolume[channels] = singleVolume;

			// 再生用サンプリング周波数[シングルチャンネル]
			sourceVoice->SetChannelVolumes((UINT32)GetWavFile()->fmt.fmtChannel, channelVolume);
		}

		else
		{
			// 全部のチャンネル
			for (int i = 0; i < GetWavFile()->fmt.fmtChannel; i++)
			{
				channelVolume[i] = mainVolume;
			}

			// 再生用サンプリング周波数[シングルチャンネル]
			sourceVoice->SetChannelVolumes((UINT32)GetWavFile()->fmt.fmtChannel, channelVolume);

		}
	}

}

//==============================================================
// 再生用サンプリング周波数の変更
//==============================================================
void SetSampleFrequency(IXAudio2SourceVoice *sourceVoice, float freq)
{
	if (sourceVoice != NULL)
	{
		// 再生用サンプリング周波数
		sourceVoice->SetFrequencyRatio(freq);
	}
}

//==============================================================
// 再生状態を取得
// 戻り値:int state[enum]
//==============================================================
int GetXAudio2StateEnum(IXAudio2SourceVoice *sourceVoice)
{
	XAUDIO2_VOICE_STATE state;
	int stateEnum = NULL;

	if (sourceVoice != NULL)
	{
		// 再生状態を取得
		state = GetXAudio2State(sourceVoice);

		if ((state.BuffersQueued > NULL) && (state.SamplesPlayed == NULL))
		{
			stateEnum = STATE_READY_TO_PLAY;

			return stateEnum;
		}

		if ((state.BuffersQueued > NULL) && (state.SamplesPlayed > NULL))
		{
			stateEnum = STATE_PLAYING;

			return stateEnum;
		}

		if ((state.BuffersQueued == NULL) && (state.SamplesPlayed == NULL))
		{
			stateEnum = STATE_END_OR_NOTHING;

			return stateEnum;
		}
	}

	return STATE_ERROR;
}

//==============================================================
// パンボリューム
//==============================================================
HRESULT SetPanVolume(IXAudio2SourceVoice *voice, float panVolume)
{
	HRESULT hr;
	float vol[4];

	float tmpVolume = panVolume + 1.0f;
	float radian = TO_RADIAN((tmpVolume / 2.0f) * 90.0f);

	// Left
	vol[0] = sinf(radian);
	vol[1] = 0.0f;

	// Right
	vol[2] = 0.0f;
	vol[3] = cosf(radian);

	hr = voice->SetOutputMatrix(masteringVoice, GetWavFile()->fmt.fmtChannel, 2, vol);

	return hr;
}