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
    _UnknownDeviceIdentifier = 0,
    
    // Device Identifier Objects
    _ADR, //Object that evaluates to a device’s address on its parent bus.
    _CID, //Object that evaluates to a device’s Plug and Play-compatible ID list.
    _DDN, //Object that associates a logical software name (for example, COM1) with a device.
    _HID, //Object that evaluates to a device’s Plug and Play hardware ID.
    _MLS, //Object that provides a human readable description of a device in multiple languages.
    _PLD, //Object that provides physical location description information.
    _SUN, //Object that evaluates to the slot-unique ID number for a slot.
    _STR, //Object that contains a Unicode identifier for a device.
    _UID, //Object that specifies a device’s unique persistent ID, or a control method that generates it.
    
    
    // Device Configuration Objects
    
    _CRS, //Object that specifies a device’s current resource settings, or a control method that generates such an object.
    _DIS, //Control method that disables a device.
    _DMA, //Object that specifies a device’s current resources for DMA transactions.
    _FIX, //Object used to provide correlation between the fixed-hardware register blocks defined in the FADT and the devices that implement these fixed-hardware registers.
    _GSB, //Object that provides the Global System Interrupt Base for a hot-plugged I/O APIC device.
    
    _HPP, // Object that specifies the cache-line size, latency timer, SERR enable, and PERR enable values to be used when configuring a PCI device inserted into a hot-plug slot or initial configuration of a PCI device at system boot.
    _HPX, //Object that provides device parameters when configuring a PCI device inserted into a hot-plug slot or initial configuration of a PCI device at system boot. Supersedes _HPP.
    _MAT, //Object that evaluates to a buffer of MADT APIC Structure entries.
    _OSC, //An object OSPM evaluates to convey specific software support / capabilities to the platform allowing the platform to configure itself appropriately.
    _PRS, //An object that specifies a device’s possible resource settings, or a control method that generates such an object.
    _PRT, //Object that specifies the PCI interrupt routing table.
    _PXM, //Object that specifies a proximity domain for a device.
    _SLI, //Object that provides updated distance information for a system locality.
    _SRS, //Control method that sets a device’s settings.
    
} DeviceIdentifiers;


//17.5.44 Field (Declare Field Objects)

typedef enum
{
   SystemMemory         =  0,
   SystemIO             =  1,
   PCI_Config           =  2,
   EmbeddedControl      =  3,
   SMBus                =  4,
   CMOS                 =  5,
   PCIBARTarget         =  6,
    
    //In addition, OEMs may define Operation Regions types 0x80 to 0xFF.
    
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
    
    ACPIObject_Type_NumericValue,
    ACPIObject_Type_StringValue,
    
    ACPIObject_Type_Package,
    //ACPIObject_Type_DWord, // should remove this one
    
}ACPIObject_Type;

//17.5.26 DefinitionBlock
typedef struct
{
    char    tableSignature[5]; // 4 chars
    ACPIDWord tableLength; // Length of the table in bytes including the block header.
    uint8_t complianceRevision;
    uint8_t tableCheckSum; // Byte checksum of the entire table.
    char    OEMId[6];
    char    tableId[9]; // 8 + 1 NULL
    
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
    
    LargeResourceItemsType_ReservedStart                        = 0x0C,
    LargeResourceItemsType_ReservedEnd                          = 0x7F
    
} LargeResourceItemsType;


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
    
    uint8_t resourceType;
    
    uint8_t typeSpecificFlags;
    uint64_t addrSpaceGranularity;
    uint64_t addrRangeMin;
    uint64_t addrRangeMax;
    uint64_t addrTranslationOffset;
    uint64_t addrTranslationLength;
    uint8_t ressourceSourceIndex;
    uint8_t ressourceSource;
    
    uint8_t maf:1;
    uint8_t mif:1;
    
    // Decode Type, _DEC: 1 This bridge subtractively decodes this address (top level bridges only) / 0 This bridge positively decodes this address
    uint8_t decodeType:1;
    
    // Consumer/Producer: 1–This device consumes this resource /  0–This device produces and consumes this resource
    uint8_t isConsumer:1;
    
} AddressSpaceDescriptor;


//6.4.3.5.3 Word Address Space Descriptor
typedef struct
{
    uint8_t ressourceType;

    uint8_t generalFlags;
    uint8_t maf:1;
    uint8_t mif:1;
    uint8_t decodeType:1;
    uint8_t isConsumer:1;


    uint8_t typeSpecificFlags;
    
    uint8_t  ressourceSourceIndex;
    uint16_t addrSpaceGranularity;
    uint16_t addrRangeMin;
    uint16_t addrRangeMax;
    uint16_t addrTranslationOffset;
    uint16_t addrTranslationLength;
    
    
} WordAddressSpaceDescriptor;

//6.4.3.5.5 Resource Type Specific Flags
typedef struct
{
    uint8_t TTP:1; // 1 TypeTranslation / 0 TypeStatic
    
    /*
     0 AddressRangeMemory
     1 AddressRangeReserved
     2 AddressRangeACPI
     3 AddressRangeNVS
     */
    uint8_t MTP:2;
    
    /*
     0 The memory is non-cacheable.
     1 The memory is cacheable.
     2 The memory is cacheable and supports write combining.
     3 The memory is cacheable and prefetchable.
     */
    uint8_t MEM:2;
    
    /*
     1 This memory range is read-write.
     0 This memory range is read-only.
     */
    uint8_t RW:1;
} ResourceType0Flags;


//6.4.3.5.2 DWord Address Space Descriptor
typedef struct
{
    uint8_t  resourceType;
    uint8_t  generalFlags;
    
    uint8_t maf:1;
    uint8_t mif:1;
    uint8_t decodeType:1;
    uint8_t isConsumer:1;
    
    uint8_t  typeSpecificFlags;
    
    uint32_t addrSpaceGranularity;
    uint32_t addrRangeMin;
    uint32_t addrRangeMax;
    uint32_t addrTranslationOffset;
    uint32_t addrTranslationLength;
    
    ResourceType0Flags specificFlags;
    
    
} DWordAddressSpaceDescriptor;



//6.4.3.5.1 QWord Address Space Descriptor
typedef struct
{
    
    uint8_t  resourceType;
    uint8_t  generalFlags;
    
    uint8_t maf:1;
    uint8_t mif:1;
    uint8_t decodeType:1;
    uint8_t isConsumer:1;
    
    uint8_t  typeSpecificFlags;
    
    uint64_t addrSpaceGranularity;
    uint64_t addrRangeMin;
    uint64_t addrRangeMax;
    uint64_t addrTranslationOffset;
    uint64_t addrTranslationLength;
    
    ResourceType0Flags specificFlags;
    
    
} QWordAddressSpaceDescriptor;


typedef struct
{
    
    //uint8_t information;
    uint16_t rangeMinBaseAddr;
    uint16_t rangeMaxBaseAddr;
    uint8_t baseAlign;
    uint8_t rangeLen;
    uint8_t isDecoder:1;
} IOPortDescriptor;


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
    
    uint8_t argCount;
    uint8_t syncLevel;
    uint8_t serializeFlag:1;
    
    
    
    const uint8_t* bodyDef;
    size_t bodySize;
    
} ACPIMethod;


// Table 17-18 OperationRegion Region Types and Access Types
typedef struct
{
    char name[6]; // 4 chars max + null terminaison
    uint64_t space;
    uint64_t offset;
    uint64_t length;
    
} ACPIOperationRegion;

typedef struct
{
    char name[5]; // 4 + NULL byte
    uint8_t accessType:4;
    /*
     0 AnyAcc
     1 ByteAcc
     2 WordAcc
     3 DWordAcc
     4 QWordAcc
     5 BufferAcc
     6 Reserved
     */
    uint8_t lockRule:1; /* 0 NoLock 1 Lock */
    uint8_t updateRule:2; /*0 Preserve 1 WriteAsOnes 2 WriteAsZeros */
    
    
    
    
    char valueName[5]; // 4 + NULL byte
    uint8_t value;
    
    uint8_t offset;
    
} ACPIField;



typedef struct
{
    uint8_t numElements;
    const uint8_t* buffer;
    size_t bufSize;
    
} ACPIPackage;
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




/* Extra types */

// 9.10.2 _FDI (Floppy Disk Information)
typedef struct
{
    uint8_t  driveNumber;               //BYTE
    uint8_t  deviceType;                //BYTE
    uint16_t maximumCylinderNumber;     //WORD
    uint16_t maximumSectorNumber;       //WORD
    uint16_t maximumHeadNumber;         //WORD
    uint8_t  disk_specify_1;            //BYTE
    uint8_t  disk_specify_2;            //BYTE
    uint8_t  disk_motor_wait;           //BYTE
    uint8_t  disk_sector_siz;           //BYTE
    uint8_t  disk_eot;                  //BYTE
    uint8_t  disk_rw_gap;               //BYTE
    uint8_t  disk_dtl;                  //BYTE
    uint8_t  disk_formt_gap;            //BYTE
    uint8_t  disk_fill;                 //BYTE
    uint8_t  disk_head_sttl;            //BYTE
    uint8_t  disk_motor_strt;           //BYTE
    
} FloppyDiskInformation;
