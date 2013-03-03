#!################################################################################
#! File:    mgl.t
#! Purpose: tmake template file from which src/mgl/files.lst containing the
#!          list of files for wxMGL library is generated by tmake
#! Author:  Vadim Zeitlin
#! Created: 28.01.00
#! Version: $Id: mgl.t,v 1.8 2002/12/04 13:56:30 VZ Exp $
#!################################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric, %wxHtml, %wxUNIX, %wxGTK, %wxMOTIF and
    #! %wxOS2PM hashes.
    IncludeTemplate("filelist.t");

    #! find all our sources

    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\bNotMGL\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MGL_SOURCES"} .= "generic/" . $file . " ";
        $project{"GENERICOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\bNotMGL\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MGL_SOURCES"} .= "common/" . $file . " ";
        $project{"COMMONOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxMGL) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MGL_SOURCES"} .= "mgl/" . $file . " ";
#!        $project{"GUIOBJS"} .= $fileobj . " ";

        if ( $wxMGL{$file} =~ /\bL\b/ ) {
            $project{"GUI_LOWLEVEL_OBJS"} .= $fileobj . " ";
        }
    }

    foreach $file (sort keys %wxUNIX) {
        next if $wxUNIX{$file} =~ /\bNotMGL\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MGL_SOURCES"} .= "unix/" . $file . " ";
        $project{"UNIXOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxHTML) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"MGL_SOURCES"} .= "html/" . $file . " ";
        $project{"HTMLOBJS"} .= $fileobj . " ";
    }

    #! find all our headers
    foreach $file (sort keys %wxWXINCLUDE) {
        $project{"MGL_HEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxMGLINCLUDE) {
        $project{"MGL_HEADERS"} .= "mgl/" . $file . " "
    }

    foreach $file (sort keys %wxGENERICINCLUDE) {
        $project{"MGL_HEADERS"} .= "generic/" . $file . " "
    }

    foreach $file (sort keys %wxUNIXINCLUDE) {
        $project{"MGL_HEADERS"} .= "unix/" . $file . " "
    }

    foreach $file (sort keys %wxHTMLINCLUDE) {
        $project{"MGL_HEADERS"} .= "html/" . $file . " "
    }

    foreach $file (sort keys %wxPROTOCOLINCLUDE) {
        $project{"MGL_HEADERS"} .= "protocol/" . $file . " "
    }
#$}
# This file was automatically generated by tmake 
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE MGL.T!
ALL_SOURCES = \
		#$ ExpandList("MGL_SOURCES");

ALL_HEADERS = \
		#$ ExpandList("MGL_HEADERS");

COMMONOBJS = \
		#$ ExpandList("COMMONOBJS");

GENERICOBJS = \
		#$ ExpandList("GENERICOBJS");

#!GUIOBJS = \
#!		#$ ExpandList("GUIOBJS");
#!
GUI_LOWLEVEL_OBJS = \
		#$ ExpandList("GUI_LOWLEVEL_OBJS");

UNIXOBJS = \
		#$ ExpandList("UNIXOBJS");

HTMLOBJS = \
		#$ ExpandList("HTMLOBJS");

