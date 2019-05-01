DefinitionBlock ("", "DSDT", 1, "BXPC", "BXDSDT", 0x00000001)
{
	OperationRegion(TOP1, GenericSerialBus, 0x00, 0x100) 
	/*
	Field(TOP1, BufferAcc, NoLock, Preserve)
	{ 
		Connection(I2CSerialBusV2(0x5a,,100000,,"\\_SB.I2C",,,,,
			RawDataBuffer(){1,6})),
		AccessAs(BufferAcc, AttribBlock),
		TFK1, 8,
		TFK2, 8 
	}
	*/
	// Create the GenericSerialBus data buffer
	Name(BUFF, Buffer(34){})
	CreateByteField(BUFF, 0x00, STAT)
	CreateBytefield(BUFF, 0x01, LEN)
	CreateWordField(BUFF, 0x03, DATW)
	CreateField(BUFF, 16, 256, DBUF)
	CreateField(BUFF, 16, 32,  DATD)


}