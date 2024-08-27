// This whole file is crap. It will have to be removed. [khe]



#ifndef __MX_TYPES_H__
#define __MX_TYPES_H__



typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef __int64 s64;
typedef unsigned __int64 u64;

namespace mx {
  
  typedef unsigned int flags;
  
/*
  enum EVideoFormat { eNoVideo = 0,
                      eMxPEG   = 1,
                      eMJPG    = 2
                    };
  enum EAudioFormat { eNoAudio = 0
                    };
*/
/*
  struct FrameId {
	  u64 Timestamp;
	  u64 Seqno;
	  u64 SeqnoMax;
  };

  struct FrameInfo {
	  int iWidth;
	  int iHeight;
	  FrameId sFrameId;
  };
*/
};

#include <time.h>
#if _MSC_VER < 1400	// valid for VC6 and VS2003 
typedef time_t __time32_t;
#define _localtime32(t) localtime(t)
#define _time32(t) time(t)
#define _mktime32(t) mktime(t)
#endif	// #if _MSC_VER < 1400	// valid for VC6 and VS2003 

// Die vielen Klammern müssen sein!
#define DWORDlessOrEqu(k,g) ( (((g)-(k))&0x80000000)==0 )



#endif // __MX_TYPES_H__


