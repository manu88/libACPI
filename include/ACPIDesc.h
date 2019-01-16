/*
 * This file is part of the libACPI project
 * Copyright (c) 2018 Manuel Deneu.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

typedef int32_t  ACPIDWord;

typedef enum
{
    HID,
    ADR,
    CRS,
    PRW,
    BBN,
    UID
    
} DefaultNames;

typedef enum
{
   SystemMemory         =  0,
   SystemIO             =  1,
   PCI_Config           =  2,
   EmbeddedControl      =  3,
   SMBus                =  4,
   CMOS                 =  5,
   PCIBARTarget         =  6,
    
} ACPIRegionType;

typedef enum
{
    ACPIObject_Type_Unknown = 0,
    ACPIObject_Type_Root,
    ACPIObject_Type_Device,
    ACPIObject_Type_DefinitionBlock,
    ACPIObject_Type_Name,
    ACPIObject_Type_Buffer,
    ACPIObject_Type_OperationRegion,
    ACPIObject_Type_VarPackage,
    ACPIObject_Type_Scope,
    ACPIObject_Type_Field,
    ACPIObject_Type_Method,
    
    
    ACPIObject_NumericValue,
    //ACPIObject_Type_DWord, // should remove this one
    
}ACPIObject_Type;


typedef struct
{
    char    tableSignature[5]; // 4 chars
    ACPIDWord tableLength; // Length of the table in bytes including the block header.
    uint8_t complianceRevision;
    uint8_t tableCheckSum; // Byte checksum of the entire table.
    char    OEMId[6];
    char    tableId[8];
    
    ACPIDWord OEMRev;
    
    ACPIDWord creatorID; //// Vendor ID of the ASL compiler
    /*
     For compatibility with ACPI versions before ACPI 2.0, the bit width of Integer objects is dependent on the ComplianceRevision. If the ComplianceRevision is less than 2, all integers are restricted to 32 bits. Otherwise, full 64-bit integers are used.
     */
} ACPIDefinitionBlock;


/*
typedef struct
{
    char id[5]; // 4 bytes + null ; 00000 denotes an empty name. empty names shall be at the end, ie no gaps in the list
    
    union
    {
        ACPIDWord word;
    } value;
    
} ACPIName;
*/

typedef enum
{
    LargeResourceItemsType_Reserved0                            = 0x00,
    LargeResourceItemsType_BitMemoryRangeDescriptor24           = 0x01,
    LargeResourceItemsType_GenericRegisterDescriptor            = 0x02,
    LargeResourceItemsType_Reserved1                            = 0x03,
    LargeResourceItemsType_VendorDefinedDescriptor              = 0x04,
    LargeResourceItemsType_MemoryRangeDescriptor32              = 0x05,
    LargeResourceItemsType_FixedLocationMemoryRangeDescriptor32 = 0x06,
    LargeResourceItemsType_DWORDAddressSpaceDescriptor          = 0x07,
    LargeResourceItemsType_WORDAddressSpaceDescriptor           = 0x08,
    LargeResourceItemsType_ExtendedIRQDescriptor                = 0x09,
    LargeResourceItemsType_QWORDAddressSpaceDescriptor          = 0x0A,
    LargeResourceItemsType_ExtendedAddressSpaceDescriptor       = 0x0B,
    
    LargeResourceItemsType_ReservedStart                        =  0x0C,
    LargeResourceItemsType_ReservedEnd                          =  0x7F
    
}LargeResourceItemsType;


typedef enum // 6.4.2 Small Resource Data Type
{
    //SmallResourceItemsType_Reserved = 0x00-0x03
    SmallResourceItemsType_IRQFormatDescriptor                = 0x04,
    SmallResourceItemsType_DMAFormatDescriptor                = 0x05,
    SmallResourceItemsType_StartDependentFunctionsDescriptor  = 0x06,
    SmallResourceItemsType_EndDependentFunctionsDescriptor    = 0x07,
    SmallResourceItemsType_IOPortDescriptor                   = 0x08,
    SmallResourceItemsType_FixedLocationIOPortDescriptor      = 0x09,
    //SmallResourceItemsType_Reserved = 0x0A–0x0D,
    SmallResourceItemsType_VendorDefinedDescriptor            = 0x0E,
    SmallResourceItemsType_EndTagDescriptor                   = 0x0F,
    
} SmallResourceItemsType;

// 6.4.3.5.1 QWord Address Space Descriptor
typedef struct
{
    /*
     ressource types :
     0 Memory range
     1 I/O range
     2 Bus number range
     3–191 Reserved
     192-255 Hardware Vendor Defined
     */
    
    uint8_t ressourceType;
    
    uint8_t typeSpecificFlags;
    uint64_t  addrSpaceGranularity;
    uint64_t  addrRangeMin;
    uint64_t  addrRangeMax;
    uint64_t  addrTranslationOffset;
    uint64_t  addrTranslationLength;
    uint8_t ressourceSourceIndex;
    uint8_t ressourceSource;
    
    uint8_t maf:1;
    uint8_t mif:1;
    
    // Decode Type, _DEC: 1 This bridge subtractively decodes this address (top level bridges only) / 0 This bridge positively decodes this address
    uint8_t decodeType:1;
    
    // Consumer/Producer: 1–This device consumes this resource /  0–This device produces and consumes this resource
    uint8_t isConsumer:1;
    
} AddressSpaceDescriptor;


typedef struct // 6.4.3.4
{
    uint32_t rangeBaseAddr;
    uint32_t rangeLength;
    uint8_t writeStatus : 1; // 1 writeable (read/write) / 0 non-writeable (read-only))
    
} MemoryRangeDescriptor32;


typedef struct
{
    char name[5]; // 4 chars max + null terminaison
                //Warning: this is a non-terminated String! ; 00000 denotes an invalid device. invalid devices shall be at the end, ie no gaps in the list
    
}ACPIDevice;


typedef struct
{
    char name[5]; // 4 chars max + null terminaison
    uint64_t space;
    uint64_t offset;
    uint64_t length;
    
} ACPIOperationRegion;

typedef struct
{
    char name[5];
    
    
} ACPIField;
/*
static inline size_t ACPIDeviceGetNamesCount(const ACPIDevice* dev)
{
    size_t n = 0;
    while (n<ACPIDeviceMaxNames)
    {
        if (dev->names[n].id[0] == 0)
        {
            break;
        }
        n++;
    }
    
    return n;
}
*/



