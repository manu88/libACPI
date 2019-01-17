
DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{

Name (STS0, Buffer () {0xAB, 0xCD, 0xEF, 0x12,0x00,0x12,0xEF,0xCD,0xAB})

Name (CR64, ResourceTemplate ()
        {
            QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
                0x0000000000000000, // Granularity
                0x0000008000000000, // Range Minimum
                0x000000FFFFFFFFFF, // Range Maximum
                0x0000000000000000, // Translation Offset
                0x0000008000000000, // Length
                ,, _Y01, AddressRangeMemory, TypeStatic)
        })



Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
            {
                Memory32Fixed (ReadOnly,
                    0xFED00000,         // Address Base
                    0x00000400,         // Address Length
                    )
            })


}
