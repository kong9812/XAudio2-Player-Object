#ifndef _MAIN_H_
#define	_MAIN_H_

#define _USE_MATH_DEFINES
#include <Windows.h>
#include <winnt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

//==============================================================
// マクロ定義
//==============================================================
// サウンドのパス
#define SOUND_PATH			("data/sound/bomb000.wav")
// 
#ifndef TO_RADIAN
#define TO_RADIAN(d)		{ (d)*(float)M_PI/180.0f }
#endif

#endif