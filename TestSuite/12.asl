DefinitionBlock ("","DSDT",1,"BXPC","BXDSDT",1)
{	
	
	
		device(S2)
		{
			Name(_HID,4)
			device(ISA)
			{
				Name(_HID,1)
				
			}
		}

	
	device (\S2.ISA.COM1)
	{
		Name(_HID,EisaId ("PNP0A03"))
					
		Name(_ADR,Zero)
		Name(_UID,One)
	}
	

}