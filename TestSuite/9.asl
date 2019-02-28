DefinitionBlock ("", "DSDT", 2, "BXPC", "BXDSDT", 1)
{

    Scope (_SB)
    {
        Device (PCI1)
        {
            Name (_HID, EisaId ("CSI2201"))
            Name (CRES, ResourceTemplate ()
            {
                Memory32Fixed (ReadOnly,
                    0xFED00000,         // Address Base
                    0x00000400,         // Address Length
                    )
            })
            OperationRegion (PCIC, PCI_Config, Zero, 0x04)
            Field (PCIC, DWordAcc, NoLock, Preserve)
            {
                VEND,   32
            }
            Method (_S1D, 0, NotSerialized)
            {
                //Return (Zero)
            }
        }
    }


}

