DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
{
        Device (SOM_)
        {
            Name (_HID, EisaId ("PNP0A03"))
        }
        Device (PCI1)
        {
            Name (_HID, EisaId ("PNP0A01"))
            Name (TADR, 0x4536ABEF)
        }
}
