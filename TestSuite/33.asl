DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{

	Device (LNKB)
	{
	    Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)
	}

	Device (LNKC)
	{
	    Name (_HID, EisaId ("PNP0C0F") /* PCI Interrupt Link Device */)
	}

	Name (PR00, Package (0x03)
    {
        Package (0x04)
        {
            0x0002FFFF, 
            Zero, 
            LNKB, 
            Zero
        }, 

        Package (0x04)
        {
            0x0002FFFF, 
            One, 
            LNKC, 
            Ones
        },
        10
    })


}