// Copyright (c) 1997-2002  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Qt_widget/include/CGAL/IO/Qt_help_window.h $
// $Id: Qt_help_window.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Radu Ursu

#ifndef CGAL_QT_HELP_WINDOW_H
#define CGAL_QT_HELP_WINDOW_H

#include <qmainwindow.h>
#include <qtextbrowser.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qdir.h>
#include <qstatusbar.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qiconset.h>
#include <qfile.h>
#include <qstylesheet.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qcombobox.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qobjectlist.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qprinter.h>
#include <qsimplerichtext.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>


class QComboBox;
class QPopupMenu;

namespace CGAL{

class Qt_help_window : public QMainWindow
{
    Q_OBJECT
public:
    Qt_help_window( const QString& home_,  const QString& path, 
                QWidget* parent = 0, const char *name=0 );
    ~Qt_help_window();
public slots:
    void print();

private slots:
    void setBackwardAvailable( bool );
    void setForwardAvailable( bool );
    void pathSelected( const QString & );
    void histChosen( int );

private:
    void readHistory();

    QTextBrowser* browser;
    QComboBox *pathCombo;
    int backwardId, forwardId;
    QStringList history;
    QPopupMenu *hist;
    QMap<int, QString> mHistory;
};

} //end CGAL namespace

#endif
