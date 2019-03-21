#include "main.h"
#include "xaudioPlayerObj.h"
#include "loadSound.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
WAV_FILE	wavFile;	// WAVファイル

//*****************************************************************************
// WAVファイルの読み込む(WAVEFORMATEX構造体)
//*****************************************************************************
WAVEFORMATEX LoadWavFile(char *path)
{
	WAVEFORMATEX	pcm;				// WAVEFORMATEX構造体
	FILE			*fp;				// ファイル構造体							
	int				chunkFlag = NULL;	// チャンクを探すループのフラグ

	{// ローカル変数の初期化
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&fp, 0, sizeof(FILE));
	}
	// ファイルの準備
	fopen_s(&fp, path, "rb");
	
	// RIFFの読み込み
	fread(&wavFile.riff, sizeof(RIFF_CHUNK), 1, fp);

	// FMTとDATAの読み込み
	while (chunkFlag != FLAG_CHUNK_END)
	{
		char	chunk[CHUNK_SIZE];
		long	size = 0;

		// チャンクとサイズの読み込み
		fread(&chunk, sizeof(chunk), 1, fp);
		fread(&size, sizeof(size), 1, fp);

		// fmtチャンク
		if (memcmp(chunk, CHUNK_FMT, CHUNK_SIZE) == CHUNK_SAME)
		{
			// チャンクとサイズを設定
			memcpy(wavFile.fmt.fmtChunk, chunk, CHUNK_SIZE);
			wavFile.fmt.fmtSize = size;

			// フォーマットIDから読み込み
			fread(&wavFile.fmt.fmtFormatTag,
				sizeof(FMT_CHUNK) - 8, 1, fp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_FMT;

			continue;
		}

		// dataチャンク
		if (memcmp(chunk, CHUNK_DATA, CHUNK_SIZE) == CHUNK_SAME)
		{
			// チャンクとサイズを設定
			memcpy(wavFile.data.dataChunk, CHUNK_DATA, CHUNK_SIZE);
			wavFile.data.waveSize = size;

			// メモリ確保
			wavFile.data.waveData = (short *)malloc(wavFile.data.waveSize);

			// 波形の読み込み
			fread(wavFile.data.waveData, wavFile.data.waveSize, 1, fp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_DATA;

			continue;
		}

		// 他のチャンク
		else
		{
			// size分をskip
			fseek(fp, size, SEEK_CUR);
		}
	}

	/* wav構造体の初期化 */
	pcm.cbSize = 0;
	pcm.nChannels = wavFile.fmt.fmtChannel;
	pcm.wBitsPerSample = wavFile.fmt.fmtBitPerSample;
	pcm.nSamplesPerSec = wavFile.fmt.fmtSampleRate;
	pcm.wFormatTag = WAVE_FORMAT_PCM;
	pcm.nBlockAlign = (wavFile.fmt.fmtChannel*wavFile.fmt.fmtBitPerSample) / 8;
	pcm.nAvgBytesPerSec = wavFile.fmt.fmtSampleRate*pcm.nBlockAlign;

	fclose(fp);

	return pcm;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitLoadSound(void)
{
	free(wavFile.data.waveData);
}

//*****************************************************************************
// WAVファイルの情報を取得
//*****************************************************************************
WAV_FILE *GetWavFile(void)
{
	return &wavFile;
}
