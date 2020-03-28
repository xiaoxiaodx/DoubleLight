#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <sys/time.h>
#include "hi_type.h"
#include "hi_avi.h"
#include "defs.h"

//////////////////////////////////////////////////////////////////////////
// 内部使用的宏定义
#define MAX_AVI_STREAM_NUM  4
#define AVI_HEADER_LENGTH       512
#define AVI_FIRST_FRAME_OFFSET  4

#define  FFMT_ENDIAN32( x )		(x)
#   define TRANSFORM_FOURCC( a, b, c, d ) \
	( ((HI_U32)a) | ( ((HI_U32)b) << 8 ) \
| ( ((HI_U32)c) << 16 ) | ( ((HI_U32)d) << 24 ) )
#   define TRANSFORM_TWOCC( a, b ) \
( (HI_U16)(a) | ( (HI_U16)(b) << 8 ) )

#define AVI_FOURCC_RIFF         TRANSFORM_FOURCC('R','I','F','F')
#define AVI_FOURCC_LIST         TRANSFORM_FOURCC('L','I','S','T')
#define AVI_FOURCC_JUNK         TRANSFORM_FOURCC('J','U','N','K')
#define AVI_FOURCC_AVI          TRANSFORM_FOURCC('A','V','I',' ')
#define AVI_FOURCC_AVIX         TRANSFORM_FOURCC('A','V','I','X')
#define AVI_FOURCC_WAVE         TRANSFORM_FOURCC('W','A','V','E')
#define AVI_FOURCC_INFO         TRANSFORM_FOURCC('I','N','F','O')

#define AVI_FOURCC_AVIH         TRANSFORM_FOURCC('a','v','i','h')
#define AVI_FOURCC_HDRL         TRANSFORM_FOURCC('h','d','r','l')
#define AVI_FOURCC_MOVI         TRANSFORM_FOURCC('m','o','v','i')
#define AVI_FOURCC_IDX1         TRANSFORM_FOURCC('i','d','x','1')

#define AVI_FOURCC_STRL         TRANSFORM_FOURCC('s','t','r','l')
#define AVI_FOURCC_STRH         TRANSFORM_FOURCC('s','t','r','h')
#define AVI_FOURCC_STRF         TRANSFORM_FOURCC('s','t','r','f')
#define AVI_FOURCC_STRD         TRANSFORM_FOURCC('s','t','r','d')
#define AVI_FOURCC_STRN         TRANSFORM_FOURCC('s','t','r','n')
#define AVI_FOURCC_INDX         TRANSFORM_FOURCC('i','n','d','x')

#define AVI_FOURCC_REC          TRANSFORM_FOURCC('r','e','c',' ')
#define AVI_FOURCC_AUDS         TRANSFORM_FOURCC('a','u','d','s')
#define AVI_FOURCC_VIDS         TRANSFORM_FOURCC('v','i','d','s')
#define AVI_FOURCC_TXTS         TRANSFORM_FOURCC('t','x','t','s')
#define AVI_FOURCC_MIDS         TRANSFORM_FOURCC('m','i','d','s')

#define AVI_FOURCC_00dc         TRANSFORM_FOURCC('0','0','d','c') 
#define AVI_FOURCC_01wb         TRANSFORM_FOURCC('0','1','w','b') 

/************************************************************************/
/*       编解码格式 - 视频                                              */
/************************************************************************/
#define AVI_FOURCC_H264         TRANSFORM_FOURCC('H','2','6','4')
#define AVI_FOURCC_h264         TRANSFORM_FOURCC('h','2','6','4')
#define AVI_FOURCC_X264         TRANSFORM_FOURCC('X','2','6','4')
#define AVI_FOURCC_x264         TRANSFORM_FOURCC('x','2','6','4')
#define AVI_FOURCC_VSSH         TRANSFORM_FOURCC('V','S','S','H')
#define AVI_FOURCC_vssh         TRANSFORM_FOURCC('v','s','s','h')
#define AVI_FOURCC_VSS1         TRANSFORM_FOURCC('V','S','S','1')
#define AVI_FOURCC_NJVC         TRANSFORM_FOURCC('N','J','V','C')
#define AVI_FOURCC_njvc         TRANSFORM_FOURCC('n','j','v','c')
#define AVI_FOURCC_avc1         TRANSFORM_FOURCC('a','v','c','1')

#define AVI_FOURCC_AVS          TRANSFORM_FOURCC('A','V','S',' ')
#define AVI_FOURCC_VC1          TRANSFORM_FOURCC('V','C','1',' ')
#define AVI_FOURCC_I420         TRANSFORM_FOURCC('I','4','2','0')
#define AVI_FOURCC_UYVY         TRANSFORM_FOURCC('U','Y','V','Y')
#define AVI_FOURCC_MPEG         TRANSFORM_FOURCC('M','P','E','G')
#define AVI_FOURCC_MPG2         TRANSFORM_FOURCC('M','P','G','2')
#define AVI_FOURCC_MPG4         TRANSFORM_FOURCC('M','P','G','4')
#define AVI_FOURCC_div3         TRANSFORM_FOURCC('d','i','v','3')
#define AVI_FOURCC_DIV3         TRANSFORM_FOURCC('D','I','V','3')
#define AVI_FOURCC_div4         TRANSFORM_FOURCC('d','i','v','4')
#define AVI_FOURCC_DIV4         TRANSFORM_FOURCC('D','I','V','4')
#define AVI_FOURCC_divx         TRANSFORM_FOURCC('d','i','v','x')
#define AVI_FOURCC_DIVX         TRANSFORM_FOURCC('D','I','V','X')
#define AVI_FOURCC_dx50         TRANSFORM_FOURCC('d','x','5','0')
#define AVI_FOURCC_DX50         TRANSFORM_FOURCC('D','X','5','0')
#define AVI_FOURCC_xvid         TRANSFORM_FOURCC('x','v','i','d')
#define AVI_FOURCC_XVID         TRANSFORM_FOURCC('X','V','I','D')
#define AVI_FOURCC_mp4s         TRANSFORM_FOURCC('m','p','4','s')
#define AVI_FOURCC_MP4S         TRANSFORM_FOURCC('M','P','4','S')
#define AVI_FOURCC_m4s2         TRANSFORM_FOURCC('m','4','s','2')
#define AVI_FOURCC_M4S2         TRANSFORM_FOURCC('M','4','S','2')

#define AVI_FOURCC_MJPG         TRANSFORM_FOURCC('M','J','P','G')
#define AVI_FOURCC_mjpg         TRANSFORM_FOURCC('m','j','p','g')
#define AVI_FOURCC_JPEG         TRANSFORM_FOURCC('J','P','E','G')
#define AVI_FOURCC_jpeg         TRANSFORM_FOURCC('j','p','e','g')
#define AVI_FOURCC_LJPG         TRANSFORM_FOURCC('L','J','P','G')
#define AVI_FOURCC_JPGL         TRANSFORM_FOURCC('J','P','G','L')
#define AVI_FOURCC_MJLS         TRANSFORM_FOURCC('M','J','L','S')
#define AVI_FOURCC_IJPG         TRANSFORM_FOURCC('I','J','P','G')
#define AVI_FOURCC_AVRn         TRANSFORM_FOURCC('A','V','R','n')



const int codec_wav_tags[][2] = 
{
	{ AVI_AUDIO_PCM,   0x01 },
	{ AVI_AUDIO_ADPCM, 0x02 },
	{ AVI_AUDIO_G711A, 0x06 },
	{ AVI_AUDIO_G711U, 0x07 },
	{ AVI_AUDIO_G726,  0x45 },
	{ AVI_AUDIO_G726,  0x64 },
	{ AVI_AUDIO_G726,  0x85 },
	{ AVI_AUDIO_G726,  0x140 },
	{ AVI_AUDIO_AMR,  0x7a21 },
	{ AVI_AUDIO_AMR,  0x7a22 },

};

#pragma pack(2)
// 内部使用的结构体定义
typedef struct 
{
    HI_U32  dwMicroSecPerFrame;
    HI_U32  dwMaxBytesPerSec;
    HI_U32  dwReserved1;
    HI_U32  dwFlags;
    HI_U32  dwTotalFrames;
    HI_U32  dwInitialFrames;
    HI_U32  dwStreams;
    HI_U32  dwSuggestedBufferSize;
    HI_U32  dwWidth;
    HI_U32  dwHeight;
    HI_U32  dwScale;
    HI_U32  dwRate;
    HI_U32  dwStart;
    HI_U32  dwLength;
}AVI_AVIH_S;
#pragma pack()

#pragma pack (1) 
typedef struct
{
    HI_U32   fccType;
    HI_U32   fccHandler;
    HI_U32   dwFlags;
    HI_U16   Priority;//低16位有效
    HI_U16   Language;//低16位有效
    HI_U32   dwInitialFrames;
    HI_U32   dwScale;
    HI_U32   dwRate;
    HI_U32   dwStart;
    HI_U32   dwLength;
    HI_U32   dwSuggestedBufferSize;
    HI_U32   dwQuality;
    HI_U32   dwSampleSize;
    HI_U16   FrameLeft;//低16位有效
    HI_U16   FrameTop;//低16位有效
    HI_U16   FrameRight;//低16位有效
    HI_U16   FrameBottom;//低16位有效
}AVI_STRH_S;

typedef struct AVI_BITMAPINFOHEADER_S
{
    HI_U32 biSize; 
    HI_S32 biWidth; 
    HI_S32 biHeight; 
    HI_U16 biPlanes; //读数据时只用读16位
    HI_U16 biBitCount;//读数据时只用读16位
    HI_U32 biCompression; 
    HI_U32 biSizeImage; 
    HI_S32 biXPelsPerMeter; 
    HI_S32 biYPelsPerMeter; 
    HI_U32 biClrUsed; 
    HI_U32 biClrImportant; 
}AVI_BITMAPINFOHEADER_S;

typedef struct AVI_WAVEFORMATEX_S
{
    HI_U16 wFormatTag;         /* format type *///读数据时只用读16位
    HI_U16 nChannels;          /* number of channels (i.e. mono, stereo...) *///读数据时只用读16位
    HI_U32 nSamplesPerSec;     /* sample rate */
    HI_U32 nAvgBytesPerSec;    /* for buffer estimation */
    HI_U16 nBlockAlign;        /* block size of data *///读数据时只用读16位
    HI_U16 wBitsPerSample;     /* number of bits per sample of mono data *///读数据时只用读16位
    HI_U16 cbSize;             /* the count in bytes of the size of extra information (after cbSize) *///读数据时只用读16位
}AVI_WAVEFORMATEX_S;


typedef struct
{
	AVI_STRH_S strh;
	union 
	{
		AVI_BITMAPINFOHEADER_S  bmp;
		AVI_WAVEFORMATEX_S      wave;
	};
	
} AVI_STRL_S;

typedef struct AVI_IDX1_S
{
    HI_U32 ckid;
    HI_U32 dwFlags;
    HI_U32 dwChunkOffset;
    HI_U32 dwChunkLength;
} AVI_INDEXENTRY_S;

#pragma pack()

typedef struct  hiavi_info_s
{
	HI_U32 m_FileLen; 

	// 下面3个用于seek和读帧
	HI_U32 m_DataOffet;
	HI_U32 m_IndexOffset;
	HI_BOOL m_bSeekAble;
    
	HI_U32 m_currFrame;   // 帧计数
	HI_U32 m_FirstIndex;  // 第一帧的偏移量, 

	HI_U8 *m_pFrameBuffer;
	HI_U32 m_pFrameBufferSize;


	HI_U64 currAudioPTS;
	HI_U64 currVideoPTS;
	HI_U64 deltaAudioPTS;
    HI_U64 deltaVideoPTS;
	HI_U64 Duration;

	HI_U32 VideoFrameNum;
	HI_U32 AudioFrameNum;
	HI_U32 TotalFrameNum;
    HI_U32 u32AudioToltalSize;

	FILE *pFile, *pFileTmp;
	AVI_AVIH_S avih;
	AVI_STRL_S strl[MAX_AVI_STREAM_NUM];

	HI_AVI_VIDEO_S Video;
	HI_AVI_AUDIO_S Audio;
	HI_AVI_FILE_S FileInfo;


    HI_U32 HasVideo;
	HI_U32 HasAudio;
} HI_AVI_INFO_S;
/***********************************************************************************************************/

unsigned int HI_GetSystemTime(void)
{
   struct timeval tv;	
   gettimeofday(&tv,0);
   
   return (unsigned int)(tv.tv_sec%10000000*1000+tv.tv_usec/1000);   
}

static void Parse_Header_StreamData(HI_U32 FourCC, HI_U32 *pStreamNum,AVI_CHUNKID_E  *pStreamType)
{
	
    HI_U8 *pChar;
	
    pChar = (HI_U8 *)&FourCC;
	
	if (pChar[2] == 'w' && pChar[3] == 'b') 
    {
		*pStreamType = AVI_CHUNKID_AUDIO;
	}
	else if (pChar[2] == 'd') 
    {
		if (pChar[3] == 'c') 
        {
			*pStreamType = AVI_CHUNKID_VIDEO;
		}
		if (pChar[3] == 'b') 
        {
			*pStreamType = AVI_CHUNKID_RGB;
		}
		if (pChar[3] == 'd') 
        {
			/* This is DivX-specific */
			*pStreamType = AVI_CHUNKID_DRM;
		}
	}
    else if ((pChar[2] == 'p') && (pChar[3] == 'c'))
    {
        *pStreamType = AVI_CHUNKID_PC;
    }
	else if (pChar[2] == 's' && pChar[3] == 't') 
    {
		*pStreamType = AVI_CHUNKID_TXTCAPTION;
	}
	else if (pChar[2] == 's' && pChar[3] == 'b') 
    {
		*pStreamType = AVI_CHUNKID_BITMAPCAPTION;
	}
	else if (pChar[2] == 'c' && pChar[3] == 'h') 
    {
		*pStreamType = AVI_CHUNKID_CHAPTER;
	}
	else if (pChar[2] == 'i' && pChar[3] == 'x') 
    {
		*pStreamType = AVI_CHUNKID_STANDARDINDEX;	
	}
    else if (pChar[0] == 'i' && pChar[1] == 'x') 
    {
		*pStreamType = AVI_CHUNKID_IX;	
        *pStreamNum = (pChar[2] - '0') * 10 + (pChar[3] - '0' );
        return;
	}
	else if (pChar[0] == 'J' && pChar[1] == 'U' && pChar[2] == 'N' && pChar[3] == 'K') 
    {
		*pStreamType = AVI_CHUNKID_JUNK;
	}
    else if (pChar[0] == 'L' && pChar[1] == 'I' && pChar[2] == 'S' && pChar[3] == 'T')
    {
        *pStreamType = AVI_CHUNKID_LIST;
    }
    else if (pChar[0] == 'r' && pChar[1] == 'e' && pChar[2] == 'c' && pChar[3] == ' ')
    {
        *pStreamType = AVI_CHUNKID_REC;
    }
    else if (pChar[0] == 'i' && pChar[1] == 'd' && pChar[2] == 'x' && pChar[3] == '1')
    {
        *pStreamType = AVI_CHUNKID_IDX1;
    }
    else if (pChar[0] == 'R' && pChar[1] == 'I' && pChar[2] == 'F' && pChar[3] == 'F')
    {
        *pStreamType = AVI_CHUNKID_RIFF;
    }
	else if (pChar[0] == 'R' && pChar[1] == 'E' && pChar[2] == 'S' &&
		(pChar[3] == '1' || pChar[3] == '2' || pChar[3] == '3')) 
    {
		*pStreamType = AVI_CHUNKID_RESERVED;	
	}
    else
    {
		*pStreamType = AVI_CHUNKID_UNKNOW;
    }
	
    *pStreamNum = (pChar[0] - '0') * 10 + (pChar[1] - '0' );
}

void HI_AVI_Parse_Header_StreamData(HI_U32 FourCC, HI_U32 *pStreamNum,AVI_CHUNKID_E  *pStreamType)
{
	
    HI_U8 *pChar;
	
    pChar = (HI_U8 *)&FourCC;
	
	if (pChar[2] == 'w' && pChar[3] == 'b') 
    {
		*pStreamType = AVI_CHUNKID_AUDIO;
	}
	else if (pChar[2] == 'd') 
    {
		if (pChar[3] == 'c') 
        {
			*pStreamType = AVI_CHUNKID_VIDEO;
		}
		if (pChar[3] == 'b') 
        {
			*pStreamType = AVI_CHUNKID_RGB;
		}
		if (pChar[3] == 'd') 
        {
			/* This is DivX-specific */
			*pStreamType = AVI_CHUNKID_DRM;
		}
	}
    else if ((pChar[2] == 'p') && (pChar[3] == 'c'))
    {
        *pStreamType = AVI_CHUNKID_PC;
    }
	else if (pChar[2] == 's' && pChar[3] == 't') 
    {
		*pStreamType = AVI_CHUNKID_TXTCAPTION;
	}
	else if (pChar[2] == 's' && pChar[3] == 'b') 
    {
		*pStreamType = AVI_CHUNKID_BITMAPCAPTION;
	}
	else if (pChar[2] == 'c' && pChar[3] == 'h') 
    {
		*pStreamType = AVI_CHUNKID_CHAPTER;
	}
	else if (pChar[2] == 'i' && pChar[3] == 'x') 
    {
		*pStreamType = AVI_CHUNKID_STANDARDINDEX;	
	}
    else if (pChar[0] == 'i' && pChar[1] == 'x') 
    {
		*pStreamType = AVI_CHUNKID_IX;	
        *pStreamNum = (pChar[2] - '0') * 10 + (pChar[3] - '0' );
        return;
	}
	else if (pChar[0] == 'J' && pChar[1] == 'U' && pChar[2] == 'N' && pChar[3] == 'K') 
    {
		*pStreamType = AVI_CHUNKID_JUNK;
	}
    else if (pChar[0] == 'L' && pChar[1] == 'I' && pChar[2] == 'S' && pChar[3] == 'T')
    {
        *pStreamType = AVI_CHUNKID_LIST;
    }
    else if (pChar[0] == 'r' && pChar[1] == 'e' && pChar[2] == 'c' && pChar[3] == ' ')
    {
        *pStreamType = AVI_CHUNKID_REC;
    }
    else if (pChar[0] == 'i' && pChar[1] == 'd' && pChar[2] == 'x' && pChar[3] == '1')
    {
        *pStreamType = AVI_CHUNKID_IDX1;
    }
    else if (pChar[0] == 'R' && pChar[1] == 'I' && pChar[2] == 'F' && pChar[3] == 'F')
    {
        *pStreamType = AVI_CHUNKID_RIFF;
    }
	else if (pChar[0] == 'R' && pChar[1] == 'E' && pChar[2] == 'S' &&
		(pChar[3] == '1' || pChar[3] == '2' || pChar[3] == '3')) 
    {
		*pStreamType = AVI_CHUNKID_RESERVED;	
	}
    else
    {
		*pStreamType = AVI_CHUNKID_UNKNOW;
    }
	
    *pStreamNum = (pChar[0] - '0') * 10 + (pChar[1] - '0' );
}	

static HI_S32 Read_Header_Avih(FILE *pFile, AVI_AVIH_S *pAvih)
{
	HI_U32 ChunkSize;
    HI_S32 Ret = 0;

	Ret |= (1 != fread(&ChunkSize, 4, 1, pFile));
//	Ret |= (ChunkSize < sizeof(AVI_AVIH_S));
	Ret |= (sizeof(AVI_AVIH_S) != fread(pAvih, 1, sizeof(AVI_AVIH_S), pFile));
	Ret |= fseek(pFile, ChunkSize + (ChunkSize & 1) - sizeof(AVI_AVIH_S), SEEK_CUR);

	return Ret;
}

static HI_S32 Read_Header_Strh(FILE *pFile, AVI_STRL_S *pStrl)
{
	HI_U32 ChunkSize;
    HI_S32 Ret = 0;
	
	Ret |= (1 != fread(&ChunkSize, 4, 1, pFile));
//	Ret |= (ChunkSize < sizeof(AVI_STRH_S));
	Ret |= (sizeof(AVI_STRH_S) != fread(&pStrl->strh, 1, sizeof(AVI_STRH_S), pFile));
	Ret |= fseek(pFile, ChunkSize + (ChunkSize & 1) - sizeof(AVI_STRH_S), SEEK_CUR);
	
	return Ret;
}

static HI_S32 Read_Header_Strf(FILE *pFile, AVI_STRL_S *pStrl)
{
	HI_U32 ChunkSize;
    HI_S32 Ret = 0;

	switch (pStrl->strh.fccType)
	{
	case AVI_FOURCC_VIDS:
		Ret |= (1 != fread(&ChunkSize, 4, 1, pFile));
//		Ret |= (ChunkSize < sizeof(AVI_BITMAPINFOHEADER_S));
		Ret |= (sizeof(AVI_BITMAPINFOHEADER_S) != fread(&pStrl->bmp, 1, sizeof(AVI_BITMAPINFOHEADER_S), pFile));
    	Ret |= fseek(pFile, ChunkSize + (ChunkSize & 1) - sizeof(AVI_BITMAPINFOHEADER_S), SEEK_CUR);
		break;

	case AVI_FOURCC_AUDS:
		Ret |= (1 != fread(&ChunkSize, 4, 1, pFile));
//		Ret |= (ChunkSize < sizeof(AVI_WAVEFORMATEX_S));
		Ret |= (sizeof(AVI_WAVEFORMATEX_S) != fread(&pStrl->wave, 1, sizeof(AVI_WAVEFORMATEX_S), pFile));
    	Ret |= fseek(pFile, ChunkSize + (ChunkSize & 1) - sizeof(AVI_WAVEFORMATEX_S), SEEK_CUR);
		break;

	default:
		Ret |= (1 != fread(&ChunkSize, 4, 1, pFile));
    	Ret |= fseek(pFile, ChunkSize + (ChunkSize & 1), SEEK_CUR);
		break;
	}

	return Ret;
}


#if 0
static void Show_Header_Avih(AVI_AVIH_S *pAvih)
{
	printf("***MainAviHeader***\n");
	printf("MicroSecPerFrame = %d\n", pAvih->dwMicroSecPerFrame);
	printf("MaxBytesPerSec = %d\n", pAvih->dwMaxBytesPerSec);
	printf("Reserved1 = %d\n", pAvih->dwReserved1);
	printf("Flags = %d\n", pAvih->dwFlags);
	printf("TotalFrames = %d\n", pAvih->dwTotalFrames);
	printf("InitialFrames = %d\n", pAvih->dwInitialFrames);
	printf("Streams = %d\n", pAvih->dwStreams);
	printf("SuggestedBufferSize = %d\n", pAvih->dwSuggestedBufferSize);
	printf("Width = %d\n", pAvih->dwWidth);
	printf("Height = %d\n", pAvih->dwHeight);
	printf("Scale = %d\n", pAvih->dwScale);
	printf("Rate = %d\n", pAvih->dwRate);
	printf("Start = %d\n", pAvih->dwStart);
	printf("Length = %d\n", pAvih->dwLength);
}

static void Show_Header_Strh(AVI_STRL_S *pStrl, int idx)
{
	printf("***AVI_AVISTREAMHEADER_S  %d***\n", idx);
	printf("fccType = %d\n", pStrl->strh.fccType);
	printf("fccHandler = %d\n", pStrl->strh.fccHandler);
	printf("Priority = %d\n", pStrl->strh.Priority);
	printf("Language = %d\n", pStrl->strh.Language);
	printf("dwInitialFrames = %d\n", pStrl->strh.dwInitialFrames);
	printf("dwScale = %d\n", pStrl->strh.dwScale);
	printf("dwRate = %d\n", pStrl->strh.dwRate);
	printf("dwStart = %d\n", pStrl->strh.dwStart);
	printf("dwLength = %d\n", pStrl->strh.dwLength);
	printf("dwSuggestedBufferSize = %d\n", pStrl->strh.dwSuggestedBufferSize);
	printf("dwQuality = %d\n", pStrl->strh.dwQuality);
	printf("dwSampleSize = %d\n", pStrl->strh.dwSampleSize);
	printf("FrameLeft = %d\n", pStrl->strh.FrameLeft);
	printf("FrameTop = %d\n", pStrl->strh.FrameTop);
	printf("FrameRight = %d\n", pStrl->strh.FrameRight);
    printf("FrameBottom = %d\n", pStrl->strh.FrameBottom);
}

static void Show_Header_Strf(AVI_STRL_S *pStrl)
{
	switch (pStrl->strh.fccType)
	{
	case AVI_FOURCC_VIDS:
		printf("***AVI_BITMAPINFOHEADER_S***\n");
		printf("biSize = %d\n", pStrl->bmp.biSize);
		printf("biWidth = %d\n", pStrl->bmp.biWidth);
		printf("biHeight = %d\n", pStrl->bmp.biHeight);
		printf("biPlanes = %d\n", pStrl->bmp.biPlanes);
		printf("biBitCount = %d\n", pStrl->bmp.biBitCount);
		printf("biCompression = %d\n", pStrl->bmp.biCompression);
		printf("biSizeImage = %d\n", pStrl->bmp.biSizeImage);
		printf("biXPelsPerMeter = %d\n", pStrl->bmp.biXPelsPerMeter);
		printf("biYPelsPerMeter = %d\n", pStrl->bmp.biYPelsPerMeter);
		printf("biClrUsed = %d\n", pStrl->bmp.biClrUsed);
        printf("biClrImportant = %d\n", pStrl->bmp.biClrImportant);
		break;
	case AVI_FOURCC_AUDS:
		printf("***AVI_WAVEFORMATEX_S***\n");
		printf("wFormatTag = %d\n", pStrl->wave.wFormatTag);
		printf("nChannels = %d\n", pStrl->wave.nChannels);
		printf("nSamplesPerSec = %d\n", pStrl->wave.nSamplesPerSec);
		printf("nAvgBytesPerSec = %d\n", pStrl->wave.nAvgBytesPerSec);
		printf("nBlockAlign = %d\n", pStrl->wave.nBlockAlign);
		printf("wBitsPerSample = %d\n", pStrl->wave.wBitsPerSample);
        printf("cbSize = %d\n", pStrl->wave.cbSize);
		break;
	}

}
#endif
static HI_S32 Read_Idx1(HI_AVI_INFO_S *pg)
{
	FILE *pFile = pg->pFile;
	AVI_INDEXENTRY_S index;
	HI_U32 i, StreamNum, ChunkSize, TotalFrames;
	AVI_CHUNKID_E StreamType;
	HI_S32 Ret = 0;

	Ret |= (1 != fread(&ChunkSize, 4, 1, pFile));
	TotalFrames = ChunkSize / 16;
//	Ret |= fseek(pFile, ChunkSize + (ChunkSize & 1), SEEK_CUR);
	
	pg->m_IndexOffset=ftell(pFile);
	for (i=0; i<TotalFrames && Ret == 0; i++)
	{
		Ret |= (16 != fread(&index, 1, 16, pFile));
		Parse_Header_StreamData(index.ckid, &StreamNum, &StreamType);
		
		if (i == 0)
		{
			pg->m_FirstIndex = index.dwChunkOffset;
			// 记录第一个offset;
		}
		
		switch (StreamType)
		{
		case AVI_CHUNKID_AUDIO:
			pg->AudioFrameNum++;
			pg->TotalFrameNum++;
			break;
		case AVI_CHUNKID_VIDEO:
		case AVI_CHUNKID_RGB:
			pg->VideoFrameNum++;
			pg->TotalFrameNum++;
			break;
		default:
			break;
		}
	}
	fseek(pFile, ChunkSize + (ChunkSize & 1) - TotalFrames * 16, SEEK_CUR);

	return Ret;
	
}

 HI_S32 VODTrans_ReadytoRead(HI_VOID *handle)
 {
     HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;
     FILE *pFile = pg->pFile;
     fseek(pFile, pg->m_DataOffet, SEEK_SET);//重新定位文件流到索引的长度域   

	 return HI_SUCCESS;
 }

 //获取AVI文件转换成要传输的文件格式后的大小
 HI_S32 VODTrans_GetfileLen(HI_VOID *handle,HI_LONG *plFileLen)
{
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;
	FILE *pFile = pg->pFile;
	AVI_INDEXENTRY_S index;
	HI_U32 i, StreamNum, ChunkSize, TotalFrames;
	AVI_CHUNKID_E StreamType;
	HI_S32 Ret = 0;
    HI_U32 filelen=0x200;//文件头长度512
    HI_U32 framelen=0;
    int testi=0;

    *plFileLen=0;
    if(pg->m_bSeekAble != HI_TRUE )
        {return HI_FAILURE;}

	fseek(pFile, pg->m_IndexOffset-4, SEEK_SET);//重新定位文件流到索引的长度域  
	Ret |= (1 != fread(&ChunkSize, 4, 1, pFile));

	TotalFrames = ChunkSize / 16;

	for (i=0; i<TotalFrames && Ret == 0; i++)
	{
		Ret |= (16 != fread(&index, 1, 16, pFile));
		Parse_Header_StreamData(index.ckid, &StreamNum, &StreamType);
		
		if (i == 0)
		{
			pg->m_FirstIndex = index.dwChunkOffset;
			// 记录第一个offset;
		}
		
		switch (StreamType)
		{
		case AVI_CHUNKID_AUDIO:
			pg->AudioFrameNum++;
            filelen+=index.dwChunkLength+8;
			pg->TotalFrameNum++;
			break;
		case AVI_CHUNKID_VIDEO:
            testi++;
                framelen=ALIGN_LENGTH(index.dwChunkLength,ALIGNTYPE_8BYTE); 
                if(index.dwFlags==0x10)
                    framelen+=0x28; //vod传输 关键帧 +帧信息0x28字节
                else
                    framelen+=0x18;//vod传输 非关键帧 +帧信息0x18字节
            filelen+=framelen;
 //           printf("%s frame:%03d,orglen:0x%06X,len:0x%06X total:0x%08lX index.dwFlags:%d\r\n",__FUNCTION__,testi,(index.dwChunkLength),framelen,filelen,index.dwFlags);
		case AVI_CHUNKID_RGB:
			pg->VideoFrameNum++;
			pg->TotalFrameNum++;
			break;
		default:
			break;
		}
	}
	fseek(pFile, ChunkSize + (ChunkSize & 1) - TotalFrames * 16, SEEK_CUR);
     printf("%s frame:%d,orglen:%d,len:0x%06X  total:%08lX\r\n",__FUNCTION__,testi,(index.dwChunkLength),framelen,filelen);
    *plFileLen=filelen;
	return Ret;
	
}

static HI_S32 AVI_CreateReader(HI_AVI_INFO_S *pg)
{
	HI_S32 EntryNum = 0;
	HI_S32 Ret = 0, i, j;
	HI_U32 ChunkIDFourCC, ChunkSize, ChunkType;

    Ret |= (1 != fread(&ChunkIDFourCC, 4, 1, pg->pFile));
    Ret |= (1 != fread(&ChunkSize, 4, 1, pg->pFile));  //f65132 文件长度
    Ret |= (1 != fread(&ChunkType, 4, 1, pg->pFile));

	//printf("ChunkIDFourCC = %d, ChunkType = %d,Ret = %d!\n",ChunkIDFourCC,ChunkType,Ret);
	if (AVI_FOURCC_RIFF != ChunkIDFourCC ||
		AVI_FOURCC_AVI != ChunkType || Ret)
	{
		printf("ChunkIDFourCC failed!\n");
		return -1;
	}
	
	// 记录文件长度
	pg->m_FileLen = ChunkSize + 8;

    printf("pg->m_FileLen = %d!\n",pg->m_FileLen);
	
	while ((HI_U32)ftell(pg->pFile) < pg->m_FileLen) 
    {
        Ret |= (1 != fread(&ChunkIDFourCC, 4, 1, pg->pFile));

		//printf("ChunkIDFourCC = %d",ChunkIDFourCC);
		switch (ChunkIDFourCC)
		{
		case AVI_FOURCC_LIST:
			Ret |= (1 != fread(&ChunkSize, 4, 1, pg->pFile));
            Ret |= (1 != fread(&ChunkType, 4, 1, pg->pFile));

            switch(ChunkType) 
            {
            case AVI_FOURCC_HDRL:
                break;
            case AVI_FOURCC_STRL:
                EntryNum++;
				Ret |= (EntryNum > MAX_AVI_STREAM_NUM);
                break;
            case AVI_FOURCC_MOVI:
				pg->m_DataOffet = ftell(pg->pFile);
				Ret |= fseek(pg->pFile, ChunkSize + (ChunkSize & 1) - 4, SEEK_CUR);
                break;
			default:
				Ret |= fseek(pg->pFile, ChunkSize + (ChunkSize & 1) - 4, SEEK_CUR);
            }

			break;
		case AVI_FOURCC_IDX1:
            pg->m_bSeekAble   = HI_TRUE;
			//pg->m_IndexOffset = ftell(pg->pFile) + 4;
			Ret |= Read_Idx1(pg);
			break;

		case AVI_FOURCC_AVIH:
			Ret |= Read_Header_Avih(pg->pFile, &pg->avih);
//			Show_Header_Avih(&pg->avih);
			break;

		case AVI_FOURCC_STRH:
			Ret |= Read_Header_Strh(pg->pFile, &pg->strl[EntryNum-1]);
//			Show_Header_Strh(&pg->strl[EntryNum-1], EntryNum-1);
			break;

		case AVI_FOURCC_STRF:
			Ret |= Read_Header_Strf(pg->pFile, &pg->strl[EntryNum-1]);
//			Show_Header_Strf(&pg->strl[EntryNum-1]);
			break;

		default:
			Ret |= (1 != fread(&ChunkSize, 4, 1,pg->pFile));
			Ret |= fseek(pg->pFile, ChunkSize + (ChunkSize & 1), SEEK_CUR);
		}

		if (Ret)
		{
			return -1;
		}

	}


	for (i=0; i<EntryNum; i++)
	{
		switch (pg->strl[i].strh.fccType)
		{
		case AVI_FOURCC_VIDS:
			pg->FileInfo.s32VideoIndex = i;
			switch (pg->strl[i].strh.fccHandler)
			{
			case AVI_FOURCC_H264:
			case AVI_FOURCC_h264:
			case AVI_FOURCC_X264:
			case AVI_FOURCC_x264:
			case AVI_FOURCC_VSSH:
			case AVI_FOURCC_vssh:
			case AVI_FOURCC_VSS1:
			case AVI_FOURCC_NJVC:
			case AVI_FOURCC_njvc:
			case AVI_FOURCC_avc1:
				pg->Video.eVEncode = AVI_VIDEO_H264;
				break;
			case AVI_FOURCC_MPG4:
			case AVI_FOURCC_div3:
			case AVI_FOURCC_DIV3:
			case AVI_FOURCC_div4:
			case AVI_FOURCC_DIV4:
			case AVI_FOURCC_divx:
			case AVI_FOURCC_DIVX:
			case AVI_FOURCC_dx50:
			case AVI_FOURCC_DX50:
			case AVI_FOURCC_xvid:
			case AVI_FOURCC_XVID:
			case AVI_FOURCC_mp4s:
			case AVI_FOURCC_MP4S:
			case AVI_FOURCC_m4s2:
        	case AVI_FOURCC_M4S2:
				pg->Video.eVEncode = AVI_VIDEO_MPEG4;
				break;
			case AVI_FOURCC_MJPG:
			case AVI_FOURCC_mjpg:
			case AVI_FOURCC_JPEG:
			case AVI_FOURCC_jpeg:
			case AVI_FOURCC_LJPG:
			case AVI_FOURCC_JPGL:
			case AVI_FOURCC_MJLS:
			case AVI_FOURCC_IJPG:
			case AVI_FOURCC_AVRn:
				pg->Video.eVEncode = AVI_VIDEO_MJPEG;
				break;
			default :
				pg->Video.eVEncode = AVI_VIDEO_UNKNOW;
			}
	//		pg->Video.eVEncode = pg->strl[i].strh.fccHandler;
			pg->Video.lWidth   = pg->strl[i].bmp.biWidth;
			pg->Video.lHeight  = pg->strl[i].bmp.biHeight;
			if (pg->strl[i].strh.dwScale != 0)
			{
				pg->Video.lFrameRate = pg->strl[i].strh.dwRate/pg->strl[i].strh.dwScale;
			}
			else if (pg->avih.dwMicroSecPerFrame != 0)
			{
				pg->Video.lFrameRate = 1000000/pg->avih.dwMicroSecPerFrame;
			}
			else
			{
				pg->Video.lFrameRate = 25;
			}
			
			pg->Video.lBitRate   = 0;
			if (pg->strl[i].strh.dwRate != 0)
			{
                pg->deltaVideoPTS  = (HI_U64)pg->strl[i].strh.dwScale * 10000000 / (HI_U64)pg->strl[i].strh.dwRate;
			
			}
			else
			{
				pg->deltaVideoPTS = pg->avih.dwMicroSecPerFrame * 10;
			}
			
			break;
		case AVI_FOURCC_AUDS:
			pg->FileInfo.s32AudioIndex = i;
			pg->Audio.eAEncode = AVI_AUDIO_UNKNOW;

			for (j=0; j<sizeof(codec_wav_tags)/8; j++)
			{
				if (codec_wav_tags[j][1] == pg->strl[i].wave.wFormatTag)
				{
					pg->Audio.eAEncode = (AVI_CODECTYPE_E)codec_wav_tags[j][0];
				}
			}
			
			pg->Audio.lBitRate = pg->strl[i].wave.nAvgBytesPerSec * 8;
			pg->Audio.lBitsPerSample = pg->strl[i].wave.wBitsPerSample;
			pg->Audio.lBlockAlign = pg->strl[i].wave.nBlockAlign;
			pg->Audio.lChannels = pg->strl[i].wave.nChannels;
			pg->Audio.length = pg->strl[i].strh.dwLength;
			pg->Audio.lFrameFlag = 0;
			pg->Audio.lSamplesPerSec = pg->strl[i].wave.nSamplesPerSec;
			if (pg->strl[i].strh.dwRate != 0)
			{
                pg->deltaAudioPTS  = (HI_U64)pg->strl[i].strh.dwScale * 10000000 / (HI_U64)pg->strl[i].strh.dwRate;
			}
			else
			{
				pg->deltaAudioPTS = 0;
			}
			break;
		}
		

	}

	pg->Duration = pg->avih.dwMicroSecPerFrame * pg->avih.dwTotalFrames / 1000000;
	pg->FileInfo.bSeekEnable = pg->m_bSeekAble;
	pg->FileInfo.struDuration = (HI_U32) pg->Duration;
	pg->FileInfo.u32StreamNum = pg->avih.dwStreams;

	
	// 初始化到第一帧所在位置;
	pg->m_currFrame = 0;
	fseek(pg->pFile, pg->m_DataOffet, SEEK_SET);

    return 0;
}


//////////////////////////////////////////////////////////////////////////
// API函数
HI_S32 HI_Avi_GetFileInfo(void * handle, HI_AVI_FILE_S *pInfo)
{
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;
	
	if (NULL == pg || NULL == pInfo)
	{
		return -1;
	}

	*pInfo = pg->FileInfo;
	return 0;
}

HI_S32 HI_Avi_GetVideoInfo(void * handle, HI_AVI_VIDEO_S *pInfo)
{
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;
	
	if (NULL == pg || NULL == pInfo)
	{
		return -1;
	}
	
	*pInfo = pg->Video;
	return 0;
}

HI_S32 HI_Avi_GetAudioInfo(void * handle, HI_AVI_AUDIO_S *pInfo)
{
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;
	
	if (NULL == pg || NULL == pInfo)
	{
		return -1;
	}
	
	*pInfo = pg->Audio;
	return 0;
}

HI_S32 HI_Avi_SeekPrevKeyFrame(void *handle, HI_U64 pts)
{
	HI_U64 VideoPts = 0, AudioPts = 0;
	HI_S32 i, Ret = 0, KeyIndex = -1, totalLen;
	AVI_INDEXENTRY_S index = {0}, tmpIndex;
	AVI_CHUNKID_E ChunkID;
	HI_U32 FilePos, ChunkSize, StreamNum;
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;
	if (NULL == pg || HI_FALSE == pg->m_bSeekAble)
	{
		return -1;
	}
	pts = pts * 10000;
	FilePos = ftell(pg->pFile);
	fseek(pg->pFile, pg->m_IndexOffset - 4, SEEK_SET);
	Ret |= (1 != fread(&ChunkSize, 4, 1, pg->pFile));
	totalLen = ChunkSize / 16;
	
	for (i=0; i<totalLen; i++)
	{
		Ret |= (16 != fread(&index, 1, 16, pg->pFile));
		Parse_Header_StreamData(index.ckid, &StreamNum, &ChunkID);
		
		if (ChunkID == AVI_CHUNKID_VIDEO)
		{
			if (index.dwFlags & 0x10)
			{
				KeyIndex = i;
				tmpIndex = index;
				pg->currVideoPTS = VideoPts;
				pg->currAudioPTS = AudioPts;
			}

			if (VideoPts >= pts)
			{
				break;
			}

			VideoPts += pg->deltaVideoPTS;
		}
		
		if (ChunkID == AVI_CHUNKID_AUDIO)
		{
			AudioPts += pg->deltaAudioPTS;
		}
		
	}

	if (KeyIndex >= 0)
	{
		// find
		Ret |= (0 != fseek(pg->pFile, pg->m_DataOffet + tmpIndex.dwChunkOffset - pg->m_FirstIndex, SEEK_SET));
		return 0;
	}
	else
	{
		// not find
		fseek(pg->pFile, FilePos, SEEK_SET);
		return -1;
	}
	
}

HI_S32 HI_Avi_SeekNextKeyFrame(void *handle, HI_U64 pts)
{
	HI_S32 Ret = 0, KeyIndex = -1 ;
	AVI_INDEXENTRY_S index = {0};
	AVI_INDEXENTRY_S IndexBuffer[256];
	AVI_CHUNKID_E ChunkID;
	HI_U32 StreamNum, FilePos, ChunkSize, i, totalLen;
	HI_U64 VideoPts = 0, AudioPts = 0;
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;
	if (NULL == pg || HI_FALSE == pg->m_bSeekAble)
	{
		return -1;
	}

	pts = pts * 10000;
	FilePos = ftell(pg->pFile);
	fseek(pg->pFile, pg->m_IndexOffset - 4, SEEK_SET);
	Ret |= (1 != fread(&ChunkSize, 4, 1, pg->pFile));
	totalLen = ChunkSize / 16;

	for (i=0; i<totalLen; i++)
	{
		if ((i & 0xff) == 0)
		{
			// 如果剩余超过256个index, 如果不足, 
			if ((totalLen - i) > 256)
			{
				Ret |= (256 != fread(IndexBuffer, 16, 256, pg->pFile));
			}
			else
			{
				Ret |= ((totalLen - i) != fread(IndexBuffer, 16, (totalLen - i), pg->pFile));
			}
			index = IndexBuffer[0];
		}
		else
		{
			index = IndexBuffer[i & 0xff];
		}
		//Ret |= (16 != fread(&index, 1, 16, pg->pFile));
		Parse_Header_StreamData(index.ckid, &StreamNum, &ChunkID);

		if (ChunkID == AVI_CHUNKID_VIDEO)
		{
			if (VideoPts >= pts && (index.dwFlags & 0x10))
			{
				KeyIndex = i;
				break;
			}
			VideoPts += pg->deltaVideoPTS;
		}

		if (ChunkID == AVI_CHUNKID_AUDIO)
		{
			AudioPts += pg->deltaAudioPTS;
		}
	}

	if (KeyIndex >= 0)
	{
		// find
		pg->currVideoPTS = VideoPts;
		pg->currAudioPTS = AudioPts;
		Ret |= (0 != fseek(pg->pFile, pg->m_DataOffet + index.dwChunkOffset - pg->m_FirstIndex, SEEK_SET));
		return 0;
	}
	else
	{
		// not find
		fseek(pg->pFile, FilePos, SEEK_SET);
		return -1;
	}

}

HI_S32 HI_Avi_ReadFrame(void *handle, HI_AVI_FRAME_S *frame)
{
	HI_U32 ChunkSize, ChunkType;	
	AVI_INDEXENTRY_S index = {0};
    HI_S32 Ret = 0;
	AVI_CHUNKID_E ChunkID;
	HI_U32 StreamNum;

	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;

	if (NULL == pg || NULL == frame)
	{
		return HI_AVI_ERR_READ_FRAME;
	}

	
	if(pg->m_bSeekAble)
	{
		// 首先定位到idx1字段, 找到偏移地址
		Ret |= (0 != fseek(pg->pFile, pg->m_IndexOffset + 16 * pg->m_currFrame, SEEK_SET)); // 从文件头开始定位
		//Ret |= (0 != fseek(pg->pFile, pg->m_IndexOffset + (pg->m_currFrame<<4), SEEK_SET)); // 从文件头开始定位
		Ret |= (16 != fread(&index, 1, 16, pg->pFile));
		Ret |= (0 != fseek(pg->pFile, pg->m_DataOffet + index.dwChunkOffset - pg->m_FirstIndex, SEEK_SET));
		Ret |= (1 != fread(&ChunkType, 4, 1, pg->pFile));
	    Ret |= (1 != fread(&ChunkSize, 4, 1, pg->pFile));
	}
	else
	{
		// 如果文件不可seek, 只能按顺序读下一帧
		Ret |= (1 != fread(&ChunkType, 4, 1, pg->pFile));
    	Ret |= (1 != fread(&ChunkSize, 4, 1, pg->pFile));
	}

	if (ChunkSize > pg->m_pFrameBufferSize)
	{
		//Ret |= fseek(pg->pFile, ChunkSize + (ChunkSize & 1), SEEK_CUR);
		return HI_AVI_ERR_READ_FRAME;
	}

	if (pg->m_currFrame >= pg->TotalFrameNum)
	{
		return HI_AVI_ERR_ENDFILE;
	}

	pg->m_currFrame++;
	Parse_Header_StreamData(ChunkType, &StreamNum, &ChunkID);

	frame->pu8Data       = pg->m_pFrameBuffer;
	frame->u32Size       = ChunkSize;
	frame->u32StreamType = ChunkID;
	frame->u32StreamNum  = StreamNum;
	frame->u32OutPutFlag = 1;
	frame->u32Flags        =  (index.dwFlags & 0x10)? 1: 0;
	//frame->u32Flags      = (index.dwFlags & 0x10)>> 1; //(index.dwFlags & 0x10)? 1: 0;

	switch(ChunkID) 
    {
    case AVI_CHUNKID_VIDEO:
		fread(pg->m_pFrameBuffer, 1, ChunkSize + (ChunkSize & 1), pg->pFile);
		frame->u64Pts     = pg->currVideoPTS;
		frame->u64Dts     = 0;
		frame->u32Duration= (HI_U32)pg->deltaVideoPTS;
		pg->currVideoPTS += pg->deltaVideoPTS;
		break;
	case AVI_CHUNKID_AUDIO:
		fread(pg->m_pFrameBuffer, 1, ChunkSize + (ChunkSize & 1), pg->pFile);
		frame->u64Pts     = pg->currAudioPTS;
		frame->u64Dts     = 0;
		frame->u32Duration= (HI_U32)pg->deltaAudioPTS;
		pg->currAudioPTS += pg->deltaAudioPTS;
		break;
	case AVI_CHUNKID_RGB:
		fread(pg->m_pFrameBuffer, 1, ChunkSize + (ChunkSize & 1), pg->pFile);
		frame->u64Pts     = pg->currVideoPTS;
		frame->u64Dts     = 0;
		frame->u32Duration= (HI_U32)pg->deltaVideoPTS;
		pg->currVideoPTS += pg->deltaVideoPTS;
		break;

	case AVI_CHUNKID_IDX1:
		Ret = -1;
		break;

	case AVI_CHUNKID_LIST:
		Ret |= fseek(pg->pFile, 4, SEEK_CUR); 
		Ret |= HI_Avi_ReadFrame(handle, frame);
		break;


	default:
        Ret |= fseek(pg->pFile, ChunkSize + (ChunkSize & 1), SEEK_CUR);   // skip 
		Ret |= HI_Avi_ReadFrame(handle, frame);
	}

	
	//printf("[yuanzhenkun] HI_Avi_ReadFrame, current frame:%ld, PTS:%ld\n", pg->m_currFrame, frame->u64Pts);
	

	if (Ret)
	{
		return HI_AVI_ERR_READ_FRAME;
	}
	else
	{
		
		//printf("===REPLAY===[%s, %d] avi fp:0x%x, avi idx fp:0x%x, fp postion:0x%x\n", __FILE__, __LINE__, pg->pFile, pg->pFileTmp, ftell(pg->pFile) );
		return HI_AVI_OK;
	}
}



/*打开一个Avi文件*/
void* HI_Avi_ReadOpen(char *pFileName)
{
	HI_AVI_INFO_S *pg = NULL;
    if (NULL == pFileName)  return NULL;
	if (0 >= strlen(pFileName))
	{
		printf("pFileName len err!\n");
		return NULL;
	}

	pg = (HI_AVI_INFO_S *) malloc(sizeof(HI_AVI_INFO_S));
	if (NULL == pg)  
	{
		printf("pg  err!\n");
		return NULL;
	}
	memset( pg, 0x0, sizeof(HI_AVI_INFO_S));

	pg->m_pFrameBuffer = NULL;
	pg->m_pFrameBufferSize = 800 * 1024;
	pg->currAudioPTS  = 0;
	pg->currVideoPTS  = 0;
	pg->deltaAudioPTS = 0;
	pg->deltaVideoPTS = 0;
	pg->TotalFrameNum = 0;
	pg->AudioFrameNum = 0;
	pg->VideoFrameNum = 0;
    pg->u32AudioToltalSize = 0;
	pg->m_bSeekAble   = HI_FALSE;

	pg->Audio.eAEncode       = AVI_AUDIO_UNKNOW;
	pg->Audio.lBitRate       = 0;
	pg->Audio.lBitsPerSample = 0;
	pg->Audio.lBlockAlign    = 0;
	pg->Audio.lChannels      = 0;
	pg->Audio.length         = 0;
	pg->Audio.lFrameFlag     = 0;
	pg->Audio.lSamplesPerSec = 0;
	pg->Audio.pReserved      = 0;

	pg->Video.eVEncode   = AVI_VIDEO_UNKNOW;
	pg->Video.lBitRate   = 0;
	pg->Video.lFrameRate = 0;
	pg->Video.lHeight    = 0;
	pg->Video.lWidth     = 0;

	pg->FileInfo.u32StreamNum = 0;
	pg->FileInfo.struDuration = 0;
	pg->FileInfo.s32AudioIndex = -1;
	pg->FileInfo.s32VideoIndex = -1;
	pg->FileInfo.bSeekEnable = HI_FALSE;

	pg->m_pFrameBuffer = (HI_U8 *)malloc(pg->m_pFrameBufferSize);

	printf("pFileName = %s!\n",pFileName);
	pg->pFile = fopen((const char *)pFileName, "rb");
	if (NULL == pg->pFile || NULL == pg->m_pFrameBuffer)  
	{
		printf("pg  open failed!\n");
		goto exit;
	}

	printf("Start to CreateReader!\n");
	// 解析文件头, 
	if (AVI_CreateReader(pg))
	{
		printf("CreateReader failed!\n");
		goto exit;
	}
	strcpy((char*)pg->FileInfo.u8FormatName, "avi");
    strcpy((char*)pg->FileInfo.u8FileName, (const char*)pFileName);

	return (void *) pg;

exit:
	HI_Avi_ReadClose(pg);
	return NULL;
}

void HI_Avi_ReadClose(void *pHandle)
{
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *)pHandle;
	
	if (NULL != pg)
	{
		if (NULL != pg->pFile) 
		{
			fclose(pg->pFile);
			pg->pFile = NULL;
		}

		if (NULL != pg->pFileTmp) 
		{
			fclose(pg->pFileTmp);
			pg->pFileTmp = NULL;
		}

		if (NULL != pg->m_pFrameBuffer)
		{
			free(pg->m_pFrameBuffer);
			pg->m_pFrameBuffer = NULL;
		}

		free(pg);
	}
}


//////////////////////////////////////////////////////////////////////////
//  下面为打包AVI部分

#pragma pack(2)

typedef struct
{
	HI_U32 Riff;
	HI_U32 FileLen;
	HI_U32 Avi;
	HI_U32 List;
	HI_U32 HdrlLen;
	HI_U32 hdrl;
	HI_U32 avih;
	HI_U32 AvihLen;
	AVI_AVIH_S main_avi_header;
} AVI_FILE_HEADER;

typedef struct
{
	HI_U32 List;
	HI_U32 StrlLen;
	HI_U32 strl;
	HI_U32 strh;
	HI_U32 StrhLen;
	AVI_STRH_S StreamHeader;
	HI_U32 strf;
	HI_U32 StrfLen;
    AVI_BITMAPINFOHEADER_S StreamFormat;
} AVI_VIDEO_HEADER;

typedef struct
{
	HI_U32 List;
	HI_U32 StrlLen;
	HI_U32 strl;
	HI_U32 strh;
	HI_U32 StrhLen;
	AVI_STRH_S StreamHeader;
	HI_U32 strf;
	HI_U32 StrfLen;
    AVI_WAVEFORMATEX_S StreamFormat;
} AVI_AUDIO_HEADER;

typedef struct
{
	HI_U32 Junk;
	HI_U32 JunkLen;
	HI_U32 Reserved[125]; // why 125??
} AVI_JUNK_HEADER;

typedef struct
{
	HI_U32 List;
	HI_U32 PayloadLen;
	HI_U32 movi;
} AVI_MOVI_HEADER;

typedef struct
{
	HI_U32 idx1;
	HI_U32 Idx1Len;
} AVI_IDX1_HEADER;

#if 0
typedef struct 
{
	HI_U32 ckid;
	HI_U32 FrameLen;
} AVI_FRAME_HEADER;
#endif

#pragma pack()

static void WriteRelease(HI_AVI_INFO_S* pg)
{
	if (NULL != pg)
	{
		if (NULL != pg->pFile)
		{
			fclose(pg->pFile);
			pg->pFile = NULL;
		}

		if (NULL != pg->pFileTmp)
		{
			fclose(pg->pFileTmp);
			pg->pFileTmp = NULL;
		}

		if (NULL != pg->m_pFrameBuffer)
		{
			free(pg->m_pFrameBuffer);
			pg->m_pFrameBuffer = NULL;
		}

		free(pg);
		pg = NULL;
	}

}


// 可以在写文件的任意时刻调用此函数
static int WriteHeader(HI_AVI_INFO_S* pg, HI_U32 bEndOfFile)
{
	AVI_FILE_HEADER file;
	AVI_JUNK_HEADER junk;
	AVI_MOVI_HEADER movi;
	int ret = 0;

	HI_U32 FileOffset = 0, IndexFileLen = 0;
	// 定位到文件头
	fseek(pg->pFile, 0, SEEK_SET);

	file.Riff     = AVI_FOURCC_RIFF;
	file.FileLen  = bEndOfFile? (pg->m_DataOffet + AVI_HEADER_LENGTH + pg->TotalFrameNum * 16): (pg->m_DataOffet + 512);
	file.Avi      = AVI_FOURCC_AVI;
	file.List     = AVI_FOURCC_LIST;
	file.HdrlLen  = AVI_HEADER_LENGTH - 32;  // 文件头长度 - LIST____movi - RIFF____AVI_LIST____
	file.hdrl     = AVI_FOURCC_HDRL;
	file.avih     = AVI_FOURCC_AVIH;
	file.AvihLen  = sizeof(AVI_AVIH_S);

    if (0 != pg->Video.lFrameRate)
	{
		file.main_avi_header.dwMicroSecPerFrame = 1000000 / pg->Video.lFrameRate;
	}
	else
	{
		file.main_avi_header.dwMicroSecPerFrame = 0;
	}
    
	file.main_avi_header.dwMaxBytesPerSec   = 0xffffffff;
	file.main_avi_header.dwReserved1        = 0;
	file.main_avi_header.dwFlags            = 0x0110;
	file.main_avi_header.dwTotalFrames      = pg->VideoFrameNum;
	file.main_avi_header.dwInitialFrames    = 0;

    //码流数，其实可以不止2个码流，但这里写死了
	file.main_avi_header.dwStreams          = ((pg->VideoFrameNum>0) ? 1: 0) + ((pg->AudioFrameNum>0)? 1: 0);
    
	file.main_avi_header.dwSuggestedBufferSize = 1000000;
	file.main_avi_header.dwWidth            = pg->Video.lWidth;
	file.main_avi_header.dwHeight           = pg->Video.lHeight;
	file.main_avi_header.dwScale            = 0;
	file.main_avi_header.dwRate             = 0;
	file.main_avi_header.dwStart            = 0;
	file.main_avi_header.dwLength           = 0;
	fwrite(&file, sizeof(AVI_FILE_HEADER),1,pg->pFile);
	FileOffset += sizeof(AVI_FILE_HEADER);

	if (pg->VideoFrameNum > 0)
	{
//        printf("$_$_$_$_$ in %s, Line %d, WriteHeader called, pg->VideoFrameNum is %u \n", __FUNCTION__, __LINE__, pg->VideoFrameNum );
    
    	AVI_VIDEO_HEADER video;
		video.List    = AVI_FOURCC_LIST;
		video.StrlLen = sizeof(AVI_STRH_S) + sizeof(AVI_BITMAPINFOHEADER_S) + 20;
		video.strl    = AVI_FOURCC_STRL;
		video.strh    = AVI_FOURCC_STRH;
		video.StrhLen = sizeof(AVI_STRH_S);
		video.strf    = AVI_FOURCC_STRF;
		video.StrfLen = sizeof(AVI_BITMAPINFOHEADER_S);

		video.StreamHeader.fccType    = AVI_FOURCC_VIDS;//vids 表示视频数据流
		video.StreamHeader.fccHandler = AVI_FOURCC_H264;//这里不应该写死 //数据流解压缩的驱动程序代码 --引用自网页
		video.StreamHeader.dwFlags    = 0;
		video.StreamHeader.Priority   = 0;
		video.StreamHeader.Language   = 0;	
		video.StreamHeader.dwInitialFrames = 0;
		video.StreamHeader.dwScale    = 1000;
		video.StreamHeader.dwRate     = 1000 * pg->Video.lFrameRate;
		video.StreamHeader.dwStart    = 0;
		video.StreamHeader.dwLength   = pg->VideoFrameNum;
		video.StreamHeader.dwSuggestedBufferSize = 1000000;
		video.StreamHeader.dwQuality  = 0xffffffff;
		video.StreamHeader.dwSampleSize = 0;
		video.StreamHeader.FrameLeft   = 0;
		video.StreamHeader.FrameTop    = 0;
		video.StreamHeader.FrameRight  = (HI_U16)pg->Video.lWidth;
		video.StreamHeader.FrameBottom = (HI_U16)pg->Video.lHeight;

		video.StreamFormat.biSize      = sizeof(AVI_BITMAPINFOHEADER_S);
		video.StreamFormat.biWidth     = pg->Video.lWidth;
		video.StreamFormat.biHeight    = pg->Video.lHeight;
		video.StreamFormat.biPlanes    = 1;
		video.StreamFormat.biBitCount  = 24;
		video.StreamFormat.biCompression = AVI_FOURCC_H264;//这里不应该写死
		video.StreamFormat.biSizeImage = pg->Video.lWidth * pg->Video.lHeight;
		video.StreamFormat.biXPelsPerMeter = 0;
		video.StreamFormat.biYPelsPerMeter = 0;
		video.StreamFormat.biClrUsed       = 0;
		video.StreamFormat.biClrImportant  = 0;
		if(fwrite(&video, sizeof(AVI_VIDEO_HEADER), 1, pg->pFile) != 1)
		{
			perror("#_$_#_$_# fwrite video header err \n");
			ret =  -1;
		}
		FileOffset += sizeof(AVI_VIDEO_HEADER);
	}

	if (pg->AudioFrameNum > 0)
	{
    	AVI_AUDIO_HEADER audio;
		HI_S32 j;

		audio.List    = AVI_FOURCC_LIST;
		audio.StrlLen = sizeof(AVI_STRH_S) + sizeof(AVI_WAVEFORMATEX_S) + 20;
		audio.strl    = AVI_FOURCC_STRL;
		audio.strh    = AVI_FOURCC_STRH;
		audio.StrhLen = sizeof(AVI_STRH_S);
		audio.strf    = AVI_FOURCC_STRF;
		audio.StrfLen = sizeof(AVI_WAVEFORMATEX_S);
#if 1
		audio.StreamHeader.fccType    = AVI_FOURCC_AUDS;
		audio.StreamHeader.fccHandler = 0;//TRANSFORM_FOURCC('g','7','1','1');//0;
		audio.StreamHeader.dwFlags    = 0;
		audio.StreamHeader.Priority   = 0;
		audio.StreamHeader.Language   = 0;	
		audio.StreamHeader.dwInitialFrames = 0;
		audio.StreamHeader.dwScale    = 1;
		audio.StreamHeader.dwRate     = pg->Audio.lSamplesPerSec;//pg->Audio.lSamplesPerSec/4;//g711:8000, g726:2000
		audio.StreamHeader.dwStart    = 0;
		audio.StreamHeader.dwLength   = pg->u32AudioToltalSize;//pg->AudioFrameNum*320;//总的采样点数//pg->AudioFrameNum*320/4;//80
		audio.StreamHeader.dwSuggestedBufferSize = 1000000;
		audio.StreamHeader.dwQuality  = 0xffffffff;
		audio.StreamHeader.dwSampleSize = pg->Audio.lBlockAlign;
		audio.StreamHeader.FrameLeft   = 0;
		audio.StreamHeader.FrameTop    = 0;
		audio.StreamHeader.FrameRight  = 0;
		audio.StreamHeader.FrameBottom = 0;


		audio.StreamFormat.wFormatTag      = 0;
		audio.StreamFormat.nChannels       = pg->Audio.lChannels;
		audio.StreamFormat.nSamplesPerSec  = pg->Audio.lSamplesPerSec;
		audio.StreamFormat.nAvgBytesPerSec = pg->Audio.lBitRate/8;//pg->Audio.lBitRate/8/2/4;//g711:8000, g726:2000
		audio.StreamFormat.nBlockAlign     = pg->Audio.lBlockAlign;
		audio.StreamFormat.wBitsPerSample  = pg->Audio.lBitsPerSample;//pg->Audio.lBitsPerSample/2/4;//g711:8, g726:2
		audio.StreamFormat.cbSize          = 0;
#else
        audio.StreamHeader.fccType    = AVI_FOURCC_AUDS;
        audio.StreamHeader.fccHandler = 0;//TRANSFORM_FOURCC('g','7','1','1');//0;
        audio.StreamHeader.dwFlags    = 0;
        audio.StreamHeader.Priority   = 0;
        audio.StreamHeader.Language   = 0;  
        audio.StreamHeader.dwInitialFrames = 0;
        audio.StreamHeader.dwScale    = 1;//pg->Audio.lBlockAlign;
        audio.StreamHeader.dwRate     = 8000;//pg->Audio.lBitRate;//pg->Audio.lSamplesPerSec*pg->Audio.lBlockAlign*105/400;  //pg->Audio.lSamplesPerSec;
        audio.StreamHeader.dwStart    = 0;
        audio.StreamHeader.dwLength   = pg->AudioFrameNum*320;//pg->AudioFrameNum;
        audio.StreamHeader.dwSuggestedBufferSize = 1000000;
        audio.StreamHeader.dwQuality  = 0xffffffff;
        audio.StreamHeader.dwSampleSize = 1;//2;//pg->Audio.lBlockAlign;
        audio.StreamHeader.FrameLeft   = 0;
        audio.StreamHeader.FrameTop    = 0;
        audio.StreamHeader.FrameRight  = 0;
        audio.StreamHeader.FrameBottom = 0;


        audio.StreamFormat.wFormatTag      = 0;
        audio.StreamFormat.nChannels       = 1;//(HI_U16) pg->Audio.lChannels;
        audio.StreamFormat.nSamplesPerSec  = 8000;//pg->Audio.lSamplesPerSec;
        audio.StreamFormat.nAvgBytesPerSec = 8000;//16000;//pg->Audio.lBitRate / 8;//pg->Audio.lBitRate;
        audio.StreamFormat.nBlockAlign     = 1;//(HI_U16) pg->Audio.lBlockAlign;
        audio.StreamFormat.wBitsPerSample  = 8;//16;//(HI_U16) pg->Audio.lBitsPerSample;
        audio.StreamFormat.cbSize          = 0;

#endif
		for (j=0; j<sizeof(codec_wav_tags)/8; j++)
		{
			if (codec_wav_tags[j][0] == pg->Audio.eAEncode)
			{
				audio.StreamFormat.wFormatTag = (HI_U16)codec_wav_tags[j][1];
                break;
			}
		}
		if (fwrite(&audio, sizeof(AVI_AUDIO_HEADER), 1, pg->pFile) != 1)
		{
			perror("############ fwrite audio header err \n");
			ret = -1;
		}
		FileOffset += sizeof(AVI_AUDIO_HEADER);
	}
	
	memset(&junk, 0, sizeof(junk));
	junk.Junk       = AVI_FOURCC_JUNK;
	junk.JunkLen    = AVI_HEADER_LENGTH - FileOffset - 12 - 8;
	fwrite(&junk, 1, junk.JunkLen + 8, pg->pFile);

	movi.List       = AVI_FOURCC_LIST;
	movi.PayloadLen = pg->m_DataOffet + 4;
	movi.movi       = AVI_FOURCC_MOVI;
	fwrite(&movi, 1, 12, pg->pFile);

	// 定位到文件尾
	fseek(pg->pFile, 0, SEEK_END);

	if (bEndOfFile)
	{
		#define INDEX_BUFFER_LEN  4096
	    HI_U8 IndexBuffer[INDEX_BUFFER_LEN];
	    AVI_IDX1_HEADER idx1;

		idx1.idx1     = AVI_FOURCC_IDX1;
		idx1.Idx1Len  = pg->TotalFrameNum * 16;
		fwrite(&idx1, 1, 8, pg->pFile);
        
		fseek(pg->pFileTmp, 0, SEEK_SET);// 定位到pg->pFileTmp文件的文件头

//        printf("$$$$$$$$$$in %s, Line %d, Write_Head called\n", __FILE__, __LINE__ );
		
		// 分段写入index数据
		do 
		{
			IndexFileLen = fread(IndexBuffer, 1, INDEX_BUFFER_LEN, pg->pFileTmp);
			if (fwrite(IndexBuffer, IndexFileLen, 1, pg->pFile) != 1)
			{
				perror("############ fwrite index err\n");
                printf("&&&&&& fwrite index err at %s, line %d\n", __FUNCTION__, __LINE__);
				ret =  -1;
                break;
			}
		} while (IndexFileLen == INDEX_BUFFER_LEN);
	}

	return ret;
}

int  HI_Avi_WriteClose(void *pHandle)
{

	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S* )pHandle;
	int ret = 0;
	
//    printf("$$$$$$$$$$in %s, Line %d, HI_Avi_WriteClose called\n", __FUNCTION__, __LINE__ );
	
	if (NULL != pg)
	{
		ret = WriteHeader(pg, 1);
		WriteRelease(pg);
	}

	return ret;

}

void * HI_Avi_WriteOpen(char *pFileName,  
						HI_AVI_VIDEO_S *pVideo, // 3个入参 
						HI_AVI_AUDIO_S *pAudio)
{
    const char *tmpFileName = "avi_idx.dat";
	//cht HI_S32 StreamNum = 0;
	HI_AVI_INFO_S *pg = NULL;
    if (NULL == pFileName || (NULL == pVideo && NULL == pAudio))
	{
		return NULL;
	}
	
	pg = (HI_AVI_INFO_S *) malloc(sizeof(HI_AVI_INFO_S));
	if (NULL == pg)  
	{
		return NULL;
	}
	
	memset(pg, 0, sizeof(HI_AVI_INFO_S));

//    printf("[INFO]at file %s, line %d, the input Para file name is %s\n", __FILE__, __LINE__, pFileName); 
	pg->pFile 	 = fopen((const char *)pFileName, "wb");//只写方式打开
	pg->pFileTmp = fopen(tmpFileName, "w+b");//更新方式打开(读和写)
	
	if (NULL == pg->pFile || NULL == pg->pFileTmp)  
	{
		goto exit;
	}

	#if 0
	
   	if((setvbuf(pg->pFile, NULL, _IONBF, 0 ) != 0)||(setvbuf(pg->pFileTmp, NULL, _IONBF, 0 ) != 0))   //不使用缓存by z00225145
   	{
        	printf( "Incorrect type or size of buffer for pg->pFile \n" );
   	}	
	#endif

	pg->TotalFrameNum = 0;
	pg->AudioFrameNum = 0;
	pg->VideoFrameNum = 0;
    pg->u32AudioToltalSize = 0;
	pg->HasVideo = 0;
	pg->HasAudio = 0;
	
	if (NULL != pVideo)
	{
		pg->HasVideo = 1;
		pg->Video.lWidth   = pVideo->lWidth;
		pg->Video.lHeight  = pVideo->lHeight;
		pg->Video.eVEncode = pVideo->eVEncode;
		if (pVideo->lFrameRate)
		{
			pg->Video.lFrameRate = pVideo->lFrameRate;
		}
		else
		{
			pg->Video.lFrameRate = 25;
		}
	}

	if (NULL != pAudio)
	{
		pg->HasAudio = 1;
		//pg->Audio = *pAudio; //by chendan 这样直接把指针赋给结构体?
		memcpy(&pg->Audio,pAudio,sizeof(HI_AVI_AUDIO_S));
	}
	WriteHeader(pg, 0);//写了个头?

	return (void *) pg;
	
exit:
	WriteRelease(pg);
	return NULL;
}


HI_S32 HI_Avi_WriteFrame(void *handle, HI_AVI_FRAME_S *frame)
{
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *)handle;
	AVI_FRAME_HEADER fh;
	AVI_INDEXENTRY_S index;
	HI_U32 u32StreamType = AVI_CHUNKID_NONE;

	HI_U32 wStart = 0, toWrite1 = 0, toWrite2 = 0, toWrite3 = 0, wrote = 0;

//frame->u64Pts的时间信息并没有写入
	if (NULL == pg || NULL == frame)
	{
		return HI_FAILURE;
	}
	if (NULL == pg->pFile || NULL == pg->pFileTmp)
	{
 		return HI_FAILURE;
	}

    
	if (1 == frame->u32StreamType)
	{
		u32StreamType = AVI_CHUNKID_AUDIO;//AVI_CHUNKID_VIDEO; 这是原先代码的一个最重大的bug
	}
	else if(2 == frame->u32StreamType)
	{
		u32StreamType = AVI_CHUNKID_VIDEO;//AVI_CHUNKID_AUDIO;
	}

    //printf("@@@@@@@@@ avi stremtype is %d at %s, line %d \n", u32StreamType, __FILE__, __LINE__);

	switch(u32StreamType)
	{
        
	case AVI_CHUNKID_VIDEO:
		if (pg->HasVideo)
		{
            //printf("@@@@@@@@@ HI_Avi_WriteFrame called at %s, line %d, HasVideo is %u \n", __FILE__, __LINE__, pg->HasVideo);

			wStart = HI_GetSystemTime( );
        
			pg->VideoFrameNum++;
			pg->TotalFrameNum++;
            
			fh.ckid       = AVI_FOURCC_00dc;//00 压缩视频帧
			fh.FrameLen   = frame->u32Size;
            
			index.ckid    = AVI_FOURCC_00dc;//00 压缩视频帧
			index.dwFlags = (frame->u32Flags == 1)? 0x10: 0;
			index.dwChunkOffset = pg->m_DataOffet + AVI_FIRST_FRAME_OFFSET;//AVI_FIRST_FRAME_OFFSET is 4			
			index.dwChunkLength = frame->u32Size;
            
			pg->m_DataOffet    += frame->u32Size  + (frame->u32Size & 1) + sizeof(AVI_FRAME_HEADER);
			
			toWrite1 = HI_GetSystemTime( );
		
			//printf("$$$$$$$$$$$$$avi write to file frame flage %d\n", frame->u32Flags);	
			if (fwrite(&fh, sizeof(AVI_FRAME_HEADER), 1, pg->pFile) != 1)
			{
				perror("############ fwrite video AVI_FRAME_HEADER err \n");
         		return HI_FAILURE;
			}
			toWrite2 = HI_GetSystemTime( );
			
			if (fwrite(frame->pu8Data, frame->u32Size + (frame->u32Size & 1), 1, pg->pFile) != 1)
			{
				perror("############ fwrite AVI video data err \n");
         		return HI_FAILURE;
			}

			toWrite3 = HI_GetSystemTime( );
			if (fwrite(&index,  16, 1, pg->pFileTmp) != 1)// idx存放于pFileTmp
			{
				perror("############ fwrite  AVI video index err \n");
         		return HI_FAILURE;
			}

			fflush(pg->pFile);

			wrote = HI_GetSystemTime( );

			if(wrote > wStart + 2000 )
				printf("Video write, start:%u, 2Write:%u-%u-%u, wrote:%u\n", wStart, toWrite1, toWrite2, toWrite3, wrote );
			
		}
		
		break;
	case AVI_CHUNKID_AUDIO:
		if (pg->HasAudio)
		{
			wStart = HI_GetSystemTime( );
			
			pg->AudioFrameNum++;
			pg->TotalFrameNum++;
            pg->u32AudioToltalSize += frame->u32Size;
			fh.ckid       = AVI_FOURCC_01wb;//01压缩音频
			fh.FrameLen   = frame->u32Size;
            
			index.ckid    = AVI_FOURCC_01wb;//01压缩音频
			index.dwFlags = 0x10;
			index.dwChunkOffset = pg->m_DataOffet + AVI_FIRST_FRAME_OFFSET;
			index.dwChunkLength = frame->u32Size;
            
			pg->m_DataOffet    += frame->u32Size  + (frame->u32Size & 1) + sizeof(AVI_FRAME_HEADER);

			toWrite1 = HI_GetSystemTime( );
			if (fwrite(&fh, sizeof(AVI_FRAME_HEADER), 1, pg->pFile) != 1)
			{
				perror("############ fwrite audio AVI_FRAME_HEADER err \n");
         		return HI_FAILURE;
			}

			toWrite2 = HI_GetSystemTime( );
			if (fwrite(frame->pu8Data, frame->u32Size + (frame->u32Size & 1), 1, pg->pFile) != 1)
			{
				perror("############ fwrite AVI audio data err \n");
         		return HI_FAILURE;
			}

			toWrite3 = HI_GetSystemTime( );
			if (fwrite(&index, 16, 1, pg->pFileTmp) != 1)// 16 is sizeof(index)
			{
				perror("############ fwrite  AVI audio index err \n");
         		return HI_FAILURE;
			}

			fflush(pg->pFile);
			
			wrote = HI_GetSystemTime( );

			if(wrote > wStart + 2000 )
			{
				printf("Audio write, start:%u, 2Write:%u-%u-%u, wrote:%u\n", wStart, toWrite1, toWrite2, toWrite3, wrote );
			}
			
		}

		break;
	default:
		break;
	}

	return HI_SUCCESS;
}

/**************************************************************************************************

*Copy right by HuNan GaoZhi Tech. ltld. 2016 - 20xx
*create by YuanZhenkun, email: yzhk1015@sina.com

***************************************************************************************************/
void* HI_Avi_vReadOpen(char *pFileName, int bufSize, HI_U64* audioPts, HI_U64* vedioPts, HI_U64* audioDeltaPts, HI_U64* vedioDeltaPts )
{
	HI_AVI_INFO_S *pg = NULL;
    if (NULL == pFileName)  return NULL;
	if (0 >= strlen(pFileName))
	{
		return NULL;
	}

	pg = (HI_AVI_INFO_S *) malloc(sizeof(HI_AVI_INFO_S));
	if (NULL == pg)  
	{
		return NULL;
	}

	memset( pg, 0x0, sizeof(HI_AVI_INFO_S));

	pg->m_pFrameBuffer = NULL;
	pg->m_pFrameBufferSize = 0;//800 * 1024;
	pg->currAudioPTS  = 0;
	pg->currVideoPTS  = 0;
	pg->deltaAudioPTS = 0;
	pg->deltaVideoPTS = 0;
	pg->TotalFrameNum = 0;
	pg->AudioFrameNum = 0;
	pg->VideoFrameNum = 0;
    pg->u32AudioToltalSize = 0;
	pg->m_bSeekAble   = HI_FALSE;

	pg->Audio.eAEncode       = AVI_AUDIO_UNKNOW;
	pg->Audio.lBitRate       = 0;
	pg->Audio.lBitsPerSample = 0;
	pg->Audio.lBlockAlign    = 0;
	pg->Audio.lChannels      = 0;
	pg->Audio.length         = 0;
	pg->Audio.lFrameFlag     = 0;
	pg->Audio.lSamplesPerSec = 0;
	pg->Audio.pReserved      = 0;

	pg->Video.eVEncode   = AVI_VIDEO_UNKNOW;
	pg->Video.lBitRate   = 0;
	pg->Video.lFrameRate = 0;
	pg->Video.lHeight    = 0;
	pg->Video.lWidth     = 0;

	pg->FileInfo.u32StreamNum = 0;
	pg->FileInfo.struDuration = 0;
	pg->FileInfo.s32AudioIndex = -1;
	pg->FileInfo.s32VideoIndex = -1;
	pg->FileInfo.bSeekEnable = HI_FALSE;
	
	pg->pFile = fopen((const char *)pFileName, "rb");
	if (NULL == pg->pFile )  
	{
		goto exit;
	}

	pg->pFileTmp = fopen((const char *)pFileName, "rb");
	if (NULL == pg->pFileTmp )  
	{
		goto exit;
	}

	// 解析文件头, 
	if (AVI_CreateReader(pg))
	{
		goto exit;
	}
	strcpy((char*)pg->FileInfo.u8FormatName, "avi");
    strcpy((char*)pg->FileInfo.u8FileName, (const char*)pFileName);

	(*audioPts) 	 = pg->currAudioPTS;
	(*vedioPts) 	 = pg->currVideoPTS;
	(*audioDeltaPts) = pg->deltaAudioPTS;
	(*vedioDeltaPts) = pg->deltaVideoPTS;

	//fseek(pg->pFileTmp, pg->m_IndexOffset, SEEK_SET);
	
	//printf("===REPLAY=== avi fp:0x%x, avi idx fp:0x%x, A/V start:0x%x, idx start:0x%x\n", pg->pFile, pg->pFileTmp, pg->m_DataOffet, pg->m_IndexOffset );
	//printf("===REPLAY===[%s, %d] avi fp:0x%x, avi idx fp:0x%x, fp postion:0x%x\n", __FILE__, __LINE__, pg->pFile, pg->pFileTmp, ftell(pg->pFile) );

	return (void *) pg;

exit:
	HI_Avi_ReadClose(pg);
	return NULL;
}


#if 0
HI_S32 HI_Avi_ReadLarge(void *handle, HI_CHAR* buffer, HI_CHAR** pOffset, HI_S32 bufSize, HI_S32* leftSize,HI_AVI_FRAME_S *frame)
{
	HI_U32 ChunkSize, ChunkType;
	//AVI_INDEXENTRY_S index = {0};
    HI_S32 Ret = HI_AVI_OK;
	AVI_CHUNKID_E ChunkID;
	HI_U32 StreamNum;

	HI_S32 adjustChunkSize = 0, readLen = 0;

	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;

	if (NULL == pg || NULL == frame)
	{
		return HI_AVI_ERR_READ_FRAME;
	}

	//printf("[yuanzhenkun] buffer:0x%x, offset:0x%x, capacity:%d, left:%d\n", buffer, *pOffset, bufSize, *leftSize);	
	if( (*leftSize) >= 8 )
	{
		ChunkType  = (*pOffset)[0];
		ChunkType += ((*pOffset)[1]<<8);
		ChunkType += ((*pOffset)[2]<<16);
		ChunkType += ((*pOffset)[3]<<24);
		(*pOffset)+= 4;

		ChunkSize  = (*pOffset)[0];
		ChunkSize += ((*pOffset)[1]<<8);
		ChunkSize += ((*pOffset)[2]<<16);
		ChunkSize += ((*pOffset)[3]<<24);
		(*pOffset)+= 4;

		adjustChunkSize = ChunkSize + (ChunkSize & 1);

		//printf("[yuanzhenkun] offset:0x%x, ChunkSize:0x%x, adjusted chunk size:0x%x, left size:0x%x\n", *pOffset, ChunkSize, adjustChunkSize, *leftSize);
		//printf("[yuanzhenkun] ChunkSize:0x%x, ChunkType:0x%x\n", ChunkSize, ChunkType);
		if( adjustChunkSize + 8 <= (*leftSize) ) // get a completely frame
		{
			(*leftSize) -= adjustChunkSize ;
			(*leftSize) -= 8;

			if (pg->m_currFrame >= pg->TotalFrameNum)
			{
				return HI_AVI_ERR_ENDFILE;
			}

			pg->m_currFrame++;
			Parse_Header_StreamData(ChunkType, &StreamNum, &ChunkID);

			//frame->pu8Data        = pg->m_pFrameBuffer;
			frame->u32Size       = ChunkSize;
			frame->u32StreamType = ChunkID;
			frame->u32StreamNum  = StreamNum;
			frame->u32OutPutFlag = 1;

			switch(ChunkID) 
		    {
		    case AVI_CHUNKID_VIDEO:
				//memcpy(pg->m_pFrameBuffer, *pOffset, adjustChunkSize );
				frame->pu8Data = *pOffset;
				(*pOffset) += adjustChunkSize;
				
				frame->u64Pts     = pg->currVideoPTS;
				frame->u64Dts     = 0;
				frame->u32Duration= (HI_U32)pg->deltaVideoPTS;
				pg->currVideoPTS += pg->deltaVideoPTS;

				frame->u32Flags   =  (ChunkType & 0x10)? 1: 0;

				Ret = adjustChunkSize;
				break;
			case AVI_CHUNKID_AUDIO:
				//memcpy(pg->m_pFrameBuffer, *pOffset, adjustChunkSize );
				frame->pu8Data = *pOffset;
				(*pOffset) += adjustChunkSize;
				
				frame->u64Pts     = pg->currAudioPTS;
				frame->u64Dts     = 0;
				frame->u32Duration= (HI_U32)pg->deltaAudioPTS;
				pg->currAudioPTS += pg->deltaAudioPTS;
				
				Ret = adjustChunkSize;
				break;
			case AVI_CHUNKID_RGB:
				//memcpy(pg->m_pFrameBuffer, *pOffset, adjustChunkSize );
				frame->pu8Data = *pOffset;
				(*pOffset) += adjustChunkSize;
				
				frame->u64Pts     = pg->currVideoPTS;
				frame->u64Dts     = 0;
				frame->u32Duration= (HI_U32)pg->deltaVideoPTS;
				pg->currVideoPTS += pg->deltaVideoPTS;
				
				Ret = adjustChunkSize;
				
				break;

			case AVI_CHUNKID_IDX1:
				Ret = -1;
				break;

			case AVI_CHUNKID_LIST:
				//Ret |= fseek(pg->pFile, 4, SEEK_CUR); 
				(*pOffset) += 4;
				(*leftSize)-= 4;
				Ret = HI_Avi_ReadLarge(handle, buffer, pOffset, bufSize, leftSize, frame);
				break;


			default:
		        //Ret |= fseek(pg->pFile, ChunkSize + (ChunkSize & 1), SEEK_CUR);   // skip 
		        (*pOffset) +=  adjustChunkSize;// ChunkSize + (ChunkSize & 1);
				(*leftSize)-=  adjustChunkSize;// ChunkSize + (ChunkSize & 1);
				Ret = HI_Avi_ReadLarge(handle, buffer, pOffset, bufSize, leftSize, frame);
			}
			
			//printf("[yuanzhenkun] HI_Avi_ReadFrame, current frame:%ld, PTS:%ld\n", pg->m_currFrame, frame->u64Pts);
			
			return Ret;
				
		}
		else
		{
			//can't get a complete frame from left buffer, read another large size data from avi .
			fseek( pg->pFile, -(*leftSize), SEEK_CUR ); 
			readLen = fread( buffer, 1, bufSize, pg->pFile);
			if(readLen == 0)
			{
				return HI_AVI_ERR_ENDFILE;
			}
			else if(readLen < 0)
			{
				return HI_AVI_ERR_READ_FRAME;
			}
			else if(readLen < adjustChunkSize )
			{
				return HI_AVI_ERR_ENDFILE;
			}

			(*leftSize) = readLen;
			(*pOffset)  = buffer;	
			
		}
		
	}
	else
	{
		//can't get a complete frame from left buffer, read another large size data from avi .
		fseek( pg->pFile, -(*leftSize), SEEK_CUR ); 
		readLen = fread( buffer, 1, bufSize, pg->pFile);
		if(readLen == 0)
		{
			return HI_AVI_ERR_ENDFILE;
		}
		else if(readLen < 0)
		{
			return HI_AVI_ERR_READ_FRAME;
		}
		else if(readLen < 8 )
		{
			return HI_AVI_ERR_ENDFILE;
		}

		(*leftSize) = readLen;
		(*pOffset)  = buffer;		
	}

	return HI_Avi_ReadLarge(handle, buffer, pOffset, bufSize, leftSize, frame);
		
}
#else

/**************************************************************************************************

*Copy right by HuNan GaoZhi Tech. ltld. 2016 - 20xx
*create by YuanZhenkun, email: yzhk1015@sina.com

***************************************************************************************************/
HI_S32 HI_Avi_ReadLarge( void *handle, HI_CHAR* buffer, HI_S32 bufSize, HI_S32 adjOffset )
{
	HI_S32 readLen = 0;
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;

	HI_S32 fpPos1 = 0,  fpPos2 = 0;

	if (NULL == pg )
	{
		return -1;
	}

	fpPos1 = ftell( pg->pFile);

	fseek( pg->pFile, adjOffset, SEEK_CUR );

	fpPos2 = ftell( pg->pFile);

	readLen = fread( buffer, 1, bufSize, pg->pFile);

	//printf("[%s, %d] ===REPLAY=== adjOffset:%d, read size:%d\n", __FILE__, __LINE__, adjOffset, readLen );
	//printf("===REPLAY===[%s, %d] avi fp:0x%x, avi idx fp:0x%x, fp postion:0x%x -> 0x%x\n", __FILE__, __LINE__, pg->pFile, pg->pFileTmp, fpPos1, fpPos2 );
	
	return readLen;		
}

#endif

/**************************************************************************************************

*Copy right by HuNan GaoZhi Tech. ltld. 2016 - 20xx
*create by YuanZhenkun, email: yzhk1015@sina.com
@descriptions: quick jump to Video Key Frame of the offset seconds

***************************************************************************************************/
HI_S32 HI_Avi_Jump2_Vedio( void *handle, HI_U32 offsetSecs, HI_U64 *currAudioPts, HI_U64 *currVedioPts )
{
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;	
	AVI_INDEXENTRY_S  aviIdxEntry;

	HI_S32 readLen = 0;
	HI_U32 seekOffset = 0;
	HI_S32 ret = HI_FAILURE;

	HI_U32 offsetFrames = ( pg->Video.lFrameRate + 50 ) * offsetSecs;// - 1;
	HI_U32 dataOffsetMove = 0;

	HI_U64 audioPts = *currAudioPts;
	HI_U64 vedioPts = *currVedioPts;
	
	const HI_U8 AVI_INDEX_ENTRY_SIZE = sizeof(AVI_INDEXENTRY_S);

	if (NULL == pg )
	{
		return HI_FAILURE;
	}

	fseek( pg->pFileTmp, pg->m_IndexOffset + offsetFrames * AVI_INDEX_ENTRY_SIZE, SEEK_SET);
	
	while( 1 )
	{
		readLen = fread( (HI_U8 *)&aviIdxEntry, 1, AVI_INDEX_ENTRY_SIZE, pg->pFileTmp ); //pFileTmp is the handle for AVI index reader
		if( readLen != AVI_INDEX_ENTRY_SIZE )
		{
			//assert(0);
			printf( "[%s, %d]read an idx1 Entry length:%d\n", __FILE__, __LINE__, readLen );
			break;
		}

		offsetFrames++;
		if( AVI_FOURCC_00dc == aviIdxEntry.ckid )
		{
						
			if( aviIdxEntry.dwFlags & 0x10 )//locate the Video Key frame
			{
				ret = HI_SUCCESS;

				printf("[%s, %d] offset secs:%lu, seeked video frames: %lu, data offset:0x%08x + 0x%08x = 0x%08x\n", 
					__FILE__, __LINE__, offsetSecs, offsetFrames, pg->m_DataOffet - 4, aviIdxEntry.dwChunkOffset, 
					pg->m_DataOffet + aviIdxEntry.dwChunkOffset - 4 );
				fseek( pg->pFile, pg->m_DataOffet + aviIdxEntry.dwChunkOffset - 4, SEEK_SET ); //pFile is the handle for AVI data reader
				fseek( pg->pFileTmp, -AVI_INDEX_ENTRY_SIZE, SEEK_CUR);
				break;
			}
			vedioPts += pg->deltaVideoPTS;
		}	
		else if( AVI_FOURCC_01wb == aviIdxEntry.ckid )
		{
			audioPts += pg->deltaAudioPTS;
		}
	}

	*currAudioPts = audioPts;
	*currVedioPts = vedioPts;
	
	return ret;	
	
}

HI_S32 HI_Avi_ReadIdx( void *handle, HI_CHAR* idxBuffer, HI_S32 bufSize, HI_S32 adjOffset )
{
	HI_S32 readLen = 0;
	HI_AVI_INFO_S *pg = (HI_AVI_INFO_S *) handle;

	if (NULL == pg )
	{
		return -1;
	}

	//printf("===REPLAY===[%s, %d] avi fp:0x%x, avi idx fp:0x%x\n", __FILE__, __LINE__, pg->pFile, pg->pFileTmp );
	
	fseek( pg->pFileTmp, adjOffset, SEEK_CUR );
	
	readLen = fread( idxBuffer, 1, bufSize, pg->pFileTmp );
	if(readLen == 0)
	{
		return HI_AVI_ERR_ENDFILE;
	}
	else if(readLen < 0)
	{
		return HI_AVI_ERR_READ_FRAME;
	}
	else if(readLen < 8 )
	{
		return HI_AVI_ERR_ENDFILE;
	}

	return readLen;
		
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


