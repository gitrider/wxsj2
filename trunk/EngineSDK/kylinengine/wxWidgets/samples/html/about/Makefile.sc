#Makefile from wxHatch for Symantec/Digital Mars compiler 
WXDIR=..\..\..
TARGET=about
OBJECTS = $(TARGET).obj 
EXTRALIBS =
include $(WXDIR)\src\makeprog.sc
