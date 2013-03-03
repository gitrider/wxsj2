INCLUDES = -I$(top_srcdir)/inc $(CEGUI_CFLAGS) $(WX_CPPFLAGS) -DEDITOR_DATA_DIR="$(pkgdatadir)"

bin_PROGRAMS = CEImagesetEditor
CEImagesetEditor_SOURCES = DialogAbout.cpp \
                           DialogResourceGroups.cpp \
                           ElasticBox.cpp \
                           ElasticBoxProperties.cpp \
                           ElasticBoxWindowRenderer.cpp \
                           CEImagesetEditor.cpp \
                           EditorDocument.cpp \
                           EditorFrame.cpp \
                           EditorGLCanvas.cpp \
                           EditorView.cpp \
                           ImagesetHandler.cpp \
                           PropertiesPanel.cpp \
                           wxPathCellEditor.cpp

CEImagesetEditor_LDADD = $(CEGUI_LIBS) $(WX_LIBS)
