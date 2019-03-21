#ifndef _LOAD_SOUND_H_
#define	_LOAD_SOUND_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �`�����N��T�����[�v�p
#define FLAG_CHUNK_FMT		(1)			// fmt�̃t���O
#define FLAG_CHUNK_DATA		(1<<1)		// data�̃t���O
#define FLAG_CHUNK_END		(3)			// end�|�C���g
// �`�����N
#define CHUNK_FMT			("fmt ")	// fmt�̃`�����N
#define CHUNK_DATA			("data")	// data�̃`�����N
#define CHUNK_SIZE			(4)			// �`�����N�̃o�C�g��
// ����p
#define CHUNK_SAME			(0)			// memcpy�̖߂�l(����)

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct	// RIFF�`�����N 
{
	char	riffChunk[4];
	long	riffSize;
	char	waveChunk[4];
}RIFF_CHUNK;

typedef struct	// fmt �`�����N 
{
	char	fmtChunk[4];
	long	fmtSize;
	short	fmtFormatTag;
	short	fmtChannel;
	long	fmtSampleRate;
	long	fmtAvgBytesPerSec;
	short	fmtBlockAlign;
	short	fmtBitPerSample;
}FMT_CHUNK;

typedef struct	// data�`�����N 
{
	char	dataChunk[4];
	long	waveSize;
	short	*waveData;
}DATA_CHUNK;

typedef struct	// WAV�t�@�C�� 
{
	RIFF_CHUNK	riff;
	FMT_CHUNK	fmt;
	DATA_CHUNK	data;
}WAV_FILE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
WAVEFORMATEX LoadWavFile(char *path);	// WAV�t�@�C���̓ǂݍ���(WAVEFORMATEX�\����)
void UninitLoadSound(void);				// �I������
WAV_FILE *GetWavFile(void);				// WAV�t�@�C���̏����擾
#endif