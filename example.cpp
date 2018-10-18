#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "linker.h"
#include "linkerapi_enum.h"

struct vic_mode
{
	const char* name;
	uint32_t width;
	uint32_t height;
	uint32_t vrefresh;
	uint32_t flags;
};

static const vic_mode vic_modes[] =
{
	{ "(na)", 0, 0, 0, 0 },
	{ "640x480", 640, 480, 60, 0 },
	{ "720x480", 720, 480, 60, 0 },
	{ "720x480", 720, 480, 60, 0 },
	{ "1280x720", 1280, 720, 60, 0 },
	{ "1920x1080i", 1920, 1080, 60, 0 },
	{ "720x480i", 720, 480, 60, 0 },
	{ "720x480i", 720, 480, 60, 0 },
	{ "720x240", 720, 240, 60, 0 },
	{ "720x240", 720, 240, 60, 0 },
	{ "2880x480i", 2880, 480, 60, 0 },
	{ "2880x480i", 2880, 480, 60, 0 },
	{ "2880x240", 2880, 240, 60, 0 },
	{ "2880x240", 2880, 240, 60, 0 },
	{ "1440x480", 1440, 480, 60, 0 },
	{ "1440x480", 1440, 480, 60, 0 },
	{ "1920x1080", 1920, 1080, 60, 0 },
	{ "720x576", 720, 576, 50, 0 },
	{ "720x576", 720, 576, 50, 0 },
	{ "1280x720", 1280, 720, 50, 0 },
	{ "1920x1080i", 1920, 1080, 50, 0 },
	{ "720x576i", 720, 576, 50, 0 },
	{ "720x576i", 720, 576, 50, 0 },
	{ "720x288", 720, 288, 50, 0 },
	{ "720x288", 720, 288, 50, 0 },
	{ "2880x576i", 2880, 576, 50, 0 },
	{ "2880x576i", 2880, 576, 50, 0 },
	{ "2880x288", 2880, 288, 50, 0 },
	{ "2880x288", 2880, 288, 50, 0 },
	{ "1440x576", 1440, 576, 50, 0 },
	{ "1440x576", 1440, 576, 50, 0 },
	{ "1920x1080", 1920, 1080, 50, 0 },
	{ "1920x1080", 1920, 1080, 24, 0 },
	{ "1920x1080", 1920, 1080, 25, 0 },
	{ "1920x1080", 1920, 1080, 30, 0 },
	{ "2880x480", 2880, 480, 60, 0 },
	{ "2880x480", 2880, 480, 60, 0 },
	{ "2880x576", 2880, 576, 50, 0 },
	{ "2880x576", 2880, 576, 50, 0 },
	{ "1920x1080i", 1920, 1080, 50, 0 },
	{ "1920x1080i", 1920, 1080, 100, 0 },
	{ "1280x720", 1280, 720, 100, 0 },
	{ "720x576", 720, 576, 100, 0 },
	{ "720x576", 720, 576, 100, 0 },
	{ "720x576i", 720, 576, 100, 0 },
	{ "720x576i", 720, 576, 100, 0 },
	{ "1920x1080i", 1920, 1080, 120, 0 },
	{ "1280x720", 1280, 720, 120, 0 },
	{ "720x480", 720, 480, 120, 0 },
	{ "720x480", 720, 480, 120, 0 },
	{ "720x480i", 720, 480, 120, 0 },
	{ "720x480i", 720, 480, 120, 0 },
	{ "720x576", 720, 576, 200, 0 },
	{ "720x576", 720, 576, 200, 0 },
	{ "720x576i", 720, 576, 200, 0 },
	{ "720x576i", 720, 576, 200, 0 },
	{ "720x480", 720, 480, 240, 0 },
	{ "720x480", 720, 480, 240, 0 },
	{ "720x480i", 720, 480, 240, 0 },
	{ "720x480i", 720, 480, 240, 0 },
	{ "1280x720", 1280, 720, 24, 0 },
	{ "1280x720", 1280, 720, 25, 0 },
	{ "1280x720", 1280, 720, 30, 0 },
	{ "1920x1080", 1920, 1080, 120, 0 },
	{ "1920x1080", 1920, 1080, 100, 0 },
	{ "1280x720", 1280, 720, 24, 0 },
	{ "1280x720", 1280, 720, 25, 0 },
	{ "1280x720", 1280, 720, 30, 0 },
	{ "1280x720", 1280, 720, 50, 0 },
	{ "1280x720", 1280, 720, 60, 0 },
	{ "1280x720", 1280, 720, 100, 0 },
	{ "1280x720", 1280, 720, 120, 0 },
	{ "1920x1080", 1920, 1080, 24, 0 },
	{ "1920x1080", 1920, 1080, 25, 0 },
	{ "1920x1080", 1920, 1080, 30, 0 },
	{ "1920x1080", 1920, 1080, 50, 0 },
	{ "1920x1080", 1920, 1080, 60, 0 },
	{ "1920x1080", 1920, 1080, 100, 0 },
	{ "1920x1080", 1920, 1080, 120, 0 },
	{ "1680x720", 1680, 720, 24, 0 },
	{ "1680x720", 1680, 720, 25, 0 },
	{ "1680x720", 1680, 720, 30, 0 },
	{ "1680x720", 1680, 720, 50, 0 },
	{ "1680x720", 1680, 720, 60, 0 },
	{ "1680x720", 1680, 720, 100, 0 },
	{ "1680x720", 1680, 720, 120, 0 },
	{ "2560x1080", 2560, 1080, 24, 0 },
	{ "2560x1080", 2560, 1080, 25, 0 },
	{ "2560x1080", 2560, 1080, 30, 0 },
	{ "2560x1080", 2560, 1080, 50, 0 },
	{ "2560x1080", 2560, 1080, 60, 0 },
	{ "2560x1080", 2560, 1080, 100, 0 },
	{ "2560x1080", 2560, 1080, 120, 0 },
	{ "3840x2160", 3840, 2160, 24, 0 },
	{ "3840x2160", 3840, 2160, 25, 0 },
	{ "3840x2160", 3840, 2160, 30, 0 },
	{ "3840x2160", 3840, 2160, 50, 0 },
	{ "3840x2160", 3840, 2160, 60, 0 },
	{ "4096x2160", 4096, 2160, 24, 0 },
	{ "4096x2160", 4096, 2160, 25, 0 },
	{ "4096x2160", 4096, 2160, 30, 0 },
	{ "4096x2160", 4096, 2160, 50, 0 },
	{ "4096x2160", 4096, 2160, 60, 0 },
	{ "3840x2160", 3840, 2160, 24, 0 },
	{ "3840x2160", 3840, 2160, 25, 0 },
	{ "3840x2160", 3840, 2160, 30, 0 },
	{ "3840x2160", 3840, 2160, 50, 0 },
	{ "3840x2160", 3840, 2160, 60, 0 },
};


static const char* str_linker_status[] =
{
	"eINPUT",	
	"eScaler",			
	"eEDID",			
	"eENC",			
	"eDOWNS",			
	"eCEC_EN",		
	"eAUTOSW",	
	"eEPPOWER",		
	"eMUTE",		
	"eSwEdid",
	"eSwHdcp",
	"eSW6_state",		
	"eSWMODE",        
	"e2chForce",
	"eSpdifForce",
	"e3DForce",
	"eHdrForce",
	"eAudioAllForce",
	"eAUTOSWPRIO",
	"eEdidCustomTable",
	"eAviCustom",		
	"eAutoFillCrc",
	"eHdrCustom",
	"eAudioDs",
	"eAviDisable",
	"eArc",
	"eLedEnable",		
	"eBLedEnable",
	"eBt2020",
	"eYuv",
	"ePreset300",
	"eAvr",
	"eEdidAlgo",
	"eAviPers",
	"eAutoHdr",
	"eCscOut",
	"eDeepC",
	"eDownCscOut",
	"eDsDeepC",
	"eMatrixEn",
	"eTargetCs",
	"eQuickP",
	"eBt2020Force",
	"ePreset4k30",
	"ePreset4k60",
	"ePreset4k30Mode",
	"ePreset4k60Mode",
	"eIgnoreEdid",
	"eHdrDisable",
};

int main(int argc, char** args)
{
	LinkerHandle h = LinkerOpen();

	if (h)
	{
		uint8_t data[256];

		char fw[32];
		if (LinkerGetFwVersion(h, fw))
			printf("Linker firmware version: %s\n", fw);

		printf("Linker state vector:\n");
		memset(data, 0, sizeof(data));
		size_t count = LinkerGetStateVector(h, data, sizeof(data));
		if (count > 0)
		{
			for (int i = 0; i <= eHdrDisable; i++)
			{
				printf("%s = %d\n", str_linker_status[i], data[i]);
			}
		}

		printf("AVI IF: ");
		count = LinkerGetAVI_IF(h, data, sizeof(data));
		for (size_t i = 0; i < count; i++)
		{
			printf("%02x ", data[i]);
		}
		printf("\n");
		// parse AVI IF based on HDMI spec
		// useful enums found here: https://github.com/torvalds/linux/blob/master/include/linux/hdmi.h
		printf("   Colorspace     : %d\n", (data[6] >> 5) & 3);
		printf("   Colorimetry    : %d\n", (data[7] >> 6) & 3);
		printf("   ExtColorimetry : %d\n", (data[8] >> 4) & 7);
		printf("   VIC            : %d [%s]\n", data[9], vic_modes[data[9]].name);
		printf("HDR IF: ");
		count = LinkerGetHDR_IF(h, data, sizeof(data));
		for (size_t i = 0; i < count; i++)
		{
			printf("%02x ", data[i]);
		}
		printf("\n");

#if 0
		// set input to port B
		LinkerSetCommand(h, ACT_INPUT, INPORT_B);

		// set HDCP version to 2.2
		LinkerSetCommand(h, HDCP_VERSION, HDCP_22);

		// disable HDR
		LinkerSetCommand(h, HDRDISABLE, 1);
#endif
	}
	else
		printf("failed to open linker device\n");

	LinkerClose(h);
}

