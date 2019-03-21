#ifndef _XAUDIO_PLAYER_OBJECT_H_ 
#define	_XAUDIO_PLAYER_OBJECT_H_

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DEFAULT_VOLUME	(1.0f)

//*****************************************************************************
// �萔��`
//*****************************************************************************
enum PLAY_STATE				// �Đ����
{
	STATE_ERROR,
	STATE_READY_TO_PLAY,
	STATE_PLAYING,
	STATE_END_OR_NOTHING
};

enum TWO_CHANNELS			// 2�`�����l���p
{
	CHANNELS_LEFT,
	CHANNELS_RIGHT
};

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct	// �{�C�X�p
{
	float freq;
	float volume;
	float channels;

}VOICE_TOOL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitXAudio2(void);		// ������
void	UninitXAudio2(void);	// �I������

// ���[�h�T�E���h
IXAudio2SourceVoice *LoadXAudio2Voice(char *path);	

// �Đ�
void PlayXAudio2Voice(IXAudio2SourceVoice *sourceVoice);

// ��~
void StopXAudio2Voice(IXAudio2SourceVoice *sourceVoice);

// �{�����[���̕ύX
void SetVolume(IXAudio2SourceVoice *sourceVoice, 
	int channels, float singleVolume, float mainVolume);

// �Đ��p�T���v�����O���g���̕ύX
void SetSampleFrequency(IXAudio2SourceVoice *sourceVoice, float freq);

// �Đ���Ԃ��擾
XAUDIO2_VOICE_STATE GetXAudio2State(IXAudio2SourceVoice *sourceVoice);
int GetXAudio2StateEnum(IXAudio2SourceVoice *sourceVoice);

// �p���{�����[��
HRESULT SetPanVolume(IXAudio2SourceVoice *voice, float panVolume);

#endif
