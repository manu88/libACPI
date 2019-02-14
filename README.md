# libACPI
ACPI Parser library written in C. This is a test project for now.

`AMLDecompiler` module provides a sax-like interface for AML parsing.

ACPI Specs can be found [here](http://www.uefi.org/sites/default/files/resources/ACPI%206_2_A_Sept29.pdf)

# Project examples
These projects are examples/Usecases for libACPI.

## Device Tree constructor
Parses and generates a device tree from an AML file. This tree can then be reresented in JSON format for debug/test purposes. For now this part is written in C++ and will be later translated to C.

## AML Decompiler
Intended to work just like `iasl -d`, but not as complete for now :)

## What's working
The 'TestSuite' folder contains asl files, compiled into aml files (with iasl), and describded in JSON files. Basically everything listed in asl files is working.






