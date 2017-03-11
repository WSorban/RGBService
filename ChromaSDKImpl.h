
#ifndef _CHROMASDKIMPL_H_
#define _CHROMASDKIMPL_H_

#pragma once

#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"

using namespace System;

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF CYAN = RGB(00, 255, 255);
const COLORREF ORANGE = RGB(255, 165, 00);
const COLORREF PINK = RGB(255, 192, 203);
const COLORREF GREY = RGB(125, 125, 125);

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1
#define MOUSEMAT_DEVICES    2
#define MOUSE_DEVICES       3
#define HEADSET_DEVICES     4
#define KEYPAD_DEVICES      5


class CChromaSDKImpl
{
public:
	CChromaSDKImpl();
	~CChromaSDKImpl();

	BOOL Initialize();
	BOOL UnInitialize();


	void SetRazerDeviceColor(UINT DeviceType, COLORREF Color);
	void SetRazerDeviceBreathingEffect(UINT DeviceType, COLORREF Color);
	void SetRazerDeviceSpectrumEffect(UINT DeviceType);
	void SetRazerDeviceReactiveEffect(UINT DeviceType, UINT Duration, COLORREF Color);
	void SetRazerDeviceWaveEffect(UINT DeviceType, UINT Direction);
	void ResetEffects(UINT DeviceType);
	BOOL IsDeviceConnected(RZDEVICEID DeviceId);
	void DebugStateUsingMousePad(int Result);
	void PlayMessage(String^ message, int displaySpeed);
	void SetGPULights(unsigned char r, unsigned char b, unsigned char g);
	void SetMOBOLights(unsigned char r, unsigned char g, unsigned char b);
	bool initASUSGPU();
	bool initASUSMOBO();

private:
	HMODULE m_ChromaSDKModule;
};

#endif
