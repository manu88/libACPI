DefinitionBlock ("", "DSDT", 20, "BXPC", "BXDSDT", 42)
{

    Scope (_SB)
    {
        Device (PCI0)
        {
            //Name (_HID, EisaId ("PNP0A03") /* PCI Bus */)  // _HID: Hardware ID
            Name (BUF0, Buffer(4) {0x01,0x02,0x03,0x04})
            Name (BUF1, Buffer() {"\tbcde"}) // This one will no be considered as a string since '\t' is not printable
            Name (BUF2, Buffer() {"abcde"}) // Length = 6
/*
            Name (CRES, ResourceTemplate ()
            {

                IO(Decode16, 0x62, 0x62, 0, 1)

                WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
                    0x0000,             // Granularity
                    0x0000,             // Range Minimum
                    0x00FF,             // Range Maximum
                    0x0000,             // Translation Offset
                    0x0100,             // Length
                    ,, )

                })
*/
        }
    }

}

