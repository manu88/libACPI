DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
{
	Device (RTC)
                {
                    Name (_HID, EisaId ("PNP0B00"))  // _HID: Hardware ID
                    Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
                    {
                        IO (Decode16,
                            0x0070,             // Range Minimum
                            0x0070,             // Range Maximum
                            0x01,               // Alignment
                            0x08,               // Length
                            )
                        IRQNoFlags ()
                            {8}
                    })

                    OperationRegion (CMS0, SystemCMOS, Zero, 0x40)
                    Field (CMS0, ByteAcc, NoLock, Preserve)
                    {
                        RTSE,   8, 
                        Offset (0x02), 
                        RTMN,   8, 
                        Offset (0x04), 
                        RTHR,   8, 
                        Offset (0x06), 
                        RTDY,   8, 
                        RTDE,   8
                    }
                }

}
