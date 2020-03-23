/*
 * Copyright (c) 2017 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software and related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */
#ifndef INCLUDED_NVBOOT_BOOT_COMPONENT_H
#define INCLUDED_NVBOOT_BOOT_COMPONENT_H

#include "nvcommon.h"
#include "nvboot_se_aes.h"
#include "nvboot_se_rsa.h"
#include "nvboot_crypto_param.h"

#define MAX_COMPONENT_COUNT 4

/**
 *  Cut and paste from Secure Boot ISS.
 */
typedef struct {
       NvU8 VersionMajor;
       NvU8 VersionMinor;
       NvU8 RatchetLevel; //to support loader enforced ratchet mechanism
       NvU8 Reserved;
} NvBootComponentVersion; // 4 (0x4) bytes

//this structure is only used when the group/binary auth requires two stage signing
//that includes nvidia signing
//here we will dictate argument such as LoadDestination and EntryPoint so that Stage2
//(OEM) will not be able to change that
typedef struct {
       NvU8 BinaryMagic[4]; //indicate component type, such as “MB1B” for MB1
       NvU32 BinaryLen; //length of the component, will be sanitized before usage
       NvU32 LoadDestination; //the destination of the component to be copied to 
       NvU32 EntryPoint; //the EntryPoint (absolute value) to start execute the component
       NvBootComponentVersion Version;
       NvU8 Res[12];
       NvU8 Digest[32]; //SHA-256 hash of the component
} NvStage1Component; //64 (0x40) bytes

//this is the second stage structure for boot component
//this is used for oem signing, fields with the same name as NvStage1Component fields needs to have the
//same value.  
typedef struct {
       NvU8 BinaryMagic[4];
       NvU32 BinaryLen;
       NvU32 LoadDestination;
       NvU32 EntryPoint;
       NvBootComponentVersion Version;
       NvU32 StartPage; //location on disk
       NvU32 StartBlock; //off set of that location on disk?
       NvU8 Encrypted; 
       NvU8 EncryptionDerivationString[7];
       NvU8 Res[12];
       NvU8 Digest[32];
} NvStage2Component; //80 (0x50) bytes

//this is the main header.  The bottom section is signed by stage 1 (NV) and signature is stored in the middle part of the structure.  The middle and bottom part is signed by stage 2 (OEM) with signature kept in unsigned section. 
typedef struct {
       //=unsigned section= //2960 (0xB90) bytes
       NvU8 HeaderMagic[4]; //has to be "NVDA"
       NvU8 UnsignedPadding[12]; //align to aes blocksize 
       NvBootCryptoSignatures Stage2Signature; // 480 bytes, 32b for SHA2 hash, 384b for RSA, 64b for ECDSA/EdDSA
       NvBootPublicCryptoParameters Pcp; //1216 bytes
       NvU8 GenPadding[1248]; //1248 pad structure to 4k
  
       //=stage 2 signed section= //832 (0x340) bytes
       NvU8 Salt2[16]; //one block of salt
       NvU32 NumBinaries2; //Number of binary in this group
       NvU8 Stage2Padding[12];
       NvStage2Component Stage2Components[MAX_COMPONENT_COUNT]; //MAX_COMPONENT_COUNT defined to 4
       NvBootCryptoSignatures Stage1Signature; // 480 bytes
  
       //=stage 1 signed section //304 (0x130) bytes
       NvU8 Salt1[16];
       NvU8 ECID[16];
       NvU32 NumBinaries1;
       NvU8 Stage1Padding[12];
       NvStage1Component Stage1Components[MAX_COMPONENT_COUNT];  
} NvBootComponentHeader; //0x800 bytes (2k)




#endif //INCLUDED_NVBOOT_BOOT_COMPONENT_H
