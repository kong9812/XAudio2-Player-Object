#include "main.h"
#include "xaudioPlayerObj.h"

//==============================================================
// プロトタイプ宣言
//==============================================================
void Uninit(void);		// 終了処理
int Init(void);			// 初期化

//==============================================================
// グローバル変数
//==============================================================
IXAudio2SourceVoice	*voice = NULL;	// ソースボイス

#if 0	// ボイスツール構造体
VOICE_TOOL	voiceTool;		// ボイスツール
#endif

bool loop = true;			// ループ

//==============================================================
// メイン処理
//==============================================================
int main(void)
{
	// メッセージ
	printf("INIT\n");

	// 初期化
	if (Init()==1)
	{
		// エラー!!
		// ループに入らないように
		loop = false;
	}

	while (loop)
	{	// メッセージ
		printf("PLAY\n");

		// 再生
		PlayXAudio2Voice(voice);

		float tmpVol = -1.0f;
		while (tmpVol < 1.0f)
		{
			// 
			tmpVol += 0.0000001f;

			// test
			SetPanVolume(voice, tmpVol);
		}

		// ボタン押したら進む
		getchar();

		// メッセージ
		printf("RIGHT CHANNEL VOLUME:0%%\n");

		// シングルチャンネルボリュームを変更
		SetVolume(voice, CHANNELS_RIGHT, 0.0f, NULL);

		// ボタン押したら進む
		getchar();

		// メッセージ
		printf("BOTH CHANNEL VOLUME:100%%\n");

		// 全部のチャンネルボリュームを変更
		SetVolume(voice, NULL, NULL, 1.0f);

		// ボタン押したら進む
		getchar();

		// メッセージ
		printf("NORMAL FREQUENCY * 20\n");

		// サンプリング周波数を変更
		SetSampleFrequency(voice, 20);

		// ボタン押したら進む
		getchar();

		// メッセージ
		printf("STOP\n");

		// 停止
		StopXAudio2Voice(voice);

		// ボタン押したら進む
		getchar();

		// ループかラ抜く
		loop = false;
	}

	// 終了処理
	Uninit();

	return 0;
}

//==============================================================
// 初期化
//==============================================================
int Init(void)
{
	// 初期化
	if (FAILED(InitXAudio2()))
	{
		// 終了処理
		Uninit();

		return 1;
	}

	// 読み込み
	voice = LoadXAudio2Voice((char *)SOUND_PATH);
	if (voice == NULL)
	{
		// 終了処理
		Uninit();

		return 1;
	}

	return 0;
}

//==============================================================
// 終了処理
//==============================================================
void Uninit(void)
{
	UninitXAudio2();
}

