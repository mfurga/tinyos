#pragma once

#if !defined(__i386__) && !defined(__x86_64__)
  #error The header can be used only for x86 architectures.
#endif

#define COM1  0x3f8
#define COM2  0x2f8
#define COM3  0x3e8
#define COM4  0x2e8

#define X86_EXP_DE         0
#define X86_EXP_DB         1
#define X86_EXP_NMI        2
#define X86_EXP_BP         3
#define X86_EXP_OF         4
#define X86_EXP_BR         5
#define X86_EXP_UD         6
#define X86_EXP_NM         7
#define X86_EXP_DF         8
#define X86_EXP_CSO        9
#define X86_EXP_TS        10
#define X86_EXP_NP        11
#define X86_EXP_SS        12
#define X86_EXP_GP        13
#define X86_EXP_PF        14
#define X86_EXP_15        15  /* reserved */
#define X86_EXP_MF        16
#define X86_EXP_AC        17
#define X86_EXP_MC        18
#define X86_EXP_XM        19
#define X86_EXP_VR        20
#define X86_EXP_CP        21
#define X86_EXP_22        22  /* reserved */
#define X86_EXP_23        23  /* reserved */
#define X86_EXP_24        24  /* reserved */
#define X86_EXP_25        25  /* reserved */
#define X86_EXP_26        26  /* reserved */
#define X86_EXP_27        27  /* reserved */
#define X86_EXP_28        28  /* reserved */
#define X86_EXP_29        29  /* reserved */
#define X86_EXP_30        30  /* reserved */
#define X86_EXP_31        31  /* reserved */
