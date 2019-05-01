/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20181213 (64-bit version)
 * Copyright (c) 2000 - 2018 Intel Corporation
 * 
 * Disassembling to symbolic ASL+ operators
 *
 * Disassembly of 33.aml, Wed May  1 16:05:03 2019
 *
 * Original Table Header:
 *     Signature        "DSDT"
 *     Length           0x0000006A (106)
 *     Revision         0x01 **** 32-bit table (V1), no 64-bit math support
 *     Checksum         0xE2
 *     OEM ID           "BXPC"
 *     OEM Table ID     "BXDSDT"
 *     OEM Revision     0x00000001 (1)
 *     Compiler ID      "INTL"
 *     Compiler Version 0x20181213 (538448403)
 */
DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{
    Device (LNKB)
    {
        Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)  // _HID: Hardware ID
    }

    Device (LNKC)
    {
        Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)  // _HID: Hardware ID
    }

    Name (PR00, Package (0x02)
    {
        Package (0x04)
        {
            0x0002FFFF, 
            Zero, 
            LNKB, 
            Zero
        }, 

        Package (0x04)
        {
            0x0002FFFF, 
            One, 
            LNKC, 
            Zero
        }
    })
}

