DefinitionBlock ("", "DSDT", 10, "BXPC", "BXDSDT", 0x00000002)
{
    Device (PCI0)
    {
            Name (_HID, EisaId ("PNP0A03") /* PCI Bus */)  // _HID: Hardware ID
            Name (_ADR, Zero)  // _ADR: Address
            Name (_UID, One)  // _UID: Unique ID
    }

    Device (PCI1)
    {
            Name (_HID, EisaId ("PNP0501") /* PCI Bus */)  // _HID: Hardware ID
            Name (_ADR, One)  // _ADR: Address
            Name (_UID, Zero)  // _UID: Unique ID
    }
}
