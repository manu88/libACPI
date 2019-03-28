DefinitionBlock ("", "DSDT", 20, "BXPC", "BXDSDT", 42)
{

    Scope (_SB)
    {
        Device (PCI0)
        {
            Name (_HID, "ACPI0003" /* Power Source Device */)  // _HID: Hardware ID
            //Name (_HID, EisaId ("PNP0A03") /* PCI Bus */)  // _HID: Hardware ID
            Name (BUF0, Buffer(4) {0x01,0x02,0x03,0x04})
            Name (BUF1, Buffer() {"\tbcde"}) // This one will no be considered as a string since '\t' is not an ascii code (?)
            Name (BUF2, Buffer() {"abcde"}) // Length = 6

        }
    }

}

