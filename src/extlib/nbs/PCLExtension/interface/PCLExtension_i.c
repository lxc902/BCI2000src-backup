

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Apr 20 13:50:57 2010
 */
/* Compiler settings for PCLExtension.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPCLArgument,0xD43BB8BB,0x2BA3,0x492e,0xB3,0xAB,0x15,0x00,0x71,0xB8,0x79,0xA9);


MIDL_DEFINE_GUID(IID, IID_IPCLWavefileLock,0xE0934AAF,0x8832,0x44cf,0xA4,0x90,0x06,0xF9,0x17,0x2F,0xC7,0xBB);


MIDL_DEFINE_GUID(IID, IID_IPCLWavefile,0xA60D18A4,0x8C3F,0x44f0,0x97,0xFA,0xB3,0x99,0xE5,0x6F,0xFD,0xC2);


MIDL_DEFINE_GUID(IID, IID_IPCLGraphicSurfaceLock,0x44E35511,0xBC78,0x40a5,0xB7,0x13,0x0E,0x90,0x77,0x93,0x3E,0x8D);


MIDL_DEFINE_GUID(IID, IID_IPCLGraphicSurface,0xA96615CA,0x28B1,0x4958,0x93,0x29,0x7F,0x1A,0x45,0x36,0x10,0xB7);


MIDL_DEFINE_GUID(IID, IID_IPCLArray,0xDD64D836,0xF3DF,0x4097,0xA1,0x2A,0x45,0xEF,0x44,0x1B,0x2F,0xD0);


MIDL_DEFINE_GUID(IID, IID_IPCLStringArray,0xCEB1D686,0xB7EC,0x44bc,0x8F,0xDA,0x87,0x0C,0x50,0x88,0x40,0x88);


MIDL_DEFINE_GUID(IID, IID_IPCLIntArray,0x6602A29A,0x38D8,0x4c3e,0xA9,0x80,0xF0,0x4A,0x0D,0xD6,0x48,0x95);


MIDL_DEFINE_GUID(IID, IID_IPCLDoubleArray,0xCA5BDA29,0xB722,0x4518,0xAF,0x47,0x26,0xC9,0xF8,0x62,0x87,0x52);


MIDL_DEFINE_GUID(IID, IID_IPCLBoolArray,0xFF2B4A6F,0x7F73,0x41b4,0xA6,0x96,0xF2,0x8C,0x1F,0x31,0x47,0xED);


MIDL_DEFINE_GUID(IID, IID_IPCLString,0x023D3759,0x685A,0x4c63,0x85,0xB3,0x9A,0x50,0xD5,0x45,0xE5,0x65);


MIDL_DEFINE_GUID(IID, IID_IPCLInt,0xCF00C2BD,0x1595,0x4c6d,0xB2,0xC9,0x8E,0xB8,0x8B,0x79,0xA0,0x78);


MIDL_DEFINE_GUID(IID, IID_IPCLDouble,0xC41A6728,0x6307,0x4846,0x88,0x3C,0x0C,0x8B,0xF4,0xAA,0xCA,0x17);


MIDL_DEFINE_GUID(IID, IID_IPCLBool,0x525A6207,0xB78E,0x47b1,0xAB,0x8E,0x4D,0xED,0x01,0x7B,0x29,0x9D);


MIDL_DEFINE_GUID(IID, IID_IPCLArguments,0xB8D5BA56,0x01D5,0x4237,0x92,0xA1,0x73,0xC9,0x72,0xC8,0x47,0x0B);


MIDL_DEFINE_GUID(IID, IID_IPCLObject,0x83CE0178,0xFAE2,0x4734,0x8A,0x90,0x31,0x38,0x95,0x43,0xE4,0xAE);


MIDL_DEFINE_GUID(IID, IID_IPCLExtensionObject,0x0DECE6D3,0x85AE,0x4d3c,0x95,0x3A,0x15,0xA5,0x55,0x04,0xC9,0xE6);


MIDL_DEFINE_GUID(IID, IID_IPCLMethod,0xB74B2585,0x4C45,0x4747,0xA2,0x97,0x9E,0xD8,0x41,0xF4,0x73,0x1D);


MIDL_DEFINE_GUID(IID, IID_IPCLMethodList,0x721705E4,0x275F,0x4aed,0xBF,0xA9,0x69,0x57,0xE4,0x0B,0xDE,0x4F);


MIDL_DEFINE_GUID(IID, IID_IPCLType,0x12349DAF,0xFD20,0x4ba6,0xB5,0x00,0x0F,0xC8,0xE0,0xE4,0x9A,0x0B);


MIDL_DEFINE_GUID(IID, IID_IPCLLibrary,0x5483302B,0x931F,0x4506,0xA9,0xD0,0x30,0xF5,0xC2,0x23,0x48,0xCB);


MIDL_DEFINE_GUID(IID, LIBID_PCLExtension,0x6C8AA9C3,0xD6D1,0x46ba,0x8E,0x56,0xFD,0xAF,0x36,0xAF,0xA8,0xDE);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Apr 20 13:50:57 2010
 */
/* Compiler settings for PCLExtension.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPCLArgument,0xD43BB8BB,0x2BA3,0x492e,0xB3,0xAB,0x15,0x00,0x71,0xB8,0x79,0xA9);


MIDL_DEFINE_GUID(IID, IID_IPCLWavefileLock,0xE0934AAF,0x8832,0x44cf,0xA4,0x90,0x06,0xF9,0x17,0x2F,0xC7,0xBB);


MIDL_DEFINE_GUID(IID, IID_IPCLWavefile,0xA60D18A4,0x8C3F,0x44f0,0x97,0xFA,0xB3,0x99,0xE5,0x6F,0xFD,0xC2);


MIDL_DEFINE_GUID(IID, IID_IPCLGraphicSurfaceLock,0x44E35511,0xBC78,0x40a5,0xB7,0x13,0x0E,0x90,0x77,0x93,0x3E,0x8D);


MIDL_DEFINE_GUID(IID, IID_IPCLGraphicSurface,0xA96615CA,0x28B1,0x4958,0x93,0x29,0x7F,0x1A,0x45,0x36,0x10,0xB7);


MIDL_DEFINE_GUID(IID, IID_IPCLArray,0xDD64D836,0xF3DF,0x4097,0xA1,0x2A,0x45,0xEF,0x44,0x1B,0x2F,0xD0);


MIDL_DEFINE_GUID(IID, IID_IPCLStringArray,0xCEB1D686,0xB7EC,0x44bc,0x8F,0xDA,0x87,0x0C,0x50,0x88,0x40,0x88);


MIDL_DEFINE_GUID(IID, IID_IPCLIntArray,0x6602A29A,0x38D8,0x4c3e,0xA9,0x80,0xF0,0x4A,0x0D,0xD6,0x48,0x95);


MIDL_DEFINE_GUID(IID, IID_IPCLDoubleArray,0xCA5BDA29,0xB722,0x4518,0xAF,0x47,0x26,0xC9,0xF8,0x62,0x87,0x52);


MIDL_DEFINE_GUID(IID, IID_IPCLBoolArray,0xFF2B4A6F,0x7F73,0x41b4,0xA6,0x96,0xF2,0x8C,0x1F,0x31,0x47,0xED);


MIDL_DEFINE_GUID(IID, IID_IPCLString,0x023D3759,0x685A,0x4c63,0x85,0xB3,0x9A,0x50,0xD5,0x45,0xE5,0x65);


MIDL_DEFINE_GUID(IID, IID_IPCLInt,0xCF00C2BD,0x1595,0x4c6d,0xB2,0xC9,0x8E,0xB8,0x8B,0x79,0xA0,0x78);


MIDL_DEFINE_GUID(IID, IID_IPCLDouble,0xC41A6728,0x6307,0x4846,0x88,0x3C,0x0C,0x8B,0xF4,0xAA,0xCA,0x17);


MIDL_DEFINE_GUID(IID, IID_IPCLBool,0x525A6207,0xB78E,0x47b1,0xAB,0x8E,0x4D,0xED,0x01,0x7B,0x29,0x9D);


MIDL_DEFINE_GUID(IID, IID_IPCLArguments,0xB8D5BA56,0x01D5,0x4237,0x92,0xA1,0x73,0xC9,0x72,0xC8,0x47,0x0B);


MIDL_DEFINE_GUID(IID, IID_IPCLObject,0x83CE0178,0xFAE2,0x4734,0x8A,0x90,0x31,0x38,0x95,0x43,0xE4,0xAE);


MIDL_DEFINE_GUID(IID, IID_IPCLExtensionObject,0x0DECE6D3,0x85AE,0x4d3c,0x95,0x3A,0x15,0xA5,0x55,0x04,0xC9,0xE6);


MIDL_DEFINE_GUID(IID, IID_IPCLMethod,0xB74B2585,0x4C45,0x4747,0xA2,0x97,0x9E,0xD8,0x41,0xF4,0x73,0x1D);


MIDL_DEFINE_GUID(IID, IID_IPCLMethodList,0x721705E4,0x275F,0x4aed,0xBF,0xA9,0x69,0x57,0xE4,0x0B,0xDE,0x4F);


MIDL_DEFINE_GUID(IID, IID_IPCLType,0x12349DAF,0xFD20,0x4ba6,0xB5,0x00,0x0F,0xC8,0xE0,0xE4,0x9A,0x0B);


MIDL_DEFINE_GUID(IID, IID_IPCLLibrary,0x5483302B,0x931F,0x4506,0xA9,0xD0,0x30,0xF5,0xC2,0x23,0x48,0xCB);


MIDL_DEFINE_GUID(IID, LIBID_PCLExtension,0x6C8AA9C3,0xD6D1,0x46ba,0x8E,0x56,0xFD,0xAF,0x36,0xAF,0xA8,0xDE);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

