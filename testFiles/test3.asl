DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
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
    }
}
