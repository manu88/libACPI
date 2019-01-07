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

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include "ACPI_Tests.h"
#include "ACPIParser.h"
//#include "UbuntuVBDef.h"


static int IaslVendorID = 0x4c544e49;

/*
static uint8_t test4[] =
{
    0x44,0x53,0x44,0x54,0x4c,0x00,0x00,0x00,
    0x01,0x8e,0x4f,0x45,0x4d,0x49,0x44,0x00,
    0x54,0x41,0x42,0x4c,0x45,0x49,0x44,0x00,
    0x00,0x00,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x5b,0x82,0x0f,0x50,
    0x43,0x49,0x30,0x08,0x5f,0x48,0x49,0x44,
    0x0c,0x41,0xd0,0x0a,0x03,0x5b,0x82,0x15,
    0x50,0x43,0x49,0x31,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0a,0x01,0x08,0x5f,
    0x41,0x44,0x52,0x00
};
*/

/*
 DefinitionBlock ("test.aml", "DSDT", 1, "OEMID", "TABLEID", 0x00000000)
 {
    Device (PCI0)
    {
        Name (_HID, EisaId ("PNP0A03"))
    }
    Device (PCI1)
    {
        Name (_HID, EisaId ("PNP0A01"))
        Name (_ADR, 0x00)
    }
 }
 */
static uint8_t test0[] =
{
    0x44,0x53,0x44,0x54,0x35,0x00,0x00,0x00,
    0x01,0x46,0x4f,0x45,0x4d,0x49,0x44,0x00,
    0x54,0x41,0x42,0x4c,0x45,0x49,0x44,0x00,
    0x00,0x00,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x5b,0x82,0x0f,0x50,
    0x43,0x49,0x30,0x08,0x5f,0x48,0x49,0x44,
    0x0c,0x41,0xd0,0x0a,0x03
};

/*
 DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
 {
    Device (PCI0)
    {
        Name (_HID, EisaId ("PNP0A03"))
    }
    Device (PCI1)
    {
        Name (_HID, EisaId ("PNP0A01"))
        Name (_ADR, 0x00)
    }
 }
 */
static uint8_t test1[] =
{
    0x44,0x53,0x44,0x54,0x4c,0x00,0x00,0x00,
    0x0a,0x32,0x49,0x44,0x4f,0x45,0x4d,0x00,
    0x53,0x4f,0x4d,0x45,0x41,0x49,0x44,0x00,
    0x34,0x12,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x5b,0x82,0x0f,0x50,
    0x43,0x49,0x30,0x08,0x5f,0x48,0x49,0x44,
    0x0c,0x41,0xd0,0x0a,0x03,0x5b,0x82,0x15,
    0x50,0x43,0x49,0x31,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0a,0x01,0x08,0x5f,
    0x41,0x44,0x52,0x00
};

/*
 DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
 {
    Device (PCI0)
    {
        Name (_HID, EisaId ("PNP0A03"))
    }
    Device (PCI1)
    {
        Name (_HID, EisaId ("PNP0A01"))
        Name (_ADR, 0x4536ABEF)
    }
 }
 */
static uint8_t test2[] =
{
    0x44,0x53,0x44,0x54,0x50,0x00,0x00,0x00,
    0x0a,0x09,0x49,0x44,0x4f,0x45,0x4d,0x00,
    0x53,0x4f,0x4d,0x45,0x41,0x49,0x44,0x00,
    0x34,0x12,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x5b,0x82,0x0f,0x50,
    0x43,0x49,0x30,0x08,0x5f,0x48,0x49,0x44,
    0x0c,0x41,0xd0,0x0a,0x03,0x5b,0x82,0x19,
    0x50,0x43,0x49,0x31,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0a,0x01,0x08,0x5f,
    0x41,0x44,0x52,0x0c,0xef,0xab,0x36,0x45
};
/*
 DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
 {
     Scope (_SB)
     {
     }
     Scope (_SB)
     {
         Device (PCI0)
         {
            Name (_HID, EisaId ("PNP0A03"))
         }
         Device (PCI1)
         {
            Name (_HID, EisaId ("PNP0A01"))
            Name (_ADR, 0x4536ABEF)
         }
     }
 }
 */
static uint8_t test3[] =
{
    0x44,0x53,0x44,0x54,0x5c,0x00,0x00,0x00,
    0x0a,0x01,0x49,0x44,0x4f,0x45,0x4d,0x00,
    0x53,0x4f,0x4d,0x45,0x41,0x49,0x44,0x00,
    0x34,0x12,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x10,0x05,0x5f,0x53,
    0x42,0x5f,0x10,0x31,0x5f,0x53,0x42,0x5f,
    0x5b,0x82,0x0f,0x50,0x43,0x49,0x30,0x08,
    0x5f,0x48,0x49,0x44,0x0c,0x41,0xd0,0x0a,
    0x03,0x5b,0x82,0x19,0x50,0x43,0x49,0x31,
    0x08,0x5f,0x48,0x49,0x44,0x0c,0x41,0xd0,
    0x0a,0x01,0x08,0x5f,0x41,0x44,0x52,0x0c,
    0xef,0xab,0x36,0x45
};

/*
 DefinitionBlock ("test.aml", "DSDT", 40, "OEMID", "PROUT", 0xABCF1234)
 {
     Scope (_SB)
     {
     }
 
     Scope (_SB)
     {
        Device (PCI0)
        {
            Name (_HID, EisaId ("PNP0A03"))
        }
        Device (PCI1)
        {
            Name (_HID, EisaId ("PNP0A01"))
            Name (_ADR, 0x4536ABEF)
        }
        Device(PCI2)
        {
            Name (_HID, EisaId ("PNP0A01"))
            Name (_ADR, 0x4536ABEF)
            Name(_CSR , 0)
            Scope (_SB)
            {
            }
            Scope (_SB)
            {
            }
            Scope (_SB)
            {
            }
            Scope (_SB)
            {
            }
        }
     }
 }
 */
static uint8_t test4[] =
{
    0x44,0x53,0x44,0x54,0x96,0x00,0x00,0x00,
    0x28,0xa3,0x4f,0x45,0x4d,0x49,0x44,0x00,
    0x50,0x52,0x4f,0x55,0x54,0x00,0x00,0x00,
    0x34,0x12,0xcf,0xab,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x10,0x05,0x5f,0x53,
    0x42,0x5f,0x10,0x4b,0x06,0x5f,0x53,0x42,
    0x5f,0x5b,0x82,0x0f,0x50,0x43,0x49,0x30,
    0x08,0x5f,0x48,0x49,0x44,0x0c,0x41,0xd0,
    0x0a,0x03,0x5b,0x82,0x19,0x50,0x43,0x49,
    0x31,0x08,0x5f,0x48,0x49,0x44,0x0c,0x41,
    0xd0,0x0a,0x01,0x08,0x5f,0x41,0x44,0x52,
    0x0c,0xef,0xab,0x36,0x45,0x5b,0x82,0x37,
    0x50,0x43,0x49,0x32,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0a,0x01,0x08,0x5f,
    0x41,0x44,0x52,0x0c,0xef,0xab,0x36,0x45,
    0x08,0x5f,0x43,0x53,0x52,0x00,0x10,0x05,
    0x5f,0x53,0x42,0x5f,0x10,0x05,0x5f,0x53,
    0x42,0x5f,0x10,0x05,0x5f,0x53,0x42,0x5f,
    0x10,0x05,0x5f,0x53,0x42,0x5f
};



/*
 DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
 {
     Device (RTC)
     {
        Name (_HID, EisaId ("PNP0B00"))  // _HID: Hardware ID
        Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
        {
            IO (Decode16,
            0x0070,             // Range Minimum
            0x0070,             // Range Maximum
            0x01,               // Alignment
            0x08,               // Length
            )
            IRQNoFlags ()
            {8}
        })
 
        OperationRegion (CMS0, SystemCMOS, Zero, 0x40)
        Field (CMS0, ByteAcc, NoLock, Preserve)
        {
             RTSE,   8,
             Offset (0x02),
             RTMN,   8,
             Offset (0x04),
             RTHR,   8,
             Offset (0x06),
             RTDY,   8,
             RTDE,   8
         }
     }
 
 }
 */
static uint8_t test5[] =
{
    0x44,0x53,0x44,0x54,0x7d,0x00,0x00,0x00,
    0x0a,0x14,0x49,0x44,0x4f,0x45,0x4d,0x00,
    0x53,0x4f,0x4d,0x45,0x41,0x49,0x44,0x00,
    0x34,0x12,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x5b,0x82,0x47,0x05,
    0x52,0x54,0x43,0x5f,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0b,0x00,0x08,0x5f,
    0x43,0x52,0x53,0x11,0x10,0x0a,0x0d,0x47,
    0x01,0x70,0x00,0x70,0x00,0x01,0x08,0x22,
    0x00,0x01,0x79,0x00,0x5b,0x80,0x43,0x4d,
    0x53,0x30,0x05,0x00,0x0a,0x40,0x5b,0x81,
    0x25,0x43,0x4d,0x53,0x30,0x01,0x52,0x54,
    0x53,0x45,0x08,0x00,0x08,0x52,0x54,0x4d,
    0x4e,0x08,0x00,0x08,0x52,0x54,0x48,0x52,
    0x08,0x00,0x08,0x52,0x54,0x44,0x59,0x08,
    0x52,0x54,0x44,0x45,0x08
};

/*
 DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
 {
     Device (SOM_)
     {
        Name (_HID, EisaId ("PNP0A03"))
     }
     Device (PCI1)
     {
        Name (_HID, EisaId ("PNP0A01"))
        Name (_ADR, 0x4536ABEF)
     }
 }
 */
static uint8_t testName[] =
{
    0x44,0x53,0x44,0x54,0x50,0x00,0x00,0x00,
    0x0a,0xc7,0x49,0x44,0x4f,0x45,0x4d,0x00,
    0x53,0x4f,0x4d,0x45,0x41,0x49,0x44,0x00,
    0x34,0x12,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x5b,0x82,0x0f,0x53,
    0x4f,0x4d,0x5f,0x08,0x5f,0x48,0x49,0x44,
    0x0c,0x41,0xd0,0x0a,0x03,0x5b,0x82,0x19,
    0x50,0x43,0x49,0x31,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0a,0x01,0x08,0x5f,
    0x41,0x44,0x52,0x0c,0xef,0xab,0x36,0x45
};

static AMLParserError ACPIInitAndParse(ACPIDocument *doc , const uint8_t* buffer , size_t bufSize)
{
    assert( ACPIDocumentInit(doc) ) ;
    
    return ACPIParseAMLByteCode(doc,buffer , bufSize );
    
}

static void tooShortBuf()
{
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, test0 , 4 ) == AMLParserError_None);
    assert(doc.hasDesc == 0);
}

static void doTest0()
{
    printf("---------Test0-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, test0 , sizeof(test0) ) == AMLParserError_None);
    assert(doc.hasDesc);
    
    assert(doc.desc.tableLength >0 );
    assert(strncmp(doc.desc.tableSignature, "DSDT", 4) == 0);
    assert(doc.desc.complianceRevision == 1);
    assert(strncmp(doc.desc.OEMId, "OEMID", 6) == 0);
    assert(strncmp(doc.desc.tableId, "TABLEID", 8) == 0);
    assert(doc.desc.OEMRev == 0x0);
    assert(doc.desc.creatorID == IaslVendorID);
    
    assert(ACPIDocumentGetRoot(&doc) != NULL );
    assert(ACPIDocumentGetRoot(&doc)->parent == NULL );
    
    const size_t numDevices = ACPIDocumentGetDevicesCount(&doc);
    assert(numDevices == 2);
    assert(ACPIDocumentGetNthDevice(&doc,0) );
    
    
    
    /*
    assert(ACPIDeviceGetNamesCount(&doc.devices[0]) == 1 );
    
    assert( strcmp(doc.devices[0].id, "PCI0") == 0 );
    assert( strcmp(doc.devices[0].names[0].id, "_HID") == 0 );
    assert( doc.devices[0].names[0].value.word == 0x30ad041 );
     */
}

static void doTest1()
{
    printf("---------Test1-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, test1 , sizeof(test1) ) == AMLParserError_None);
    assert(doc.hasDesc);
    assert(doc.desc.tableLength >0 );
    assert(doc.desc.creatorID == IaslVendorID);
    assert(strncmp(doc.desc.tableSignature, "DSDT", 4) == 0);
    assert(doc.desc.complianceRevision == 0x0a);
    assert(strncmp(doc.desc.OEMId, "IDOEM", 6) == 0);
    assert(strncmp(doc.desc.tableId, "SOMEAID", 8) == 0);
    assert(doc.desc.OEMRev == 0x00001234);

    assert(ACPIDocumentGetRoot(&doc) != NULL );
    
    const size_t numDevices = ACPIDocumentGetDevicesCount(&doc);
    assert(numDevices == 2);
    assert(ACPIDocumentGetNthDevice(&doc,0) );
    assert(ACPIDocumentGetNthDevice(&doc,1) );
    
    
    
    char name[5] = {0};
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,0), name));
    assert(strcmp(name, "PCI0") == 0);
    
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,1), name));
    assert(strcmp(name, "PCI1") == 0);
    
    /*
    assert(ACPIDeviceGetNamesCount(&doc.devices[0]) == 1 );
    assert( strcmp(doc.devices[0].id, "PCI0") == 0 );
    assert( strcmp(doc.devices[0].names[0].id, "_HID") == 0 );
    assert( doc.devices[0].names[0].value.word == 0x30ad041 );
    
    assert(ACPIDeviceGetNamesCount(&doc.devices[1]) == 2 );
    assert( strcmp(doc.devices[1].id, "PCI1") == 0 );
    assert( strcmp(doc.devices[1].names[0].id, "_HID") == 0 );
    assert(        doc.devices[1].names[0].value.word == 0x10ad041 );
    assert( strcmp(doc.devices[1].names[1].id, "_ADR") == 0 );
    assert(        doc.devices[1].names[1].value.word == 0 );
     */
}

static void doTest2()
{
    printf("---------Test2-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, test2 , sizeof(test2) ) == AMLParserError_None);
    assert(doc.hasDesc);
    assert(doc.desc.tableLength >0 );
    assert(doc.desc.creatorID == IaslVendorID);
    assert(strncmp(doc.desc.tableSignature, "DSDT", 4) == 0);
    assert(doc.desc.complianceRevision == 0x0a);
    assert(strncmp(doc.desc.OEMId, "IDOEM", 6) == 0);
    assert(strncmp(doc.desc.tableId, "SOMEAID", 8) == 0);
    assert(doc.desc.OEMRev == 0x00001234);
    
    
    
    assert(ACPIDocumentGetRoot(&doc) != NULL );
    assert(ACPIDocumentGetDevicesCount(&doc) == 2);
    
    assert(ACPIDocumentGetNthDevice(&doc,0) );
    assert(ACPIDocumentGetNthDevice(&doc,1) );
    assert(ACPIDocumentGetNthDevice(&doc,2) == NULL );
    
    char name[5];
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,0), name));
    assert(strcmp(name, "PCI0") == 0);
    
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,1), name));
    assert(strcmp(name, "PCI1") == 0);
    /*
    assert(ACPIDeviceGetNamesCount(&doc.devices[0]) == 1 );
    assert( strcmp(doc.devices[0].id, "PCI0") == 0 );
    assert( strcmp(doc.devices[0].names[0].id, "_HID") == 0 );
    assert( doc.devices[0].names[0].value.word == 0x30ad041 );
    
    assert(ACPIDeviceGetNamesCount(&doc.devices[1]) == 2 );
    assert( strcmp(doc.devices[1].id, "PCI1") == 0 );
    assert( strcmp(doc.devices[1].names[0].id, "_HID") == 0 );
    assert(        doc.devices[1].names[0].value.word == 0x10ad041 );
    assert( strcmp(doc.devices[1].names[1].id, "_ADR") == 0 );
    assert(        doc.devices[1].names[1].value.word == 0x4536ABEF );
    */
}

static void doTest3()
{
    printf("---------Test3-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, test3 , sizeof(test3) ) == AMLParserError_None);
    assert(doc.hasDesc);
    assert(doc.desc.tableLength >0 );
    assert(doc.desc.creatorID == IaslVendorID);
    assert(strncmp(doc.desc.tableSignature, "DSDT", 4) == 0);
    assert(doc.desc.complianceRevision == 0x0a);
    assert(strncmp(doc.desc.OEMId, "IDOEM", 6) == 0);
    assert(strncmp(doc.desc.tableId, "SOMEAID", 8) == 0);
    assert(doc.desc.OEMRev == 0x00001234);
    
    assert(ACPIDocumentGetRoot(&doc) != NULL );
    assert(ACPIDocumentGetDevicesCount(&doc) == 2);
    
    assert(ACPIDocumentGetNthDevice(&doc,0) );
    assert(ACPIDocumentGetNthDevice(&doc,1) );
    assert(ACPIDocumentGetNthDevice(&doc,2) == NULL );
    
    char name[5];
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,0), name));
    assert(strcmp(name, "PCI0") == 0);
    
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,1), name));
    assert(strcmp(name, "PCI1") == 0);
    
    //assert(ACPIDeviceGetNamesCount(&doc.devices[0]) == 1 );
    //assert(ACPIDeviceGetNamesCount(&doc.devices[1]) == 2 );
}

static void doTest4()
{
    printf("---------Test4-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, test4 , sizeof(test4) ) == AMLParserError_None);
    assert(doc.hasDesc);
    assert(doc.desc.tableLength >0 );
    assert(doc.desc.creatorID == IaslVendorID);
    assert(strncmp(doc.desc.tableSignature, "DSDT", 4) == 0);
    assert(doc.desc.complianceRevision == 0x28);
    assert(strncmp(doc.desc.OEMId, "OEMID", 6) == 0);
    assert(strncmp(doc.desc.tableId, "PROUT", 8) == 0);
    assert(doc.desc.OEMRev == 0xABCF1234);
    
    assert(ACPIDocumentGetRoot(&doc) != NULL );
    assert(ACPIDocumentGetDevicesCount(&doc) == 3);
    
    assert(ACPIDocumentGetNthDevice(&doc,0) );
    assert(ACPIDocumentGetNthDevice(&doc,1) );
    assert(ACPIDocumentGetNthDevice(&doc,2)  );
    assert(ACPIDocumentGetNthDevice(&doc,3) == NULL );
    
    char name[5];
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,0), name));
    assert(strcmp(name, "PCI0") == 0);
    
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,1), name));
    assert(strcmp(name, "PCI1") == 0);
    
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,2), name));
    assert(strcmp(name, "PCI2") == 0);
    
}


static void doTest5()
{
    printf("---------Test5-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, test5 , sizeof(test5) ) == AMLParserError_None);
    assert(doc.hasDesc);
    assert(strncmp(doc.desc.tableSignature, "DSDT", 4) == 0);
    assert(doc.desc.complianceRevision == 10);
    assert(strncmp(doc.desc.OEMId, "IDOEM", 6) == 0);
    assert(strncmp(doc.desc.tableId, "SOMEAID", 8) == 0);
    assert(doc.desc.OEMRev == 0x00001234);
    
    assert(ACPIDocumentGetRoot(&doc) != NULL );
    assert(ACPIDocumentGetDevicesCount(&doc) == 1);
    
    char name[5];
    assert(ACPIDeviceGetName(ACPIDocumentGetNthDevice(&doc,0), name));
    assert(strcmp(name, "RTC") == 0);
    /*
    assert(strcmp(doc.devices[0].id, "RTC") == 0);
    
    size_t numNames = ACPIDeviceGetNamesCount(&doc.devices[0]);
    assert(numNames == 2 );
    
    assert( strcmp(doc.devices[0].names[0].id, "_HID") == 0 );
    assert( strcmp(doc.devices[0].names[1].id, "_CRS") == 0 );
     */
}

static void doTestName()
{
    printf("---------TestName-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, testName , sizeof(testName) ) == AMLParserError_None);
    assert(doc.hasDesc);
    assert(strncmp(doc.desc.tableSignature, "DSDT", 4) == 0);
    assert(doc.desc.complianceRevision == 10);
    assert(strncmp(doc.desc.OEMId, "IDOEM", 6) == 0);
    assert(strncmp(doc.desc.tableId, "SOMEAID", 8) == 0);
    assert(doc.desc.OEMRev == 0x00001234);
    
    assert(ACPIDocumentGetDevicesCount(&doc) == 2);
    assert(ACPIDocumentGetRoot(&doc) != NULL );
    char name[5];
    const TreeElement* device0 = ACPIDocumentGetNthDevice(&doc,0);
    assert(device0);
    assert(device0->parent == ACPIDocumentGetRoot(&doc) );
    assert(ACPIDeviceGetName(device0, name));
    assert(strcmp(name, "SOM") == 0);
    
    memset(name, 0, 5);
    const TreeElement* device1 = ACPIDocumentGetNthDevice(&doc,1);
    assert(device1);
    assert(device1->parent == ACPIDocumentGetRoot(&doc) );
    assert(ACPIDeviceGetName(device1, name));
    assert(strcmp(name, "PCI1") == 0);
    
    
    
}

/*
static void doUbuntuVBTest()
{
    printf("---------Test Ubuntu virtualBox-------\n");
    ACPIDocument doc;
    assert(ACPIInitAndParse(&doc, testUbuntuVB , sizeof(testUbuntuVB) ) == AMLParserError_None);

}
*/

static void BasicTests()
{
    ACPIDocument doc;
    assert(ACPIDocumentInit(&doc) == 1);
    assert(doc.hasDesc == 0);
    assert(ACPIDocumentGetDevicesCount(&doc) == 0);
    assert(ACPIDocumentGetRoot(&doc) == NULL );
    //assert(ACPIDeviceGetNamesCount(&doc.devices[0]) == 0);
    
    
}

void ACPI_Tests()
{

    BasicTests();
    tooShortBuf();
    doTest0();
    doTest1();
    doTest2();
    doTest3();
    doTest4();
 
    doTest5();
    doTestName();
    
    //doUbuntuVBTest(); //not working for now
    
}


void ACPIReadAML(int argc, const char * argv[])
{
    
    int fd = open(argv[1], O_RDONLY);
    
    uint8_t c = 0;
    int a = 0;
    while(  read(fd, &c, 1) >0)
    {
        if (a++ % 8 == 0)
        {
            printf("\n");
        }
        printf("0x%02x," ,c);
        
        //
        }
}



/*

void ACPIWriteAML()
{
    size_t c;
    FILE *fp;
    
 
    fp = fopen("test_file", "w");
    if (fp == NULL) {
        printf("Open Error\n");
        return ;
    }
    

    for (c=0; c < sizeof(t); c++)
        fputc(t[c], fp);
    

    fclose(fp);
    
    return ;
}

*/
