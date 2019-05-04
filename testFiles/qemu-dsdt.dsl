// File Generated with AML::Decompiler from libACPI
DefinitionBlock ("","DSDT",1,"BXPC","BXDSDT",1)
{
	Scope(\)
	{
		OperationRegion(DBG ,SystemIO,0x0402,One)
		Field (DBG, ByteAcc, NoLock, Preserve )
		{
			DBGB, 0x0000000000000008,
		}
		Method(DBUG,1)
		{
			// Some Stuff
		}
	}
	Scope(_SB)
	{
		Device(PCI0)
		{
			Name(_HID,EisaId ("PNP0A03"))
			Name(_ADR,Zero)
			Name(_UID,One)
		}
	}
	Scope(_SB.PCI0)
	{
		Name(CRES, ResourceTemplate ()
		{
				WordBusNumber (ResourceProducer,MinFixed,MaxFixed,PosDecode,
				0x0000,
				0x0000,
				0x00FF,
				0x0000,
				0x0100,
				,, )

				IO (Decode16,
				0x0CF8,
				0x0CF8,
				0x01 ,
				0x08 ,
				)

				WordBusNumber (ResourceProducer,MinFixed,MaxFixed,PosDecode,
				0x0000,
				0x0000,
				0x0CF7,
				0x0000,
				0x0CF8,
				,, )

				WordBusNumber (ResourceProducer,MinFixed,MaxFixed,PosDecode,
				0x0000,
				0x0D00,
				0xFFFF,
				0x0000,
				0xF300,
				,, )

				DWordMemory (ResourceProducer,PosDecode,MinFixed,MaxFixed, Cacheable, ReadWrite,
				0x00000000,
				0x000A0000,
				0x000BFFFF,
				0x00000000,
				0x00020000,
				,,,  AddressRangeMemory, TypeStatic)

				DWordMemory (ResourceProducer,PosDecode,MinFixed,MaxFixed,NonCacheable, ReadWrite,
				0x00000000,
				0xE0000000,
				0xFEBFFFFF,
				0x00000000,
				0x1EC00000,
				,,,  AddressRangeMemory, TypeStatic)

		})
		Name(CR64, ResourceTemplate ()
		{
			QWordMemory( ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
			0x0000000000000000,
			0x0000008000000000,
			0x000000FFFFFFFFFF,
			0x0000000000000000,
			0x0000008000000000,
			,,,  AddressRangeMemory, TypeStatic)
		})
		Method(_CRS,0)
		{
			// Some Stuff
		}
	}
	Scope(_SB)
	{
		Device(HPET)
		{
			Name(_HID,EisaId ("PNP0103"))
			Name(_UID,Zero)
			OperationRegion(HPTM ,SystemMemory,0x0000,0x0400)
			Field (HPTM, DWordAcc, Lock, Preserve )
			{
				VEND, 0x0000000000000020,
				PRD, 0x0000000000000020,
			}
			Method(_STA,0)
			{
				// Some Stuff
			}
			Name(_CRS, ResourceTemplate ()
			{
				Memory32Fixed( ReadOnly,
				0xFED00000,
				0x00000400,
				)
			})
		}
	}
	Scope(_SB.PCI0)
	{
		Device(VGA)
		{
			Name(_ADR,0x0000000000020000)
			OperationRegion(PCIC ,PCI_Config,0x0000,0x0004)
			Field (PCIC, DWordAcc, NoLock, Preserve )
			{
				VEND, 0x0000000000000020,
			}
			Method(_S1D,0)
			{
				// Some Stuff
			}
			Method(_S2D,0)
			{
				// Some Stuff
			}
			Method(_S3D,0)
			{
				// Some Stuff
			}
		}
	}
	Scope(_SB.PCI0)
	{
		Device(PX13)
		{
			Name(_ADR,0x0000000000010003)
			OperationRegion(P13C ,PCI_Config,0x0000,0x00FF)
		}
	}
	Scope(_SB.PCI0)
	{
		Device(ISA)
		{
			Name(_ADR,0x0000000000010000)
			OperationRegion(P40C ,PCI_Config,0x0060,0x0004)
			Field (^PX13.P13C, AnyAcc, NoLock, Preserve )
			{
				Offset(0x5F),
				,0x0000000000000007,
				LPEN, 0x0000000000000001,
				Offset(0x67),
				,0x0000000000000003,
				CAEN, 0x0000000000000001,
				,0x0000000000000003,
				CBEN, 0x0000000000000001,
			}
			Name(FDEN,One)
		}
	}
	Scope(_SB.PCI0.ISA)
	{
		Device(RTC)
		{
			Name(_HID,EisaId ("PNP0B00"))
			Name(_CRS,Buffer(0x00000015) {0x47,0x01,0x70,0x00,0x70,0x00,0x10,0x02,0x22,0x00,0x01,0x47,0x01,0x72,0x00,0x72,0x00,0x02,0x06,0x79,0x00})
		}
		Device(KBD)
		{
			Name(_HID,EisaId ("PNP0303"))
			Method(_STA,0)
			{
				// Some Stuff
			}
			Name(_CRS,Buffer(0x00000015) {0x47,0x01,0x60,0x00,0x60,0x00,0x01,0x01,0x47,0x01,0x64,0x00,0x64,0x00,0x01,0x01,0x22,0x02,0x00,0x79,0x00})
		}
		Device(MOU)
		{
			Name(_HID,EisaId ("PNP0F13"))
			Method(_STA,0)
			{
				// Some Stuff
			}
			Name(_CRS,Buffer(0x00000005) {0x22,0x00,0x10,0x79,0x00})
		}
		Device(FDC0)
		{
			Name(_HID,EisaId ("PNP0700"))
			Method(_STA,0)
			{
				// Some Stuff
			}
			Name(_CRS,Buffer(0x00000018) {0x47,0x01,0xF2,0x03,0xF2,0x03,0x00,0x04,0x47,0x01,0xF7,0x03,0xF7,0x03,0x00,0x01,0x22,0x40,0x00,0x2A,0x04,0x00,0x79,0x00})
		}
		Device(LPT)
		{
			Name(_HID,EisaId ("PNP0400"))
			Method(_STA,0)
			{
				// Some Stuff
			}
			Name(_CRS,Buffer(0x0000000D) {0x47,0x01,0x78,0x03,0x78,0x03,0x08,0x08,0x22,0x80,0x00,0x79,0x00})
		}
		Device(COM1)
		{
			Name(_HID,EisaId ("PNP0501"))
			Name(_UID,One)
			Method(_STA,0)
			{
				// Some Stuff
			}
			Name(_CRS,Buffer(0x0000000D) {0x47,0x01,0xF8,0x03,0xF8,0x03,0x00,0x08,0x22,0x10,0x00,0x79,0x00})
		}
		Device(COM2)
		{
			Name(_HID,EisaId ("PNP0501"))
			Name(_UID,0x0000000000000002)
			Method(_STA,0)
			{
				// Some Stuff
			}
			Name(_CRS,Buffer(0x0000000D) {0x47,0x01,0xF8,0x02,0xF8,0x02,0x00,0x08,0x22,0x08,0x00,0x79,0x00})
		}
	}
	Scope(_SB.PCI0)
	{
		OperationRegion(PCST ,SystemIO,0xAE00,0x0008)
		Field (PCST, DWordAcc, NoLock, WriteAsZeros )
		{
			PCIU, 0x0000000000000020,
			PCID, 0x0000000000000020,
		}
		OperationRegion(SEJ ,SystemIO,0xAE08,0x0004)
		Field (SEJ, DWordAcc, NoLock, WriteAsZeros )
		{
			B0EJ, 0x0000000000000020,
		}
		Method(PCEJ,1)
		{
			// Some Stuff
		}
		Method(PCNF,0)
		{
			// Some Stuff
		}
	}
	Scope(_SB)
	{
		Scope(PCI0)
		{
			Name(_PRT,			Package(0x80)
			{
				Package(0x04)
				{
						0x000000000000FFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000000FFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000000FFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000000FFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000001FFFF,
						Zero,
						LNKS,
						Zero,
				},
				Package(0x04)
				{
						0x000000000001FFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000001FFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000001FFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000002FFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000002FFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000002FFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000002FFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000003FFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000003FFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000003FFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000003FFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000004FFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000004FFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000004FFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000004FFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000005FFFF,
						Zero,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000005FFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000005FFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000005FFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000006FFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000006FFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000006FFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000006FFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000007FFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000007FFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000007FFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000007FFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000008FFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000008FFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000008FFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000008FFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000009FFFF,
						Zero,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000009FFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000009FFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000009FFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000AFFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000AFFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000AFFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000AFFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000BFFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000BFFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000BFFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000BFFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000CFFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000CFFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000CFFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000CFFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000DFFFF,
						Zero,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000DFFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000DFFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000DFFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000EFFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000EFFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000EFFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000EFFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000FFFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000FFFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000FFFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000000FFFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000010FFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000010FFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000010FFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000010FFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000011FFFF,
						Zero,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000011FFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000011FFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000011FFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000012FFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000012FFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000012FFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000012FFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000013FFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000013FFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000013FFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000013FFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000014FFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000014FFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000014FFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000014FFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000015FFFF,
						Zero,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000015FFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000015FFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000015FFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000016FFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000016FFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000016FFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000016FFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000017FFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000017FFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000017FFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000017FFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000018FFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x000000000018FFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000018FFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000018FFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000019FFFF,
						Zero,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x000000000019FFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x000000000019FFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x000000000019FFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001AFFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001AFFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001AFFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001AFFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001BFFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001BFFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001BFFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001BFFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001CFFFF,
						Zero,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001CFFFF,
						One,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001CFFFF,
						0x0000000000000002,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001CFFFF,
						0x0000000000000003,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001DFFFF,
						Zero,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001DFFFF,
						One,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001DFFFF,
						0x0000000000000002,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001DFFFF,
						0x0000000000000003,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001EFFFF,
						Zero,
						LNKB,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001EFFFF,
						One,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001EFFFF,
						0x0000000000000002,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001EFFFF,
						0x0000000000000003,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001FFFFF,
						Zero,
						LNKC,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001FFFFF,
						One,
						LNKD,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001FFFFF,
						0x0000000000000002,
						LNKA,
						Zero,
				},
				Package(0x04)
				{
						0x00000000001FFFFF,
						0x0000000000000003,
						LNKB,
						Zero,
				},
			})
			}
			Field (PCI0.ISA.P40C, ByteAcc, NoLock, Preserve )
			{
				PRQ0, 0x0000000000000008,
				PRQ1, 0x0000000000000008,
				PRQ2, 0x0000000000000008,
				PRQ3, 0x0000000000000008,
			}
			Method(IQST,1)
			{
				// Some Stuff
			}
			Method(IQCR,1)
			{
				// Some Stuff
			}
			Device(LNKA)
			{
				Name(_HID,EisaId ("PNP0C0F"))
				Name(_UID,Zero)
				Name(_PRS,Buffer(0x00000013) {0x89,0x0E,0x00,0x09,0x03,0x05,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x00,0x00,0x79,0x00})
				Method(_STA,0)
				{
					// Some Stuff
				}
				Method(_DIS,0)
				{
					// Some Stuff
				}
				Method(_CRS,0)
				{
					// Some Stuff
				}
				Method(_SRS,1)
				{
					// Some Stuff
				}
			}
			Device(LNKB)
			{
				Name(_HID,EisaId ("PNP0C0F"))
				Name(_UID,One)
				Name(_PRS,Buffer(0x00000013) {0x89,0x0E,0x00,0x09,0x03,0x05,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x00,0x00,0x79,0x00})
				Method(_STA,0)
				{
					// Some Stuff
				}
				Method(_DIS,0)
				{
					// Some Stuff
				}
				Method(_CRS,0)
				{
					// Some Stuff
				}
				Method(_SRS,1)
				{
					// Some Stuff
				}
			}
			Device(LNKC)
			{
				Name(_HID,EisaId ("PNP0C0F"))
				Name(_UID,0x0000000000000002)
				Name(_PRS,Buffer(0x00000013) {0x89,0x0E,0x00,0x09,0x03,0x05,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x00,0x00,0x79,0x00})
				Method(_STA,0)
				{
					// Some Stuff
				}
				Method(_DIS,0)
				{
					// Some Stuff
				}
				Method(_CRS,0)
				{
					// Some Stuff
				}
				Method(_SRS,1)
				{
					// Some Stuff
				}
			}
			Device(LNKD)
			{
				Name(_HID,EisaId ("PNP0C0F"))
				Name(_UID,0x0000000000000003)
				Name(_PRS,Buffer(0x00000013) {0x89,0x0E,0x00,0x09,0x03,0x05,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x0B,0x00,0x00,0x00,0x79,0x00})
				Method(_STA,0)
				{
					// Some Stuff
				}
				Method(_DIS,0)
				{
					// Some Stuff
				}
				Method(_CRS,0)
				{
					// Some Stuff
				}
				Method(_SRS,1)
				{
					// Some Stuff
				}
			}
			Device(LNKS)
			{
				Name(_HID,EisaId ("PNP0C0F"))
				Name(_UID,0x0000000000000004)
				Name(_PRS,Buffer(0x0000000B) {0x89,0x06,0x00,0x09,0x01,0x09,0x00,0x00,0x00,0x79,0x00})
				Method(_STA,0)
				{
					// Some Stuff
				}
				Method(_DIS,0)
				{
					// Some Stuff
				}
				Method(_CRS,0)
				{
					// Some Stuff
				}
				Method(_SRS,1)
				{
					// Some Stuff
				}
			}
		}
		Scope(_SB)
		{
			Method(CPMA,1)
			{
				// Some Stuff
			}
			Method(CPST,1)
			{
				// Some Stuff
			}
			Method(CPEJ,2)
			{
				// Some Stuff
			}
			OperationRegion(PRST ,SystemIO,0xAF00,0x0020)
			Field (PRST, ByteAcc, NoLock, Preserve )
			{
				PRS, 0x0000000000000100,
			}
			Method(PRSC,0)
			{
				// Some Stuff
			}
		}
		Scope(_GPE)
		{
			Name(_HID,"ACPI0006")
				Method(_L00,0)
				{
					// Some Stuff
				}
				Method(_E01,0)
				{
					// Some Stuff
				}
				Method(_E02,0)
				{
					// Some Stuff
				}
				Method(_L03,0)
				{
					// Some Stuff
				}
				Method(_L04,0)
				{
					// Some Stuff
				}
				Method(_L05,0)
				{
					// Some Stuff
				}
				Method(_L06,0)
				{
					// Some Stuff
				}
				Method(_L07,0)
				{
					// Some Stuff
				}
				Method(_L08,0)
				{
					// Some Stuff
				}
				Method(_L09,0)
				{
					// Some Stuff
				}
				Method(_L0A,0)
				{
					// Some Stuff
				}
				Method(_L0B,0)
				{
					// Some Stuff
				}
				Method(_L0C,0)
				{
					// Some Stuff
				}
				Method(_L0D,0)
				{
					// Some Stuff
				}
				Method(_L0E,0)
				{
					// Some Stuff
				}
				Method(_L0F,0)
				{
					// Some Stuff
				}
			}
}
