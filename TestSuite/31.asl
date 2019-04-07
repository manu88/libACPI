DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{
	Method (_OSC, 4, NotSerialized)  // _OSC: Operating System Capabilities
            {
                If ((Arg0 == ToUUID ("33db4d5b-1ff7-401c-9657-7441c03dd766") /* PCI Host Bridge Device */))
                {
                    Return (Arg3)
                }
                Else
                {
                    CreateDWordField (Arg3, Zero, CDW1)
                    CDW1 |= 0x04
                    Return (Arg3)
                }
            }
}