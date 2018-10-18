#ifndef __LINKERAPI_ENUM_H__
#define __LINKERAPI_ENUM_H__

typedef enum {
	INPORT_B,
	INPORT_A = 2,
} LINKER_INPORT;


typedef enum {
	SLIDESW_MIDDLE = 1,
	SLIDESW_LEFT,
	SLIDESW_RIGHT
} LINKER_SLIDESW;

typedef enum {
	HDCP_14 = 2,
	HDCP_22
} LINKER_HDCP;

typedef enum {
	EDID_AUTOMIX = 1,
	EDID_FIXED,
	EDID_CUSTOM
} LINKER_EDID;


typedef enum {
	AUDIO_HDMI = 1,
	AUDIO_AUTO = 2,
	AUDIO_JACK = 3
} LINKER_AUDIO;

typedef enum {
	SCALE_UP = 0,
	SCALE_THRU = 1,
	SCALE_DN = 2,
} LINKER_SCALE;

typedef enum {
	DEEPC_8,
	DEEPC_10,
	DEEPC_12,
	DEEPC_16
} LINKER_DC;

typedef enum {
	BT2020_FOLLOW,
	BT2020_ON,
	BT2020_OFF
} LINKER_BT2020;

typedef enum {
	FACTRESET_EDIDONLY = 1,
	FACTRESET_NOEDID,
	FACTRESET_ALL
} LINKER_FACTRESET;

typedef enum {
	COLFMT_YUV422_BT601,
	COLFMT_XVYCC422_BT601,
	COLFMT_YUV444_BT601,
	COLFMT_XVYCC444_BT601,
	COLFMT_YUV422_BT709,
	COLFMT_XVYCC422_BT709,
	COLFMT_YUV444_BT709,
	COLFMT_XVYCC444_BT709,
	COLFMT_RGB_FULL_RANGE,

	COLFMT_RESERVED_9,
	COLFMT_RGB_LIMITED_RANGE,
	COLFMT_RESERVED_11,
	COLFMT_RESERVED_12,
	COLFMT_YUV420_BT601,
	COLFMT_RESERVED_14, 
	COLFMT_YUV420_BT709,

	COLFMT_YUV444_BT2020,
	COLFMT_BT2020_RGBYCBCR,
	COLFMT_FOLLOW_INPUT,
	COLFMT_MAX
} LINKER_COLFMT;

typedef enum {  // for GUI selection
	ACT_INPUT = 0,
	AUTO_SWITCH,
	SCALER,
	HDCP_VERSION,
	OUTPUT_420,
	EDID_MODE,
	VIC_MODE,
	FW_VERSION,
	AUDIO2CFORCE,
	AUDIOSPDIFFORCE,
	FORCE3D,
	HDRFORCE,
	AUDIOALLFORCE,
	AUTO_SWITCH_PRIO,
	EDID_CUSTOM_TABLE,
	AVICUSTOM,
	AUTOFILLCRC,
	HDRCUSTOM,
	AUDIODS,
	AVIDISABLE,
	ARCEN,
	SPD_IF,
	ENABLE_LED,
	ENABLE_BLED,
	BT2020,
	FACTRESET,
	YUV,
	CEC,
	CEC_LA,
	AVR,
	HDR_IF,
	HVS_IF,
	EDIDALGO,
	AVIPERS,
	AUTOHDR,
	USER_HDR_IF,
	USER_AVI_IF,
	HDMIVS_IF,
	AUD_IF,
	AVI_IF,
	CSCOUT,
	DEEPC,
	DOWNCSCOUT,
	DSDEEPC,
	USER_3X3,
	MATRIXEN,
	TARGETCS,
	QUICKP,
	BT2020FORCE,
	PRESET4K30,
	PRESET4K60,
	PRESET4K30MODE,
	PRESET4K60MODE,
	MONITORN,
	IGNOREEDID,
	RESETHV1,
	HDRDISABLE,
	HTPCMODE,
	PRESET300
} LINKER_CMD;

enum LINKER_STATUS {
	eINPUT,	
	eScaler,			
	eEDID,			
	eENC,			
	eDOWNS,			
	eCEC_EN,		
	eAUTOSW,	
	eEPPOWER,		
	eMUTE,		
	eSwEdid,
	eSwHdcp,
	eSW6_state,		
	eSWMODE,        
	e2chForce,
	eSpdifForce,
	e3DForce,
	eHdrForce,
	eAudioAllForce,
	eAUTOSWPRIO,
	eEdidCustomTable,
	eAviCustom,		
	eAutoFillCrc,
	eHdrCustom,
	eAudioDs,
	eAviDisable,
	eArc,
	eLedEnable,		
	eBLedEnable,
	eBt2020,
	eYuv,
	ePreset300,
	eAvr,
	eEdidAlgo,
	eAviPers,
	eAutoHdr,
	eCscOut,
	eDeepC,
	eDownCscOut,
	eDsDeepC,
	eMatrixEn,
	eTargetCs,
	eQuickP,
	eBt2020Force,
	ePreset4k30,
	ePreset4k60,
	ePreset4k30Mode,
	ePreset4k60Mode,
	eIgnoreEdid,
	eHdrDisable,
	
};

typedef struct
{
	double A0;
	double A1;
	double A2;
	double A3;
	double B0;
	double B1;
	double B2;
	double B3;
	double C0;
	double C1;
	double C2;
	double C3;
} SB_VALUES;


typedef enum {
	PRESET_4K30_444_8,
	PRESET_4K30_420_8_10_12,
	PRESET_1080P30_444_8_10_12,
	PRESET_4K30_422_12,
	PRESET_4K60_420_8,
	PRESET_1080P60_444_8_10_12
} LINKER_PRESET;

typedef enum {
	eCustom_Uploaded_EDID = 1,
	e4k60_444600MHz_HDR_BT2020_AllSound,
	e4k60_444_600MHz_HDR_BT_2020_Stereo,
	e4k60_444_600MHz_AllSound,
	e4k60_444_600MHz_Stereo,
	e4k50_444_600MHz_HDR_BT2020_AllSound,
	e4k50_444_600MHz_HDR_BT2020_Stereo,
	e4k60_420_12bit_HDR_BT2020_AllSound,
	e4k60_420_12bit_HDR_BT2020_Stereo,
	e4k60_420_12bit_BT2020_AllSound,
	e4k60_420_12bit_BT2020_Stereo,
	e4k60_420_8bit_300MHz_AllSound,
	e4k60_420_8bit_300MHz_Stereo,
	e4k30_444_300MHz_AllSound,
	e4k30_444_300MHz_Stereo,
	e4k30_RGB_300MHz_AllSound,
	e4k30_RGB_300MHz_Stereo,
	e4k24_422_12bit_BT2020_AllSound,
	e4k24_422_12bit_BT2020_Stereo,
	e1080p_444_148MHz_AllSound,
	e1080p_444_148MHz_HDR_Stereo,
	e1080p24_444_AllSound,
	e1080p24_444_Stereo,
	e1080I_444_AllSound,
	e1080I_444_Stereo,
} EDIDTABLE;

typedef struct VIDEO_STATUS
{
	char video_format[120];
	char video_outformat[120];
	char Csc[120];
	char CscO[120];
	char PixelClock[120];
	char OutPixelClock[120];
	char SinkLimited[120];
	char SPDIF[120]; //text
	char VSI[120];
	char HDMIVSI[120];
	char AVI[120];
	char HDR[120];
	char HdrInput[120];
	char SPD[120]; //bytes
	char AUD[120];
	char HdcpIn[120];
	char HdcpOut[120];
	char DcIn[120];
	char DcOut[120];	
	char MonitorName[120];
} LINKER_VIDEOSTATUS; 

#endif /* __LINKERAPI_ENUM_H__ */
