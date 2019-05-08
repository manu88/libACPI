DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{
	OperationRegion (SYSI, SystemIO, 0x4048, 0x08)
	Field (SYSI, DWordAcc, NoLock, Preserve)
    {
        IDX0,   32, 
        DAT0,   32
    }

	IndexField (IDX0, DAT0, DWordAcc, NoLock, Preserve)
    {
        
        
        
        SL2B,   32, 
      } 

	Device(TEST)
	{
		Name (_HID, EisaId ("PNP0501") /* 16550A-compatible COM Serial Port */)  // _HID: Hardware ID
		Method (_STA, 0, NotSerialized)  // _STA: Status
	    {
	        If ((SL2B == Zero))
	        {
	            Return (Zero)
	        }
	        Else
	        {
	            Return (0x0F)
	        }
	    }
	    Name (_LOL, 1234 )
	}
}