#include "common.h"

struct fat_ptr_16 {
  u16 off;
  u16 seg;
} PACKED;

struct vbe_info_block {
  char VbeSignature[4];
  u16 VbeVersion;
  struct fat_ptr_16 OemStringPtr;
  u8 Capabilities[4];
  struct fat_ptr_16 VideoModePtr;
  u16 TotalMemory;

  /* VBE 2.0+ */
  u16 OemSoftwareRev;
  u32 OemVendorNamePtr;
  u32 OemProductNamePtr;
  u32 OemProductRevPtr;
  u8 reserved[222];
  u8 OemData[256];
} PACKED;

struct vbe_mode_info_block {
  u16 ModeAttributes;
  u8 WinAAttributes;
  u8 WinBAttributes;
  u16 WinGranularity;
  u16 WinSize;
  u16 WinASegment;
  u16 WinBSegment;
  struct fat_ptr_16 WinFuncPtr;
  u16 BytesPerScanLine;

  /* VBE 1.2+ */
  u16 XResolution;
  u16 YResolution;
  u8 XCharSize;
  u8 YCharSize;
  u8 NumberOfPlanes;
  u8 BitsPerPixel;
  u8 NumberOfBanks;
  u8 MemoryModel;
  u8 BankSize;
  u8 NumberOfImagePages;
  u8 Reserved0;

  u8 RedMaskSize, RedFieldPosition;
  u8 GreenMaskSize, GreenFieldPosition;
  u8 BlueMaskSize, BlueFieldPosition;
  u8 RsvdMaskSize, RsvdFieldPosition;
  u8 DirectColorModeInfo;

  /* VBE 2.0+ */
  u32 PhysBasePtr;
  u32 Reserved1;
  u16 Reserved2;

  /* VBE 3.0+ */
  u16 LinBytesPerScanLine;
} PACKED;

bool vbe_get_info_block(struct vbe_info_block *ib);

bool vbe_get_mode_info(u16 mode);

