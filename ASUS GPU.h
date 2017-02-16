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

HMODULE AsusDLL = NULL;

