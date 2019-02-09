# libACPI
ACPI Parser library written in C. This is a test project for now.

`AMLDecompiler` module provides a sax-like interface for AML parsing.

ACPI Specs can be found [here](http://www.acpi.info/DOWNLOADS/ACPI_5_Errata%20A.pdf)

# Project examples
These projects are examples/Usecases for libACPI.

## Device Tree constructor
Parses and generates a device tree from an AML file. This tree can then be reresented in JSON format for debug/test purposes. For now this part is written in C++ and will be later translated to C.

## AML Decompiler
Intended to work just like `iasl -d`, but not as complete for now :)








