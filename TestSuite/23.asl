DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{
    Scope (\) 
    {
        OperationRegion (FCFG, SystemIO, 0x510, 2)
        
        Field (FCFG, WordAcc, NoLock, Preserve)
        {
            FSEL,   16, // Selector Register
        }

        Field (FCFG, ByteAcc, NoLock, Preserve)
        {
            Offset (1),
            FDAT,   8,  // Data Register
        }


        Method (FRDI, 1, Serialized) 
        {
            Local1 = 0
            For (Local0 = 0, Local0 < Arg0, Local0++)
            {
                Local1 = (Local1 << 8) | FDAT
            }
            Return (Local1)
        }

        Method (FRDB, 1, Serialized) {
            Local1 = Buffer (Arg0) {}
            For (Local0 = 0, Local0 < Arg0, Local0++)
            {
                Local1[Local0] = FDAT
            }
            Return (Local1)
        }
    }
    Device (PCI0)
    {
            Name (_HID, EisaId ("PNP0A03") /* PCI Bus */)  // _HID: Hardware ID
            Name (_ADR, Zero)  // _ADR: Address
            Name (_UID, One)  // _UID: Unique ID

            Method (FRDF, 1, Serialized) 
            {
            FSEL = 0x19                     // FW_CFG_FILE_DIR
            Local0 = FRDI(4)                // files count
            While (Local0 > 0) {
                Local1 = FRDI(4)            // file size
                Local2 = FRDI(2)            // write this to FSEL to select it
                FRDI(2)                     // reserved
                Local3 = ToString(FRDB(56)) // name
                If (Local3 == Arg0) {
                    FSEL = Local2
                    Return (FRDB(Local1))
                }
                Local0--
            }
            Return (Zero)
        }
    }
}
