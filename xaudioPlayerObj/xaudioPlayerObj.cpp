#include "main.h"
#include "loadSound.h"
#include "xaudioPlayerObj.h"

//==============================================================
// �O���[�o���ϐ�
//==============================================================
IXAudio2MasteringVoice	*masteringVoice = NULL;	// �}�X�^�[�{�C�X
IXAudio2				*xAudio2 = NULL;		// XAudio2�̃C���^�[�t�F�C�X
float					*channelVolume = NULL;	// �{�����[��

//==============================================================
// ������
//==============================================================
HRESULT InitXAudio2(void)
{
	// COM���C�u�����̏�����
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return E_FAIL;
	}

	// XAudio2�I�u�W�F�N�g�̍쐬
	if (FAILED(XAudio2Create(&xAudio2)))
	{
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̍쐬
	if (FAILED(xAudio2->CreateMasteringVoice(&masteringVoice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==============================================================
// ���[�h�T�E���h
//==============================================================
IXAudio2SourceVoice *LoadXAudio2Voice(char *path)
{
	IXAudio2SourceVoice *sourceVoice = NULL;	// �\�[�X�{�C�X
	WAVEFORMATEX		pcm;					// WAVEFORMATEX�\����
	XAUDIO2_BUFFER		buffer;					// �o�b�t�@

	{// ���[�J���ϐ��̏�����

		// �\���̂̏�����
		memset(&pcm, 0, sizeof(WAVEFORMATEX));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// wav�t�@�C���̓ǂݍ���(WAVEFORMATEX�\����)
		pcm = LoadWavFile(path);

		// �o�b�t�@�̐ݒ�
		buffer.pAudioData = (BYTE*)GetWavFile()->data.waveData;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes = GetWavFile()->data.waveSize;
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// �\�[�X�{�C�X�̍쐬
	if (FAILED(xAudio2->CreateSourceVoice(&sourceVoice, &pcm, NULL, XAUDIO2_MAX_FREQ_RATIO)))
	{
		return NULL;
	}

	// �o�b�t�@���o
	if (FAILED(sourceVoice->SubmitSourceBuffer(&buffer)))
	{
		return NULL;
	}

	{// �{�����[���̏�����
		// �������m��[�`�����l����]
		channelVolume = (float *)malloc(GetWavFile()->fmt.fmtChannel * sizeof(float));
		for (int i = 0; i < GetWavFile()->fmt.fmtChannel; i++)
		{
			channelVolume[i] = DEFAULT_VOLUME;
		}
	}

	return sourceVoice;
}

//==============================================================
// �I������
//==============================================================
void UninitXAudio2(void)
{
	// COM���C�u�����̏I������
	CoUninitialize();

	// �}�X�^�[�{�C�X
	if (masteringVoice != NULL)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = NULL;
	}

	// �C���^�[�t�F�[�X
	if (xAudio2 != NULL)
	{
		xAudio2->Release();
		xAudio2 = NULL;
	}

	// LoadSound�̏I������
	UninitLoadSound();

	// �{�����[���̃��������
	free(channelVolume);
}

//==============================================================
// �Đ�
//==============================================================
void PlayXAudio2Voice(IXAudio2SourceVoice *sourceVoice)
{
	if (sourceVoice != NULL)
	{
		// �Đ��\�H
		if (GetXAudio2StateEnum(sourceVoice) == STATE_READY_TO_PLAY)
		{
			// �Đ�
			sourceVoice->Start(0);
		}
	}
}

//==============================================================
// ��~
//==============================================================
void StopXAudio2Voice(IXAudio2SourceVoice *sourceVoice)
{
	if (sourceVoice != NULL)
	{
		// �Đ����H
		if (GetXAudio2StateEnum(sourceVoice) == STATE_PLAYING)
		{
			// ��~
			sourceVoice->Stop(0);
		}
	}
}

//==============================================================
// �Đ���Ԃ��擾
// �߂�l:XAUDIO2_VOICE_STATE state
//==============================================================
XAUDIO2_VOICE_STATE GetXAudio2State(IXAudio2SourceVoice *sourceVoice)
{
	XAUDIO2_VOICE_STATE state;

	if (sourceVoice != NULL)
	{
		// �\���̂̏�����
		memset(&state, 0, sizeof(XAUDIO2_VOICE_STATE));

		// �Đ���Ԃ��擾
		sourceVoice->GetState(&state);
	}

	return state;
}

//==============================================================
// �{�����[���̕ύX
//==============================================================
void SetVolume(IXAudio2SourceVoice *sourceVoice, int channels, float singleVolume, float mainVolume)
{	
	if (sourceVoice != NULL)
	{
		if (mainVolume == NULL)
		{
			// 1�`�����l��
			channelVolume[channels] = singleVolume;

			// �Đ��p�T���v�����O���g��[�V���O���`�����l��]
			sourceVoice->SetChannelVolumes((UINT32)GetWavFile()->fmt.fmtChannel, channelVolume);
		}

		else
		{
			// �S���̃`�����l��
			for (int i = 0; i < GetWavFile()->fmt.fmtChannel; i++)
			{
				channelVolume[i] = mainVolume;
			}

			// �Đ��p�T���v�����O���g��[�V���O���`�����l��]
			sourceVoice->SetChannelVolumes((UINT32)GetWavFile()->fmt.fmtChannel, channelVolume);

		}
	}

}

//==============================================================
// �Đ��p�T���v�����O���g���̕ύX
//==============================================================
void SetSampleFrequency(IXAudio2SourceVoice *sourceVoice, float freq)
{
	if (sourceVoice != NULL)
	{
		// �Đ��p�T���v�����O���g��
		sourceVoice->SetFrequencyRatio(freq);
	}
}

//==============================================================
// �Đ���Ԃ��擾
// �߂�l:int state[enum]
//==============================================================
int GetXAudio2StateEnum(IXAudio2SourceVoice *sourceVoice)
{
	XAUDIO2_VOICE_STATE state;
	int stateEnum = NULL;

	if (sourceVoice != NULL)
	{
		// �Đ���Ԃ��擾
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
// �p���{�����[��
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