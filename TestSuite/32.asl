DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{

	Device (LNKB)
	{
	    Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)
	}

	Device (LNK)
	{
	    Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)
	}

	Name (PR00, Package (0x06)
    {
        
        0x0002FFFF, 
        Zero, 
        LNKB, 
        One,
        LNK, 
        Ones

    })


}