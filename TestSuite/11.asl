DefinitionBlock ("", "DSDT", 2, "VBOX  ", "VBOXBIOS", 0x00000002)
{
				OperationRegion (CBAT, SystemIO, 0x4040, 0x08)
                Field (CBAT, DWordAcc, NoLock, Preserve)
                {
                    IDX0,   32, 
                    DAT0,   32
                }

                IndexField (IDX0, DAT0, DWordAcc, NoLock, Preserve)
                {
                    STA_,   32, 
                    _RAT,   32, 
                    /*
                    RCAP,   32, 
                    PVOL,   32, 
                    UNIT,   32, 
                    DCAP,   32, 
                    LFCP,   32, 
                    BTEC,   32, 
                    DVOL,   32, 
                    DWRN,   32, 
                    DLOW,   32, 
                    GRN1,   32, 
                    GRN2,   32, 
                    BSTA,   32, 
                    */
                    Offset (0x80), 
                    APSR,   32
                }
}