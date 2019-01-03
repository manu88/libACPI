/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20181213 (64-bit version)
 * Copyright (c) 2000 - 2018 Intel Corporation
 * 
 * Disassembling to symbolic ASL+ operators
 *
 * Disassembly of testName.aml, Sun Dec 30 13:57:36 2018
 *
 * Original Table Header:
 *     Signature        "DSDT"
 *     Length           0x00000050 (80)
 *     Revision         0x0A
 *     Checksum         0xC7
 *     OEM ID           "IDOEM"
 *     OEM Table ID     "SOMEAID"
 *     OEM Revision     0x00001234 (4660)
 *     Compiler ID      "INTL"
 *     Compiler Version 0x20181213 (538448403)
 */
DefinitionBlock ("", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
{
    Device (SOM)
    {
        Name (_HID, EisaId ("PNP0A03") /* PCI Bus */)  // _HID: Hardware ID
    }

    Device (PCI1)
    {
        Name (_HID, EisaId ("PNP0A01"))  // _HID: Hardware ID
        Name (_ADR, 0x4536ABEF)  // _ADR: Address
    }
}

