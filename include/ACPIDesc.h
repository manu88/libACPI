//
//  ACPIDesc.h
//  KernelTaskV1
//
//  Created by Manuel Deneu on 26/12/2018.
//  Copyright © 2018 Manuel Deneu. All rights reserved.
//

#ifndef ACPIDesc_h
#define ACPIDesc_h

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
    ACPIObject_Type_Unknown = 0,
    ACPIObject_Type_Root,
    ACPIObject_Type_Device,
    ACPIObject_Type_Name,
    ACPIObject_Type_OperationRegion,
    ACPIObject_Type_Scope,
    
    
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



typedef struct
{
    char id[5]; // 4 bytes + null ; 00000 denotes an empty name. empty names shall be at the end, ie no gaps in the list
    
    union
    {
        ACPIDWord word;
    } value;
    
} ACPIName;

#define ACPIDeviceMaxNames 6 // FIXME : this is absolutly arbitrary :)
typedef struct
{
    char id[5]; // 4 bytes + null ; 00000 denotes an invalid device. invalid devices shall be at the end, ie no gaps in the list
    ACPIName names[ACPIDeviceMaxNames];
}ACPIDevice;


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

#endif /* ACPIDesc_h */



