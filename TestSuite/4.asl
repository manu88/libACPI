DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{
	Scope (_SB)
    {
    	Device (PCI0)
    	{
            Name (_HID, EisaId ("PNP0A03") )
            Name (_ADR, Zero) 
            Name (_UID, One)
    	}

    	Scope(PCI0)
    	{
    		Device (ISA)
        	{
        		Name (_ADR, 0x00010000)
        		
        	}
    	}

    	Scope (PCI0.ISA)
    	{
        	Device (RTC)
        	{
            	Name (_HID, EisaId ("PNP0B00"))
            }
        }
    }

    Scope (_SB.PCI0.ISA.RTC)
    {
    	Name (_UID, 0x12)
    }

}
