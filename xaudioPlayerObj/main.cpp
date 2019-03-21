#include "main.h"
#include "xaudioPlayerObj.h"

//==============================================================
// �v���g�^�C�v�錾
//==============================================================
void Uninit(void);		// �I������
int Init(void);			// ������

//==============================================================
// �O���[�o���ϐ�
//==============================================================
IXAudio2SourceVoice	*voice = NULL;	// �\�[�X�{�C�X

#if 0	// �{�C�X�c�[���\����
VOICE_TOOL	voiceTool;		// �{�C�X�c�[��
#endif

bool loop = true;			// ���[�v

//==============================================================
// ���C������
//==============================================================
int main(void)
{
	// ���b�Z�[�W
	printf("INIT\n");

	// ������
	if (Init()==1)
	{
		// �G���[!!
		// ���[�v�ɓ���Ȃ��悤��
		loop = false;
	}

	while (loop)
	{	// ���b�Z�[�W
		printf("PLAY\n");

		// �Đ�
		PlayXAudio2Voice(voice);

		float tmpVol = -1.0f;
		while (tmpVol < 1.0f)
		{
			// 
			tmpVol += 0.0000001f;

			// test
			SetPanVolume(voice, tmpVol);
		}

		// �{�^����������i��
		getchar();

		// ���b�Z�[�W
		printf("RIGHT CHANNEL VOLUME:0%%\n");

		// �V���O���`�����l���{�����[����ύX
		SetVolume(voice, CHANNELS_RIGHT, 0.0f, NULL);

		// �{�^����������i��
		getchar();

		// ���b�Z�[�W
		printf("BOTH CHANNEL VOLUME:100%%\n");

		// �S���̃`�����l���{�����[����ύX
		SetVolume(voice, NULL, NULL, 1.0f);

		// �{�^����������i��
		getchar();

		// ���b�Z�[�W
		printf("NORMAL FREQUENCY * 20\n");

		// �T���v�����O���g����ύX
		SetSampleFrequency(voice, 20);

		// �{�^����������i��
		getchar();

		// ���b�Z�[�W
		printf("STOP\n");

		// ��~
		StopXAudio2Voice(voice);

		// �{�^����������i��
		getchar();

		// ���[�v��������
		loop = false;
	}

	// �I������
	Uninit();

	return 0;
}

//==============================================================
// ������
//==============================================================
int Init(void)
{
	// ������
	if (FAILED(InitXAudio2()))
	{
		// �I������
		Uninit();

		return 1;
	}

	// �ǂݍ���
	voice = LoadXAudio2Voice((char *)SOUND_PATH);
	if (voice == NULL)
	{
		// �I������
		Uninit();

		return 1;
	}

	return 0;
}

//==============================================================
// �I������
//==============================================================
void Uninit(void)
{
	UninitXAudio2();
}

