//! \example ChromaSDKImpl.cpp

#include "stdafx.h"
#include "ChromaSDKImpl.h"
#include "AsusSDKDefines.h"

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Mousepad;
using namespace ChromaSDK::Headset;

using namespace System;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

INIT Init = NULL;
UNINIT UnInit = NULL;
CREATEEFFECT CreateEffect = NULL;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
CREATEMOUSEEFFECT CreateMouseEffect = NULL;
CREATEHEADSETEFFECT CreateHeadsetEffect = NULL;
CREATEMOUSEPADEFFECT CreateMousepadEffect = NULL;
CREATEKEYPADEFFECT CreateKeypadEffect = NULL;
SETEFFECT SetEffect = NULL;
DELETEEFFECT DeleteEffect = NULL;
QUERYDEVICE QueryDevice = NULL;

CChromaSDKImpl::CChromaSDKImpl() :m_ChromaSDKModule(NULL)
{
}

CChromaSDKImpl::~CChromaSDKImpl()
{
}

BOOL CChromaSDKImpl::Initialize()
{
	if (m_ChromaSDKModule == NULL)
	{
		m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
		if (m_ChromaSDKModule == NULL)
		{
			ASSERT(GetLastError() == ERROR_SUCCESS);
			return FALSE;
		}
	}

	if (Init == NULL)
	{
		RZRESULT Result = RZRESULT_INVALID;
		Init = (INIT)GetProcAddress(m_ChromaSDKModule, "Init");
		if (Init)
		{
			Result = Init();
			if (Result == RZRESULT_SUCCESS)
			{
				CreateEffect = (CREATEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateEffect");
				CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect");
				CreateMouseEffect = (CREATEMOUSEEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMouseEffect");
				CreateHeadsetEffect = (CREATEHEADSETEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateHeadsetEffect");
				CreateMousepadEffect = (CREATEMOUSEPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateMousepadEffect");
				CreateKeypadEffect = (CREATEKEYPADEFFECT)GetProcAddress(m_ChromaSDKModule, "CreateKeypadEffect");
				SetEffect = (SETEFFECT)GetProcAddress(m_ChromaSDKModule, "SetEffect");
				DeleteEffect = (DELETEEFFECT)GetProcAddress(m_ChromaSDKModule, "DeleteEffect");
				QueryDevice = (QUERYDEVICE)GetProcAddress(m_ChromaSDKModule, "QueryDevice");

				if (CreateEffect &&
					CreateKeyboardEffect &&
					CreateMouseEffect &&
					CreateHeadsetEffect &&
					CreateMousepadEffect &&
					CreateKeypadEffect &&
					SetEffect &&
					DeleteEffect &&
					QueryDevice)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CChromaSDKImpl::UnInitialize()
{
	if (m_ChromaSDKModule != NULL)
	{
		RZRESULT Result = RZRESULT_INVALID;
		UNINIT UnInit = (UNINIT)GetProcAddress(m_ChromaSDKModule, "UnInit");
		if (UnInit)
		{
			Result = UnInit();
			ASSERT(Result == RZRESULT_SUCCESS);
		}

		FreeLibrary(m_ChromaSDKModule);
		m_ChromaSDKModule = NULL;

		return TRUE;
	}

	return FALSE;
}

bool CChromaSDKImpl::initASUSGPU()
{
	AsusVGADLL = LoadLibraryW(L"VGA_Extra.dll");
	if (!AsusVGADLL)
		return FALSE;

	LoadVenderDLL = (LoadVenderDLL_t)GetProcAddress(AsusVGADLL, "LoadVenderDLL");
	if (LoadVenderDLL() == -1)
		return FALSE;

	Get_LED_ExternalFan_Info = (Get_LED_ExternalFan_Info_t)GetProcAddress(AsusVGADLL, "Get_LED_ExternalFan_Info");
	Set_LED_ExternalFan_Info = (Set_LED_ExternalFan_Info_t)GetProcAddress(AsusVGADLL, "Set_LED_ExternalFan_Info");
	DetectControlIC = (DetectControlIC_t)GetProcAddress(AsusVGADLL, "DetectControlIC");
	GetOSInfo = (GetOSInfo_t)GetProcAddress(AsusVGADLL, "GetOSInfo");

	return Get_LED_ExternalFan_Info 
		&& Set_LED_ExternalFan_Info 
		&& DetectControlIC 
		&& GetOSInfo;
}

bool CChromaSDKImpl::initASUSMOBO()
{
	AsusMOBODLL = LoadLibraryW(L"D:\\VisualStudio\\RGBService\\LED_DLL_forMB.dll");
	if (!AsusMOBODLL)
		return FALSE;

	/*fnGetInterfaceVersion
	fnEnumSupportDevice
	fnGetLEDConfig*/
	fnSetLEDON = (fnSetLEDON_t)GetProcAddress(AsusMOBODLL, "fnSetLEDON");
	fnSetLEDEFFECT = (fnSetLEDEFFECT_t)GetProcAddress(AsusMOBODLL, "fnSetLEDEFFECT");
	fnGetLEDEFFECT = (fnGetLEDEFFECT_t)GetProcAddress(AsusMOBODLL, "fnGetLEDEFFECT");
	fnSetLEDColor = (fnSetLEDColor_t)GetProcAddress(AsusMOBODLL, "fnSetLEDColor");
	fnGetLEDColor = (fnGetLEDColor_t)GetProcAddress(AsusMOBODLL, "fnGetLEDColor");
	fnEffectReset = (fnEffectReset_t)GetProcAddress(AsusMOBODLL, "fnEffectReset");
	/*fnSetLEDFrame
	fnGetLEDFrame
	fnGetLEDBurn
	fnSetLEDBurn
	fnGetGPUTemperatureColor
	fnGetLastError_VGA_AURA
	fnLoadDefaultSetting*/

	return fnSetLEDON
		&& fnSetLEDEFFECT
		&& fnGetLEDEFFECT
		&& fnSetLEDColor
		&& fnGetLEDColor
		&& fnEffectReset;
}

void SetRazerKeyboardColor(COLORREF Color)
{
	if (CreateKeyboardEffect)
	{
		//This effect will not do transition
		ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Effect = {};
		for (UINT row = 0; row<ChromaSDK::Keyboard::MAX_ROW; row++)
		{
			for (UINT col = 0; col<ChromaSDK::Keyboard::MAX_COLUMN; col++)
			{
				Effect.Color[row][col] = Color;
			}
		}

		RZRESULT Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Effect, NULL);

		//This effect will do transition
		/*ChromaSDK::Keyboard::STATIC_EFFECT_TYPE Effect = {};
		Effect.Color = Color;

		RZRESULT Result = CreateEffect(BLACKWIDOW_CHROMA, ChromaSDK::CHROMA_STATIC, &Effect, NULL);
		//RZRESULT Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &Effect, NULL);
		*/
		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerMousepadColor(COLORREF Color)
{
	if (CreateMousepadEffect)
	{
		//This effect will not transition
		ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE Effect = {};
		for (UINT led = 0; led<ChromaSDK::Mousepad::MAX_LEDS; led++)
		{
			Effect.Color[led] = Color;
		}
		RZRESULT Result = CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM, &Effect, NULL);

		//This effect will transition
		/*ChromaSDK::Mousepad::STATIC_EFFECT_TYPE Effect = {};
		Effect.Color = Color;

		RZRESULT Result = CreateEffect(FIREFLY_CHROMA, ChromaSDK::CHROMA_STATIC, &Effect, NULL);
		//RZRESULT Result = CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_STATIC, &Effect, NULL);
		*/
		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerMouseColor(COLORREF Color)
{
	if (CreateMouseEffect)
	{
		ChromaSDK::Mouse::STATIC_EFFECT_TYPE Effect = {};
		Effect.Color = Color;
		Effect.LEDId = RZLED_ALL;

		RZRESULT Result = CreateMouseEffect(ChromaSDK::Mouse::CHROMA_STATIC, &Effect, NULL);
		//RZRESULT Result = CreateEffect(NAGA_EPIC_CHROMA, ChromaSDK::CHROMA_STATIC, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerHeadsetColor(COLORREF Color)
{
	if (CreateHeadsetEffect)
	{
		ChromaSDK::Headset::STATIC_EFFECT_TYPE Effect = {};
		Effect.Color = Color;

		//RZRESULT Result = CreateMouseEffect(ChromaSDK::Mouse::CHROMA_STATIC, &Effect, NULL);
		RZRESULT Result = CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_STATIC, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerKeypadColor(COLORREF Color)
{
	if (CreateKeypadEffect)
	{
		ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE Effect = {};

		for (UINT i = 0; i < ChromaSDK::Keypad::MAX_ROW; i++)
		{
			for (UINT j = 0; j < ChromaSDK::Keypad::MAX_COLUMN; j++)
			{
				Effect.Color[i][j] = Color;
			}
		}

		RZRESULT Result = CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_CUSTOM, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void CChromaSDKImpl::SetRazerDeviceColor(UINT DeviceType, COLORREF Color)
{
	switch (DeviceType)
	{
		case 1:
			SetRazerKeyboardColor(Color);
			break;
		case 2:
			SetRazerMousepadColor(Color);
			break;
		case 3:
			SetRazerMouseColor(Color);
			break;
		case 4:
			SetRazerHeadsetColor(Color);
			break;
		case 5:
			SetRazerKeypadColor(Color);
			break;
		case 6:
			SetRazerKeyboardColor(Color);
			SetRazerMousepadColor(Color);
			SetRazerMouseColor(Color);
			SetRazerHeadsetColor(Color);
			SetRazerKeypadColor(Color);
			break;
	}
}

void SetRazerKeyboardBreathingEffect(COLORREF Color)
{
	if (CreateKeyboardEffect)
	{
		ChromaSDK::Keyboard::BREATHING_EFFECT_TYPE Effect = {};
		Effect.Type = Effect.TWO_COLORS;
		Effect.Color1 = Color;
		Effect.Color2 = RED;

		RZRESULT Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_BREATHING, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerMousepadBreathingEffect(COLORREF Color)
{
	if (CreateMousepadEffect)
	{
		ChromaSDK::Mousepad::BREATHING_EFFECT_TYPE Effect = {};
		Effect.Type = Effect.TWO_COLORS;
		Effect.Color1 = Color;
		Effect.Color2 = RED;

		RZRESULT Result = CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_BREATHING, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerHeadsetBreathingEffect(COLORREF Color)
{
	if (CreateHeadsetEffect)
	{
		ChromaSDK::Headset::BREATHING_EFFECT_TYPE Effect = {};
		Effect.Color = Color;

		RZRESULT Result = CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_BREATHING, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerKeypadBreathingEffect(COLORREF Color)
{
	if (CreateKeypadEffect)
	{
		ChromaSDK::Keypad::BREATHING_EFFECT_TYPE Effect = {};
		Effect.Type = Effect.TWO_COLORS;
		Effect.Color1 = Color;
		Effect.Color2 = RED;

		RZRESULT Result = CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_BREATHING, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void CChromaSDKImpl::SetRazerDeviceBreathingEffect(UINT DeviceType, COLORREF Color)
{
	switch (DeviceType)
	{
	case 1:
		SetRazerKeyboardBreathingEffect(Color);
		break;
	case 2:
		SetRazerMousepadBreathingEffect(Color);
		break;
	case 4:
		SetRazerHeadsetBreathingEffect(Color);
		break;
	case 5:
		SetRazerKeypadBreathingEffect(Color);
		break;
	case 6:
		SetRazerKeyboardBreathingEffect(Color);
		SetRazerMousepadBreathingEffect(Color);
		SetRazerHeadsetBreathingEffect(Color);
		SetRazerKeypadBreathingEffect(Color);
		break;
	}
}

void SetRazerKeyboardSpectrumEffect()
{
	if (CreateKeyboardEffect)
	{
		RZRESULT Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_SPECTRUMCYCLING, NULL, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerMousepadSpectrumEffect()
{
	if (CreateMousepadEffect)
	{
		RZRESULT Result = CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_SPECTRUMCYCLING, NULL, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerMouseSpectrumEffect()
{
	if (CreateMouseEffect)
	{
		RZRESULT Result = CreateMouseEffect(ChromaSDK::Mouse::CHROMA_SPECTRUMCYCLING, NULL, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerHeadsetSpectrumEffect()
{
	if (CreateHeadsetEffect)
	{
		RZRESULT Result = CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_SPECTRUMCYCLING, NULL, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerKeypadSpectrumEffect()
{
	if (CreateKeypadEffect)
	{
		RZRESULT Result = CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_SPECTRUMCYCLING, NULL, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void CChromaSDKImpl::SetRazerDeviceSpectrumEffect(UINT DeviceType)
{
	switch (DeviceType)
	{
	case 1:
		SetRazerKeyboardSpectrumEffect();
		break;
	case 2:
		SetRazerMousepadSpectrumEffect();
		break;
	case 3:
		SetRazerMouseSpectrumEffect();
		break;
	case 4:
		SetRazerHeadsetSpectrumEffect();
		break;
	case 5:
		SetRazerKeypadSpectrumEffect();
		break;
	case 6:
		SetRazerKeyboardSpectrumEffect();
		SetRazerMousepadSpectrumEffect();
		SetRazerMouseSpectrumEffect();
		SetRazerHeadsetSpectrumEffect();
		SetRazerKeypadSpectrumEffect();
		break;
	}
}

void SetRazerKeyboardReactiveEffect(UINT Duration, COLORREF Color)
{
	if (CreateKeyboardEffect)
	{
		ChromaSDK::Keyboard::REACTIVE_EFFECT_TYPE Effect = {};
		Effect.Color = Color;
		switch (Duration)
		{
		case 0:
			Effect.Duration = Effect.DURATION_SHORT;
			break;
		case 1:
			Effect.Duration = Effect.DURATION_MEDIUM;
			break;
		case 2:
			Effect.Duration = Effect.DURATION_LONG;
			break;
		}

		RZRESULT Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_REACTIVE, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerMouseReactiveEffect(UINT Duration, COLORREF Color)
{
	if (CreateMouseEffect)
	{
		ChromaSDK::Mouse::REACTIVE_EFFECT_TYPE Effect = {};
		Effect.Color = Color;
		switch (Duration)
		{
		case 0:
			Effect.Duration = Effect.DURATION_SHORT;
			break;
		case 1:
			Effect.Duration = Effect.DURATION_MEDIUM;
			break;
		case 2:
			Effect.Duration = Effect.DURATION_LONG;
			break;
		}

		RZRESULT Result = CreateMouseEffect(ChromaSDK::Mouse::CHROMA_REACTIVE, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerKeypadReactiveEffect(UINT Duration, COLORREF Color)
{
	if (CreateKeyboardEffect)
	{
		ChromaSDK::Keypad::REACTIVE_EFFECT_TYPE Effect = {};
		Effect.Color = Color;
		switch (Duration)
		{
		case 0:
			Effect.Duration = Effect.DURATION_SHORT;
			break;
		case 1:
			Effect.Duration = Effect.DURATION_MEDIUM;
			break;
		case 2:
			Effect.Duration = Effect.DURATION_LONG;
			break;
		}

		RZRESULT Result = CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_REACTIVE, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void CChromaSDKImpl::SetRazerDeviceReactiveEffect(UINT DeviceType, UINT Duration, COLORREF Color)
{
	switch (DeviceType)
	{
	case 1:
		SetRazerKeyboardReactiveEffect(Duration, Color);
		break;
	case 3:
		SetRazerMouseReactiveEffect(Duration, Color);
		break;
	case 5:
		SetRazerKeypadReactiveEffect(Duration, Color);
		break;
	case 6:
		SetRazerKeyboardReactiveEffect(Duration, Color);
		SetRazerMouseReactiveEffect(Duration, Color);
		SetRazerKeypadReactiveEffect(Duration, Color);
		break;
	}
}

void SetRazerKeyboardWaveEffect(UINT Direction)
{
	if (CreateKeyboardEffect)
	{
		ChromaSDK::Keyboard::WAVE_EFFECT_TYPE Effect = {};

		switch (Direction) {
		case -1:
			Effect.Direction = Effect.DIRECTION_NONE;
			break;
		case 0:
			Effect.Direction = Effect.DIRECTION_LEFT_TO_RIGHT;
			break;
		case 1:
			Effect.Direction = Effect.DIRECTION_RIGHT_TO_LEFT;
			break;
		default:
			Effect.Direction = Effect.DIRECTION_INVALID;
			break;
		}

		RZRESULT Result = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_WAVE, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerMousepadWaveEffect(UINT Direction)
{
	if (CreateMousepadEffect)
	{
		ChromaSDK::Mousepad::WAVE_EFFECT_TYPE Effect = {};

		switch (Direction) {
		case -1:
			Effect.Direction = Effect.DIRECTION_NONE;
			break;
		case 0:
			Effect.Direction = Effect.DIRECTION_LEFT_TO_RIGHT;
			break;
		case 1:
			Effect.Direction = Effect.DIRECTION_RIGHT_TO_LEFT;
			break;
		default:
			Effect.Direction = Effect.DIRECTION_INVALID;
			break;
		}

		RZRESULT Result = CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_WAVE, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void SetRazerKeypadWaveEffect(UINT Direction)
{
	if (CreateMousepadEffect)
	{
		ChromaSDK::Keypad::WAVE_EFFECT_TYPE Effect = {};

		switch (Direction) {
		case -1:
			Effect.Direction = Effect.DIRECTION_NONE;
			break;
		case 0:
			Effect.Direction = Effect.DIRECTION_LEFT_TO_RIGHT;
			break;
		case 1:
			Effect.Direction = Effect.DIRECTION_RIGHT_TO_LEFT;
			break;
		default:
			Effect.Direction = Effect.DIRECTION_INVALID;
			break;
		}

		RZRESULT Result = CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_WAVE, &Effect, NULL);

		ASSERT(Result == RZRESULT_SUCCESS);
	}
}

void CChromaSDKImpl::SetRazerDeviceWaveEffect(UINT DeviceType, UINT Direction)
{
	switch (DeviceType)
	{
	case 1:
		SetRazerKeyboardWaveEffect(Direction);
		break;
	case 2:
		SetRazerMousepadWaveEffect(Direction);
		break;
	case 4:
		SetRazerKeypadWaveEffect(Direction);
		break;
	default:
		SetRazerKeyboardWaveEffect(Direction);
		SetRazerMousepadWaveEffect(Direction);
		SetRazerKeypadWaveEffect(Direction);
		break;
	}
}

void CChromaSDKImpl::ResetEffects(UINT DeviceType)
{
	switch (DeviceType)
	{
	case 6:
		if (CreateKeyboardEffect)
		{
			CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, NULL, NULL);
		}

		if (CreateMousepadEffect)
		{
			CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, NULL, NULL);
		}

		if (CreateMouseEffect)
		{
			//CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, NULL, NULL);
			// Use custom effect command is faster because some of the mice has fade-to-black effect.
			ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE2 None = {};
			CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM2, &None, NULL);
		}

		if (CreateHeadsetEffect)
		{
			CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, NULL, NULL);
		}

		if (CreateKeypadEffect)
		{
			CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, NULL, NULL);
		}
		break;
	case 1:
		if (CreateKeyboardEffect)
		{
			CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, NULL, NULL);
		}
		break;
	case 2:
		if (CreateMousepadEffect)
		{
			CreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, NULL, NULL);
		}
		break;
	case 3:
		if (CreateMouseEffect)
		{
			//CreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, NULL, NULL);
			// Use custom effect command is faster because some of the mice has fade-to-black effect.
			ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE2 None = {};
			CreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM2, &None, NULL);
		}
		break;
	case 4:
		if (CreateHeadsetEffect)
		{
			CreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, NULL, NULL);
		}
		break;
	case 5:
		if (CreateKeypadEffect)
		{
			CreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, NULL, NULL);
		}
		break;
	}
}

BOOL CChromaSDKImpl::IsDeviceConnected(RZDEVICEID DeviceId)
{
	if (QueryDevice != NULL)
	{
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		RZRESULT Result = QueryDevice(DeviceId, DeviceInfo);

		ASSERT(Result == RZRESULT_SUCCESS);

		return DeviceInfo.Connected;
	}

	return FALSE;
}

struct _ActiveKeysData
{
	UINT numKeys;
	UINT Key[20];
	COLORREF Color;
} ActiveKeysData;

RZKEY getKeyRefByName(char name)
{
	RZKEY key;
	switch (name)
	{
	case '0':
		key = RZKEY_0;
		break;
	case '1':
		key = RZKEY_1;
		break;
	case '2':
		key = RZKEY_2;
		break;
	case '3':
		key = RZKEY_3;
		break;
	case '4':
		key = RZKEY_4;
		break;
	case '5':
		key = RZKEY_5;
		break;
	case '6':
		key = RZKEY_6;
		break;
	case '7':
		key = RZKEY_7;
		break;
	case '8':
		key = RZKEY_8;
		break;
	case '9':
		key = RZKEY_9;
		break;
	default:
		if (name == 'a' || name == 'A')
		{
			key = RZKEY_A;
		}
		else if (name == 'b' || name == 'B')
		{
			key = RZKEY_B;
		}
		else if (name == 'c' || name == 'C')
		{
			key = RZKEY_C;
		}
		else if (name == 'd' || name == 'D')
		{
			key = RZKEY_D;
		}
		else if (name == 'e' || name == 'E')
		{
			key = RZKEY_E;
		}
		else if (name == 'f' || name == 'F')
		{
			key = RZKEY_F;
		}
		else if (name == 'g' || name == 'G')
		{
			key = RZKEY_G;
		}
		else if (name == 'h' || name == 'H')
		{
			key = RZKEY_H;
		}
		else if (name == 'i' || name == 'I')
		{
			key = RZKEY_I;
		}
		else if (name == 'j' || name == 'J')
		{
			key = RZKEY_J;
		}
		else if (name == 'k' || name == 'K')
		{
			key = RZKEY_K;
		}
		else if (name == 'l' || name == 'L')
		{
			key = RZKEY_L;
		}
		else if (name == 'm' || name == 'M')
		{
			key = RZKEY_M;
		}
		else if (name == 'n' || name == 'N')
		{
			key = RZKEY_N;
		}
		else if (name == 'o' || name == 'O')
		{
			key = RZKEY_O;
		}
		else if (name == 'p' || name == 'P')
		{
			key = RZKEY_P;
		}
		else if (name == 'q' || name == 'Q')
		{
			key = RZKEY_Q;
		}
		else if (name == 'r' || name == 'R')
		{
			key = RZKEY_R;
		}
		else if (name == 's' || name == 'S')
		{
			key = RZKEY_S;
		}
		else if (name == 't' || name == 'T')
		{
			key = RZKEY_T;
		}
		else if (name == 'u' || name == 'U')
		{
			key = RZKEY_U;
		}
		else if (name == 'v' || name == 'V')
		{
			key = RZKEY_V;
		}
		else if (name == 'w' || name == 'W')
		{
			key = RZKEY_W;
		}
		else if (name == 'x' || name == 'X')
		{
			key = RZKEY_X;
		}
		else if (name == 'y' || name == 'Y')
		{
			key = RZKEY_Y;
		}
		else if (name == 'z' || name == 'Z')
		{
			key = RZKEY_Z;
		}
		else
		{
			key = RZKEY_ESC;
		}
	}
	return key;
}

DWORD WINAPI ThreadPlayMessage(LPVOID lParam)
{
	Keyboard::CUSTOM_KEY_EFFECT_TYPE CustEffect = {};
	Keyboard::STATIC_EFFECT_TYPE StatEffect = {};

	std::string message = *((std::string*)lParam);
	CString msg(message.c_str());

	OutputDebugString(msg.AllocSysString());
	UINT* RazerKeys = new UINT[msg.GetLength()];


	StatEffect.Color = GREEN;
	ActiveKeysData.Color = GREEN;
	RZRESULT Result;

	// Write the welcome message
	for (int j = 0; j < msg.GetLength(); j++)
	{
		RazerKeys[j] = getKeyRefByName(msg.GetAt(j));
		COLORREF Color = 0x01000000 | ActiveKeysData.Color;
		CustEffect.Key[HIBYTE(RazerKeys[j])][LOBYTE(RazerKeys[j])] = Color;
		CreateKeyboardEffect(Keyboard::CHROMA_CUSTOM_KEY, &CustEffect, NULL);
		Sleep(750);
	}

	delete[] RazerKeys;

	return 0;
}

void CChromaSDKImpl::PlayMessage(std::string message, int displaySpeed) {
	HANDLE hThread = CreateThread(NULL, 0, ThreadPlayMessage, &message, 0, NULL);
	if (WaitForSingleObject(hThread, 5000) == WAIT_OBJECT_0)
	{
		CloseHandle(hThread);
	}
}

void CChromaSDKImpl::SetGPULights(unsigned char r, unsigned char g, unsigned char b)
{
	FanOptions_t Options;
	memset(&Options, 0, sizeof(Options));
	Options.IsRGB = 1;
	Options.R = r;
	Options.G = g;
	Options.B = b;
	Set_LED_ExternalFan_Info(Options);
}

void CChromaSDKImpl::SetMOBOLights(unsigned char r, unsigned char g, unsigned char b)
{
	MoboOptions_t Options;
	memset(&Options, 0, sizeof(Options));
	Options.R = r;
	Options.G = g;
	Options.B = b;
	//fnSetLEDColor(Options);
}
