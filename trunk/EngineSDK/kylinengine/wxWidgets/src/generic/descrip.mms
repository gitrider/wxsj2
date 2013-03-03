#*****************************************************************************
#                                                                            *
# Make file for VMS                                                          *
# Author : J.Jansen (joukj@hrem.stm.tudelft.nl)                              *
# Date : 9 November 1999                                                     *
#                                                                            *
#*****************************************************************************
.first
	define wx [--.include.wx]

.ifdef __WXMOTIF__
CXX_DEFINE = /define=(__WXMOTIF__=1)/name=(as_is,short)\
	   /assume=(nostdnew,noglobal_array_new)
CC_DEFINE = /define=(__WXMOTIF__=1)/name=(as_is,short)
.else
.ifdef __WXGTK__
CXX_DEFINE = /define=(__WXGTK__=1)/float=ieee/name=(as_is,short)/ieee=denorm\
	   /assume=(nostdnew,noglobal_array_new)
CC_DEFINE = /define=(__WXGTK__=1)/float=ieee/name=(as_is,short)/ieee=denorm
.else
.ifdef __WXX11__
CXX_DEFINE = /define=(__WXX11__=1,__WXUNIVERSAL__==1)/float=ieee\
	/name=(as_is,short)/assume=(nostdnew,noglobal_array_new)
CC_DEFINE = /define=(__WXX11__=1,__WXUNIVERSAL__==1)/float=ieee\
	/name=(as_is,short)
.else
CXX_DEFINE =
CC_DEFINE =
.endif
.endif
.endif

.suffixes : .cpp

.cpp.obj :
	cxx $(CXXFLAGS)$(CXX_DEFINE) $(MMS$TARGET_NAME).cpp
.c.obj :
	cc $(CFLAGS)$(CC_DEFINE) $(MMS$TARGET_NAME).c

OBJECTS = \
		busyinfo.obj,\
		calctrl.obj,\
		caret.obj,\
		choicdgg.obj,\
		colrdlgg.obj,\
		datectlg.obj,\
		dcpsg.obj,\
		dirctrlg.obj,\
		dirdlgg.obj,\
		fdrepdlg.obj,\
		fontdlgg.obj,\
		grid.obj,\
		gridctrl.obj,\
		gridsel.obj,\
		helpext.obj,\
		imaglist.obj,\
		laywin.obj,\
		listctrl.obj,\
		logg.obj,\
		msgdlgg.obj,\
		numdlgg.obj,\
		panelg.obj,\
		printps.obj,\
		prntdlgg.obj,\
		propdlg.obj,\
		progdlgg.obj,\
		renderg.obj,\
		sashwin.obj,\
		selstore.obj,\
		splitter.obj,\
		tabg.obj,\
		textdlgg.obj,\
		tipdlg.obj,\
		tipwin.obj,\
		treectlg.obj,\
		wizard.obj

SOURCES = \
		accel.cpp,\
		busyinfo.cpp,\
		calctrl.cpp,\
		caret.cpp,\
		choicdgg.cpp,\
		colrdlgg.cpp,\
		datectlg.cpp,\
		dcpsg.cpp,\
		dirctrlg.cpp,\
		dirdlgg.cpp,\
		filedlgg.cpp,\
		fdrepdlg.cpp,\
		fontdlgg.cpp,\
		grid.cpp,\
		gridctrl.cpp,\
		gridsel.cpp,\
		helpext.cpp,\
		imaglist.cpp,\
		laywin.cpp,\
		listctrl.cpp,\
		logg.cpp,\
		msgdlgg.cpp,\
		notebook.cpp,\
		numdlgg.cpp,\
		paletteg.cpp,\
		panelg.cpp,\
		printps.cpp,\
		prntdlgg.cpp,\
		propdlg.cpp,\
		progdlgg.cpp,\
		renderg.cpp,\
		sashwin.cpp,\
		selstore.cpp,\
		splitter.cpp,\
		statline.cpp,\
		statusbr.cpp,\
		tabg.cpp,\
		textdlgg.cpp,\
		tipdlg.cpp,\
		tipwin.cpp,\
		treectlg.cpp,\
		wizard.cpp,\
		dragimgg.cpp,\
		fdrepdlg.cpp,\
		htmllbox.cpp,\
		listbkg.cpp,\
		mdig.cpp,\
		scrlwing.cpp,\
		spinctlg.cpp,\
		splash.cpp,\
		timer.cpp,\
		vlbox.cpp,\
		vscroll.cpp

.ifdef __WXMOTIF__
OBJECTS0=,statusbr.obj,statline.obj,notebook.obj,scrlwing.obj,spinctlg.obj
.else
.ifdef __WXX11__
OBJECTS0=,accel.obj,filedlgg.obj,dragimgg.obj,fdrepdlg.obj,htmllbox.obj,\
	listbkg.obj,mdig.obj,scrlwing.obj,spinctlg.obj,splash.obj,timer.obj,\
	vlbox.obj,vscroll.obj
.else
OBJECTS0=,accel.obj,statusbr.obj,filedlgg.obj,paletteg.obj
.endif
.endif

all : $(SOURCES)
	$(MMS)$(MMSQUALIFIERS) $(OBJECTS)$(OBJECTS0)
.ifdef __WXMOTIF__
	library/crea [--.lib]libwx_motif.olb $(OBJECTS)$(OBJECTS0)
.else
.ifdef __WXGTK__
	library/crea [--.lib]libwx_gtk.olb $(OBJECTS)$(OBJECTS0)
.else
.ifdef __WXX11__
	library/crea [--.lib]libwx_x11_univ.olb $(OBJECTS)$(OBJECTS0)
.endif
.endif
.endif

accel.obj : accel.cpp
busyinfo.obj : busyinfo.cpp
calctrl.obj : calctrl.cpp
caret.obj : caret.cpp
choicdgg.obj : choicdgg.cpp
colrdlgg.obj : colrdlgg.cpp
datectlg.obj : datectlg.cpp
dcpsg.obj : dcpsg.cpp
dirctrlg.obj : dirctrlg.cpp
dirdlgg.obj : dirdlgg.cpp
filedlgg.obj : filedlgg.cpp
fontdlgg.obj : fontdlgg.cpp
fdrepdlg.obj : fdrepdlg.cpp
grid.obj : grid.cpp
gridctrl.obj : gridctrl.cpp
gridsel.obj : gridsel.cpp
helpext.obj : helpext.cpp
imaglist.obj : imaglist.cpp
laywin.obj : laywin.cpp
listctrl.obj : listctrl.cpp
logg.obj : logg.cpp
msgdlgg.obj : msgdlgg.cpp
notebook.obj : notebook.cpp
numdlgg.obj : numdlgg.cpp
paletteg.obj : paletteg.cpp
panelg.obj : panelg.cpp
printps.obj : printps.cpp
prntdlgg.obj : prntdlgg.cpp
progdlgg.obj : progdlgg.cpp
propdlg.obj : propdlg.cpp
scrlwing.obj : scrlwing.cpp
spinctlg.obj : spinctlg.cpp
renderg.obj : renderg.cpp
sashwin.obj : sashwin.cpp
selstore.obj : selstore.cpp
splitter.obj : splitter.cpp
statline.obj : statline.cpp
statusbr.obj : statusbr.cpp
tabg.obj : tabg.cpp
textdlgg.obj : textdlgg.cpp
tipdlg.obj : tipdlg.cpp
tipwin.obj : tipwin.cpp
treectlg.obj : treectlg.cpp
wizard.obj : wizard.cpp
dragimgg.obj : dragimgg.cpp
fdrepdlg.obj : fdrepdlg.cpp
htmllbox.obj : htmllbox.cpp
listbkg.obj : listbkg.cpp
mdig.obj : mdig.cpp
scrlwing.obj : scrlwing.cpp
spinctlg.obj : spinctlg.cpp
splash.obj : splash.cpp
timer.obj : timer.cpp
vlbox.obj : vlbox.cpp
vscroll.obj : vscroll.cpp
