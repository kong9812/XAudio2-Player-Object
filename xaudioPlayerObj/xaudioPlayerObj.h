#ifndef _XAUDIO_PLAYER_OBJECT_H_ 
#define	_XAUDIO_PLAYER_OBJECT_H_

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEFAULT_VOLUME	(1.0f)

//*****************************************************************************
// 定数定義
//*****************************************************************************
enum PLAY_STATE				// 再生状態
{
	STATE_ERROR,
	STATE_READY_TO_PLAY,
	STATE_PLAYING,
	STATE_END_OR_NOTHING
};

enum TWO_CHANNELS			// 2チャンネル用
{
	CHANNELS_LEFT,
	CHANNELS_RIGHT
};

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct	// ボイス用
{
	float freq;
	float volume;
	float channels;

}VOICE_TOOL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitXAudio2(void);		// 初期化
void	UninitXAudio2(void);	// 終了処理

// ロードサウンド
IXAudio2SourceVoice *LoadXAudio2Voice(char *path);	

// 再生
void PlayXAudio2Voice(IXAudio2SourceVoice *sourceVoice);

// 停止
void StopXAudio2Voice(IXAudio2SourceVoice *sourceVoice);

// ボリュームの変更
void SetVolume(IXAudio2SourceVoice *sourceVoice, 
	int channels, float singleVolume, float mainVolume);

// 再生用サンプリング周波数の変更
void SetSampleFrequency(IXAudio2SourceVoice *sourceVoice, float freq);

// 再生状態を取得
XAUDIO2_VOICE_STATE GetXAudio2State(IXAudio2SourceVoice *sourceVoice);
int GetXAudio2StateEnum(IXAudio2SourceVoice *sourceVoice);

// パンボリューム
HRESULT SetPanVolume(IXAudio2SourceVoice *voice, float panVolume);

#endif
