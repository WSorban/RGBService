//VGA functions
struct FanOptions_t
{
	char _0x0000[0xC];
	BOOL IsRGB;
	int R;
	int G;
	int B;
	struct
	{
		BOOL UseFlag;
		enum
		{
			BREATHING = 2,
			STROBING = 3,
			SPECTRUM = 4
		} FlagType;
	} Flags;
	char _0x0020[0x40];
};//size 0x64

HMODULE AsusVGADLL = NULL;

typedef int(* LoadVenderDLL_t)();
typedef void(* Get_LED_ExternalFan_Info_t)();
typedef void(* Set_LED_ExternalFan_Info_t)(FanOptions_t Options);
typedef int(* DetectControlIC_t)(DWORD NUm);
typedef void(* GetOSInfo_t)();

LoadVenderDLL_t LoadVenderDLL = NULL;
DetectControlIC_t DetectControlIC = NULL;
GetOSInfo_t GetOSInfo = NULL;
Get_LED_ExternalFan_Info_t Get_LED_ExternalFan_Info = NULL;
Set_LED_ExternalFan_Info_t Set_LED_ExternalFan_Info = NULL;

//MOBO functions
struct MoboOptions_t
{
	int a1 = 0; //not used in the function itself
	int a2 = 0; //used in the comparison to return 0
	int a3 = 0; //used in the comparison to return 0
	int a4 = 0; //used as an argument in an unknown function, which will return the result of the main function
	int a5 = 0; //not used in the function itself
	int a6 = 0; //not used in the function itself
	int a7 = 0; //not used in the function itself
	int a8 = 0; //not used in the function itself
	int a9 = 0; //not used in the function itself
	int a10 = 0; //not used in the function itself
	int R; //a11, possibly R
	int G; //a12, possibly G
	int B; //a13, possibly B
};

struct LEDConfigOptions_t
{
	int a1;
	int a2;
	int a3;
	int a4;
	int a5;
	int a6;
	int a7;
	int a8;
	int a9;
	int a10;
	int a11;
};

HMODULE AsusMOBODLL = NULL;

typedef int(*fnGetInterfaceVersion_t)();
typedef int(*fnEnumSupportDevice_t)(int a1);
typedef int(*fnGetLEDConfig_t)(LEDConfigOptions_t Options);
typedef char(*fnSetLEDON_t)();
typedef char(*fnSetLEDEFFECT_t)();
typedef char(*fnGetLEDEFFECT_t)();
typedef char(*fnSetLEDColor_t)(MoboOptions_t Options);
typedef char(*fnGetLEDColor_t)();
typedef char(*fnEffectReset_t)();
typedef char(*fnSetLEDFrame_t)(); //fnSetLEDFrame(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, char a11)
typedef char(*fnGetLEDFrame_t)(); //fnGetLEDFrame(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, _DWORD *a11)

fnSetLEDON_t fnSetLEDON = NULL;
fnSetLEDEFFECT_t fnSetLEDEFFECT = NULL;
fnGetLEDEFFECT_t fnGetLEDEFFECT = NULL;
fnSetLEDColor_t fnSetLEDColor = NULL;
fnGetLEDColor_t fnGetLEDColor = NULL;
fnEffectReset_t fnEffectReset = NULL;

