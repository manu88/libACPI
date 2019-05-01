/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20181213 (64-bit version)
 * Copyright (c) 2000 - 2018 Intel Corporation
 * 
 * Disassembling to symbolic ASL+ operators
 *
 * Disassembly of 30.aml, Wed May  1 13:47:31 2019
 *
 * Original Table Header:
 *     Signature        "SSDT"
 *     Length           0x0000006C (108)
 *     Revision         0x02
 *     Checksum         0xB1
 *     OEM ID           "OemId"
 *     OEM Table ID     "OemTblId"
 *     OEM Revision     0x00000001 (1)
 *     Compiler ID      "INTL"
 *     Compiler Version 0x20181213 (538448403)
 */
DefinitionBlock ("", "SSDT", 2, "OemId", "OemTblId", 0x00000001)
{
    Scope (\)
    {
        Name (ROMB, Buffer (0x00040000){})
        Name (AAAA, Buffer (0x0A)
        {
            "P00.00A"
        })
        Name (AAAC, Buffer (0x0A)
        {
            /* 0000 */  0x50, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x41, 0x00,  // P00.00A.
            /* 0008 */  0x00, 0x00                                       // ..
        })
        Name (AAAD, Buffer (0x0A)
        {
            /* 0000 */  0x50, 0x30, 0x30, 0x2E, 0x30, 0x30, 0x41, 0x00,  // P00.00A.
            /* 0008 */  0x00, 0x00                                       // ..
        })
    }
}

