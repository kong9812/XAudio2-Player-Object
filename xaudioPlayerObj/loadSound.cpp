#include "main.h"
#include "xaudioPlayerObj.h"
#include "loadSound.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
WAV_FILE	wavFile;	// WAV�t�@�C��

//*****************************************************************************
// WAV�t�@�C���̓ǂݍ���(WAVEFORMATEX�\����)
//*****************************************************************************
WAVEFORMATEX LoadWavFile(char *path)
{
	WAVEFORMATEX	pcm;				// WAVEFORMATEX�\����
	FILE			*fp;				// �t�@�C���\����							
	int				chunkFlag = NULL;	// �`�����N��T�����[�v�̃t���O

	{// ���[�J���ϐ��̏�����
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&fp, 0, sizeof(FILE));
	}
	// �t�@�C���̏���
	fopen_s(&fp, path, "rb");
	
	// RIFF�̓ǂݍ���
	fread(&wavFile.riff, sizeof(RIFF_CHUNK), 1, fp);

	// FMT��DATA�̓ǂݍ���
	while (chunkFlag != FLAG_CHUNK_END)
	{
		char	chunk[CHUNK_SIZE];
		long	size = 0;

		// �`�����N�ƃT�C�Y�̓ǂݍ���
		fread(&chunk, sizeof(chunk), 1, fp);
		fread(&size, sizeof(size), 1, fp);

		// fmt�`�����N
		if (memcmp(chunk, CHUNK_FMT, CHUNK_SIZE) == CHUNK_SAME)
		{
			// �`�����N�ƃT�C�Y��ݒ�
			memcpy(wavFile.fmt.fmtChunk, chunk, CHUNK_SIZE);
			wavFile.fmt.fmtSize = size;

			// �t�H�[�}�b�gID����ǂݍ���
			fread(&wavFile.fmt.fmtFormatTag,
				sizeof(FMT_CHUNK) - 8, 1, fp);

			// �t���O����
			chunkFlag += FLAG_CHUNK_FMT;

			continue;
		}

		// data�`�����N
		if (memcmp(chunk, CHUNK_DATA, CHUNK_SIZE) == CHUNK_SAME)
		{
			// �`�����N�ƃT�C�Y��ݒ�
			memcpy(wavFile.data.dataChunk, CHUNK_DATA, CHUNK_SIZE);
			wavFile.data.waveSize = size;

			// �������m��
			wavFile.data.waveData = (short *)malloc(wavFile.data.waveSize);

			// �g�`�̓ǂݍ���
			fread(wavFile.data.waveData, wavFile.data.waveSize, 1, fp);

			// �t���O����
			chunkFlag += FLAG_CHUNK_DATA;

			continue;
		}

		// ���̃`�����N
		else
		{
			// size����skip
			fseek(fp, size, SEEK_CUR);
		}
	}

	/* wav�\���̂̏����� */
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
// �I������
//*****************************************************************************
void UninitLoadSound(void)
{
	free(wavFile.data.waveData);
}

//*****************************************************************************
// WAV�t�@�C���̏����擾
//*****************************************************************************
WAV_FILE *GetWavFile(void)
{
	return &wavFile;
}
