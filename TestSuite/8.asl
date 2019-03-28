DefinitionBlock ("", "DSDT", 120, "BXPC", "BXDSDT", 12)
{

    Scope (_SB)
    {
        Device (PCI0)
        {
            Name (_HID, EisaId ("PNP0A03") /* PCI Bus */)  // _HID: Hardware ID
            Name (CRES, ResourceTemplate ()
            {
                QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                0x0000000000000000, // Granularity
                0x0000008000000000, // Range Minimum
                0x000000ffffffffff, // Range Maximum
                0x0000000000000000, // Translation Offset
                0x0000008000000000, // Length
                ,, _Y01, AddressRangeMemory, TypeStatic)
            })
        }
    }


}

