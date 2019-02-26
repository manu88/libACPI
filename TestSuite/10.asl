DefinitionBlock ("", "DSDT", 20, "BXPC", "BXDSDT", 42)
{

    Scope (_SB)
    {
        Device (PCI0)
        {
            Name (BUF0, Buffer(4) {0x01,0x02,0x03,0x79,0x79,0x79})
        }
    }

}

