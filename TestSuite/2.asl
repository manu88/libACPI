DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{
    Device (PCI0)
    {
            Name (_HID, EisaId ("PNP0A03") /* PCI Bus */)  // _HID: Hardware ID
            Name (_ADR, Zero)  // _ADR: Address
            Name (_UID, One)  // _UID: Unique ID
    }
}
