DefinitionBlock ("test.aml", "DSDT", 40, "OEMID", "PROUT", 0xABCF1234)
{
    Scope (_SB)
    {
    }

    Scope (_SB)
    {
        Device (PCI0)
        {
            Name (_HID, EisaId ("PNP0A03"))
        }
        Device (PCI1)
        {
            Name (_HID, EisaId ("PNP0A01"))
            Name (_ADR, 0x4536ABEF)
        }
	Device(PCI2)
	{
	    Name (_HID, EisaId ("PNP0A01"))
            Name (_ADR, 0x4536ABEF)
	    Name(_CSR , 0)
	    Scope (_SB)
    	    {
     	    }
	    Scope (_SB)
    	    {
    	    }
	    Scope (_SB)
            {
            }
            Scope (_SB)
            {
            }
	}
    }
}
