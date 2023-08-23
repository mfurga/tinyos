#pragma once

#include "common.h"

#define VGA_TEXT_MODE_80_25_COLOR 0x03

#define VBE_MODE_USE_BANKED_FB 0x0000
#define VBE_MODE_USE_LINEAR_FB 0x4000

struct VbeFarPtr {
  u16 off;
  u16 seg;
} PACKED;

struct VbeInfoBlock {
  char VbeSignature[4];     /* VBE Signature */
  u16 VbeVersion;           /* VBE Version */  
  struct VbeFarPtr OemStringPtr; /* VbeFarPtr to OEM String */
  u8 Capabilities[4];       /* Capabilities of graphics controller */
  struct VbeFarPtr VideoModePtr; /* VbeFarPtr to VideoModeList */
  u16 TotalMemory;          /* Number of 64kb memory blocks */

  /* VBE 2.0+ */
  u16 OemSoftwareRev;       /* VBE implementation Software revision */
  u32 OemVendorNamePtr;     /* VbeFarPtr to Vendor Name String */
  u32 OemProductNamePtr;    /* VbeFarPtr to Product Name String */
  u32 OemProductRevPtr;     /* VbeFarPtr to Product Revision String */
  u8 _reserved[222];        /* Reserved for VBE implementation scratch area */
  u8 OemData[256];          /* Data Area for OEM Strings */
} PACKED;

struct VbeModeInfoBlock {
  u16 ModeAttributes;       /* mode attributes */
  u8 WinAAttributes;        /* window A attributes */
  u8 WinBAttributes;        /* window B attributes */
  u16 WinGranularity;       /* window granularity */
  u16 WinSize;              /* window size */
  u16 WinASegment;          /* window A start segment */
  u16 WinBSegment;          /* window B start segment */
  struct VbeFarPtr WinFuncPtr;  /* real mode pointer to window function */
  u16 BytesPerScanLine;     /* bytes per scan line */

  /* VBE 1.2+ */
  u16 XResolution;          /* horizontal resolution in pixels or characters */
  u16 YResolution;          /* vertical resolution in pixels or characters */
  u8 XCharSize;             /* character cell width in pixels */
  u8 YCharSize;             /* character cell height in pixels */
  u8 NumberOfPlanes;        /* number of memory planes */
  u8 BitsPerPixel;          /* bits per pixel */
  u8 NumberOfBanks;         /* number of banks */
  u8 MemoryModel;           /* memory model type */
  u8 BankSize;              /* bank size in KB */
  u8 NumberOfImagePages;    /* number of images */
  u8 _reserved0;            /* reserved for page function */

  /* Direct Color fields */
  u8 RedMaskSize;           /* size of direct color red mask in bits */
  u8 RedFieldPosition;      /* bit position of lsb of red mask */
  u8 GreenMaskSize;         /* size of direct color green mask in bits */
  u8 GreenFieldPosition;    /* bit position of lsb of green mask */
  u8 BlueMaskSize;          /* size of direct color blue mask in bits */
  u8 BlueFieldPosition;     /* bit position of lsb of blue mask */
  u8 RsvdMaskSize;          /* size of direct color reserved mask in bits */
  u8 RsvdFieldPosition;     /* bit position of lsb of reserved mask */
  u8 DirectColorModeInfo;   /* direct color mode attributes */

  /* VBE 2.0+ */
  u32 PhysBasePtr;
  u32 _reserved1;           /* Reserved - always set to 0 */
  u16 _reserved2;           /* Reserved - always set to 0 */

  /* VBE 3.0+ */
  u16 LinBytesPerScanLine;  /* bytes per scan line for linear modes */
  u8 BnkNumberOfImagePages; /* number of images for banked modes */
  u8 LinNumberOfImagePages; /* number of images for linear modes */
  u8 LinRedMaskSize;        /* size of direct color red mask (linear modes) */
  u8 LinRedFieldPosition;   /* */
  u8 LinGreenMaskSize;      /* size of direct color green mask (linear modes) */
  u8 LinGreenFieldPosition; /* bit position of lsb of green mask (linear modes) */
  u8 LinBlueMaskSize;       /* size of direct color blue mask (linear modes) */
  u8 LinBlueFieldPosition;  /* bit position of lsb of blue mask (linear modes) */
  u8 LinRsvdMaskSize;       /* size of direct color reserved mask (linear modes) */
  u8 LinRsvdFieldPosition;  /* bit position of lsb of reserved mask (linear modes) */
  u32 MaxPixelClock;        /* maximum pixel clock (in Hz) for graphics mode */

  u8 _reserved3[189];       /* remainder of ModeInfoBlock */
} PACKED;

void fetch_video_modes(void);

void print_video_modes(void);

void print_video_mode_info(void);

bool vbe_get_info_block(struct VbeInfoBlock *ib);

bool vbe_get_mode_info_block(u16 mode, struct VbeModeInfoBlock *mib);

void vbe_set_video_mode(void);

void vga_set_video_mode(u8 mode);

bool select_video_mode(u8 idx);
