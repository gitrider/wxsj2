///////////////////////////////////////////////////////////////////////////////
//  For project details and authors, refer to README and AUTHORS files
//
//  This file is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This file is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//  To view the licence online, go to: http://www.gnu.org/copyleft/gpl.html
////////////////////////////////////////////////////////////////////////////////
#ifndef _ELASTIC_BOX_WIDGET_H_
#define _ELASTIC_BOX_WIDGET_H_

// parent class
#include <CEGUIWindow.h> 
// for factory creation
#include <CEGUIWindowFactory.h>
// we reuse the SizingLocation enumeration from FrameWindow
#include <elements/CEGUIFrameWindow.h> 
// properties for this class
#include "ElasticBoxProperties.h"

/*!
\brief
    Class for a bounding box widget.

    The elastic box widget is a custom widget for the CEImagesetEditor; it
    provides a widget that can be drag sized and drag moved.  The purpose of
    the widget is to provide a means to visually define some region.

    ElasticBox is based somewhat on FrameWindow, from the core CEGUI
    library.
*/
class ElasticBox : public CEGUI::Window
{
public:
    //! Namespace for global events
    static const CEGUI::String EventNamespace;
    //! Window factory name
    static const CEGUI::String WidgetTypeName;
    //! Event fired when the cursor is to be changed to normal
    static const CEGUI::String EventSetNormalCursor;
    //! Event fired when the cursor is to be changed for moving
    static const CEGUI::String EventSetMoveCursor;
    //! Event fired when the cursor is to be changed for east / west sizing
    static const CEGUI::String EventSetEastWestCursor;
    //! Event fired when the cursor is to be changed for north / south sizing
    static const CEGUI::String EventSetNorthSouthCursor;
    //! Event fired when the cursor is to be changed for north-east / south-west sizing
    static const CEGUI::String EventSetNorthEastSouthWestCursor;
    //! Event fired when the cursor is to be changed for north-west / south-east sizing
    static const CEGUI::String EventSetNorthWestSouthEastCursor;
    //! Default size for the sizing border (in pixels)
    static const float DefaultSizingBorderSize;

    /*!
    \brief
        Constructor for ElasticBox objects.
    */
    ElasticBox( const CEGUI::String& name, const CEGUI::String& type );

    /*!
    \brief
        Destructor for ElasticBox objects.
    */
    virtual ~ElasticBox( void );

    /*!
    \brief
        Return whether this window is sizable (by the user).

    \return
        - true if the window can be sized
        - false if the window can not be sized
    */
    bool isSizable( void ) const;

    /*!
    \brief
        Return whether the window is movable (by the user).

    \return
        - true if the window can be moved around
        - false if the window is fixed in position
    */
    bool isMovable( void ) const;

    /*!
    \brief
        Return the thickness of the sizing border in pixels.

    \return
        float value describing the thickness of the sizing border in pixels.
    */
    float getSizingBorderThickness( void ) const;

    /*!
    \brief
        Returns the current scale 'snap' value.

        The scale snap value is used to limit sizing and moving operations to
        pixel multiples of the scale snap value (useful when operating upon a
        scaled image for example).  Defaults to 1.

    \return
        float value describing the current scale snap value.
    */
    float getScaleSnap( void ) const;

    /*!
    \brief
        Enables or disables sizing (by the user) for this window.

    \param setting
        - true to enable sizing
        - false to disable sizing.

    \return
        Nothing
    */
    void setSizable( bool setting );

    /*!
    \brief
        Enables or disables moving (by the user) for this window.

    \param setting
        - true to enable moving of the window
        - false to disable moving of the window

    \return
        Nothing.
    */
    void setMovable( bool setting );

    /*!
    \brief
        Set the size of the sizing border for this window in pixels.

    \param pixels
        float value specifying the thickness for the sizing border in pixels.

    \return
        Nothing.
    */
    void setSizingBorderThickness( float pixels );

    /*!
    \brief
        Sets the current scale 'snap' value.

        The scale snap value is used to limit sizing and moving operations to
        pixel multiples of the scale snap value (useful when operating upon a
        scaled image for example).  Defaults to 1.

    \param value
        float value describing the scale snap value to be used.

    \return
        Nothing
    */
    void setScaleSnap( float value );

    // Base class implementations and overrides
    virtual void initialiseComponents( void );

protected:
    /*!
    \brief
        move the window's left edge by 'delta'.  The rest of the window does
        not move, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in
        which direction.  Positive values make window smaller.
    */
    void moveLeftEdge( float delta );

    /*!
    \brief
        move the window's right edge by 'delta'.  The rest of the window does
        not move, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in
        which direction.  Positive values make window larger.
    */
    void moveRightEdge( float delta );

    /*!
    \brief
        move the window's top edge by 'delta'.  The rest of the window does
        notmove, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in
        which direction.  Positive values make window smaller.
    */
    void moveTopEdge( float delta );

    /*!
    \brief
        move the window's bottom edge by 'delta'.  The rest of the window does
        not move, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in
        which direction.  Positive values make window larger.
    */
    void moveBottomEdge( float delta );

    /*!
    \brief
        check local pixel co-ordinate point 'pt' and return one of the
        CEGUI::FrameWindow::SizingLocation enumerated values depending where
        the point falls on the sizing border.

    \param pt
        Point object describing, in pixels, the window relative offset to check.

    \return
        One of the CEGUI::FrameWindow::SizingLocation enumerated values that
        describe which part of the sizing border that \a pt corresponded to,
        if any.
    */
    CEGUI::FrameWindow::SizingLocation getSizingBorderAtPoint( const CEGUI::Point& pt ) const;

    /*!
    \brief
        return true if given CEGUI::FrameWindow::SizingLocation is on left edge.

    \param loc
        CEGUI::FrameWindow::SizingLocation value to be checked.

    \return
        - true if \a loc is on the left edge.
        - false if \a loc is not on the left edge.
    */
    bool isLeftSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const;

    /*!
    \brief
        return true if given CEGUI::FrameWindow::SizingLocation is on right edge.

    \param loc
        CEGUI::FrameWindow::SizingLocation value to be checked.

    \return
        - true if \a loc is on the right edge.
        - false if \a loc is not on the right edge.
    */
    bool isRightSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const;

    /*!
    \brief
        return true if given CEGUI::FrameWindow::SizingLocation is on top edge.

    \param loc
        CEGUI::FrameWindow::SizingLocation value to be checked.

    \return
        - true if \a loc is on the top edge.
        - false if \a loc is not on the top edge.
    */
    bool isTopSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const;

    /*!
    \brief
        return true if given CEGUI::FrameWindow::SizingLocation is on bottom edge.

    \param loc
        CEGUI::FrameWindow::SizingLocation value to be checked.

    \return
        - true if \a loc is on the bottom edge.
        - false if \a loc is not on the bottom edge.
    */
    bool isBottomSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const;

    /*!
    \brief
        Return a Rect that describes, in window relative pixel co-ordinates,
        the outer edge of the sizing area for this window.
    */
    CEGUI::Rect getSizingRect( void ) const;

    /*!
    \brief
        Handles the update logic for sizing operations.

    \param mousePosition
        Latest mouse position in window local pixel co-ordinates.
    */
    void doSizingUpdate( const CEGUI::Point& mousePosition );

    /*!
    \brief
        Handles the update logic for moving operations.

    \param mousePosition
        Latest mouse position in window local pixel co-ordinates.
    */
    void doMovingUpdate( const CEGUI::Point& mousePosition );

    /*!
    \brief
        Call appropriate handler to process cursor change requirements.

        The base class handlers provided here just fire events.
    */
    void callCursorChangeHandler( const CEGUI::Point& mousePosition );

    /*!
    \brief
        Helper to calculate snapped deltas on the x axis.
    */
    float calculateSnappedXDelta(float delta);

    /*!
    \brief
        Helper to calculate snapped deltas on the y axis.
    */
    float calculateSnappedYDelta(float delta);

    /*!
    \brief
        Return whether this window was inherited from the given class name at
        some point in the inheritance hierarchy.

    \param class_name
        The class name that is to be checked.

    \return
        - true if this window was inherited from \a class_name.
        - false if this window was not inherited from \a class_name.
    */
    virtual bool testClassName_impl( const CEGUI::String& class_name ) const;

    /*!
    \brief
        Handler called when the cursor is to be changed back to normal.
    */
    virtual void onSetNormalCursor( CEGUI::WindowEventArgs& e );

    /*!
    \brief
        Handler called when the cursor is to be changed for movement.
    */
    virtual void onSetMoveCursor( CEGUI::WindowEventArgs& e );

    /*!
    \brief
        Handler called when the cursor is to be changed for east / west sizing.
    */
    virtual void onSetEastWestCursor( CEGUI::WindowEventArgs& e );

    /*!
    \brief
        Handler called when the cursor is to be changed for north / south sizing.
    */
    virtual void onSetNorthSouthCursor( CEGUI::WindowEventArgs& e );

    /*!
    \brief
        Handler called when the cursor is to be changed for north-east / south-west sizing.
    */
    virtual void onSetNorthEastSouthWestCursor( CEGUI::WindowEventArgs& e );

    /*!
    \brief
        Handler called when the cursor is to be changed for north-west / south-east sizing.
    */
    virtual void onSetNorthWestSouthEastCursor( CEGUI::WindowEventArgs& e );

    // Overridden event processing
    virtual void onMouseMove( CEGUI::MouseEventArgs& e );
    virtual void onMouseButtonDown( CEGUI::MouseEventArgs& e );
    virtual void onMouseButtonUp( CEGUI::MouseEventArgs& e );
    virtual void onCaptureLost( CEGUI::WindowEventArgs& e );

    // Implementation Data
    //! true if window is user-sizable.
    bool d_sizable;
    //! true if the window is user-movable
    bool d_movable;
    //! true if window is currently being sized.
    bool d_beingSized;
    //! true if window is currently being moved.
    bool d_beingMoved;
    //! thickness of the sizing border around this window
    float d_borderThickness;
    //! current scale snap value
    float d_scaleSnap;
    //! tracks x pixels currently dragged - used for scale snap implementation.
    float d_xSnapPixelsMoved; 
    //! tracks y pixels currently dragged - used for scale snap implementation.
    float d_ySnapPixelsMoved; 
    //! point window is being dragged at.
    CEGUI::Point d_dragPoint;
    /** Tracks last sizing location we knew about.  Required because at high
     * zoom - due to 'snap' - we're likely to move the mouse off the edge of the
     * window which will then not show up as a sizing location, causing very
     * rough response.
    */
    CEGUI::FrameWindow::SizingLocation d_lastSizingLocation;

private:
    //! Propery to access the sizable state
    static ElasticBoxProperties::Sizable d_sizableProperty;
    //! Property to access the movable state
    static ElasticBoxProperties::Movable d_movableProperty;
    //! Property to access the sizing border thickness setting
    static ElasticBoxProperties::SizingBorderThickness d_borderThicknessProperty;

    //! Adds properties to the window!
    void addElasticBoxProperties( void );
};

/*!
\brief
    WindowFactory class for ElasticBox objects.
*/
class ElasticBoxFactory : public CEGUI::WindowFactory
{
public:
    ElasticBoxFactory() : CEGUI::WindowFactory( ElasticBox::WidgetTypeName ) {}
    CEGUI::Window* createWindow(const CEGUI::String& name)
    { return new ElasticBox(d_type, name); }
    void destroyWindow(CEGUI::Window* window)
    { delete window; }
};

ElasticBoxFactory& getElasticBoxFactory();

#endif // _ELASTIC_BOX_WIDGET_H_
