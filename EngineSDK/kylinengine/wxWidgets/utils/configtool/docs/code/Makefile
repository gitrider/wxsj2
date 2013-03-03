#
# File:		Makefile
# Author:	Julian Smart
# Created:	1999
#
# Makefile : Builds code documentation on Windows
#

DOCDIR=..
LOCALDOCDIR=.
DOXYGEN="c:\Program Files\doxygen\bin\doxygen.exe"

all: html/genericapp.chm html/genericapp.htb

clean:
    -erase /Y html\*.html html\*.htb html\*.chm

run:
    -start html\genericapp.chm

htb: html/genericapp.htb
html: html/genericapp.html
htmlhelp: html/genericapp.chm
pdfrtf: genericapp_rtf.rtf

html/genericapp.chm : html/index.html html/index.hhp
    cd html
	-hhc index.hhp
    cd ..

genericapp_rtf.rtf: $(DOCSOURCES) tex2rtf.ini
        -start /w tex2rtf genericapp.tex genericapp_rtf.rtf -twice -rtf

genericapp.rtf: $(DOCSOURCES) tex2rtf.ini
        -start /w tex2rtf genericapp.tex genericapp.rtf -twice -winhelp

html/index.html:         
        $(DOXYGEN) doxysettings.doxy

html/genericapp.htb:    html/index.html
        cd html
        -erase /Y genericapp.htb
        -zip genericapp.htb *.html *.hhp *.hhk *.hhc *.gif *.png
        cd ..

