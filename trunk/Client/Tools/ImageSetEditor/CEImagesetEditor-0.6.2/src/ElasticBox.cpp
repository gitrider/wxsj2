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
#include "ElasticBox.h"
#include <CEGUICoordConverter.h>
#include <cmath>

using namespace CEGUI;

// Namespace for global events
const CEGUI::String ElasticBox::EventNamespace( "ElasticBox" );
// Window factory name
const CEGUI::String ElasticBox::WidgetTypeName( "CEImagesetEditor/ElasticBox" );
// Default size for the sizing border (in pixels)
const float ElasticBox::DefaultSizingBorderSize = 3.0f;
// Event fired when the cursor is to be changed to normal
const CEGUI::String ElasticBox::EventSetNormalCursor( "SetNormalCursor" );
// Event fired when the cursor is to be changed for moving
const CEGUI::String ElasticBox::EventSetMoveCursor( "SetMoveCursor" );
// Event fired when the cursor is to be changed for east / west sizing
const CEGUI::String ElasticBox::EventSetEastWestCursor( "SetEastWestCursor" );
// Event fired when the cursor is to be changed for north / south sizing
const CEGUI::String ElasticBox::EventSetNorthSouthCursor( "SetNorthSouthCursor" );
// Event fired when the cursor is to be changed for north-east / south-west sizing
const CEGUI::String ElasticBox::EventSetNorthEastSouthWestCursor( "SetNorthEastSouthWestCursor" );
// Event fired when the cursor is to be changed for north-west / south-east sizing
const CEGUI::String ElasticBox::EventSetNorthWestSouthEastCursor( "SetNorthWestSouthEastCursor" );
// Propery to access the sizable state
ElasticBoxProperties::Sizable ElasticBox::d_sizableProperty;
// Property to access the movable state
ElasticBoxProperties::Movable ElasticBox::d_movableProperty;
// Property to access the sizing border thickness setting
ElasticBoxProperties::SizingBorderThickness ElasticBox::d_borderThicknessProperty;

//-----------------------------------------------------------------------
ElasticBox::ElasticBox( const CEGUI::String& name, const CEGUI::String& type ) :
        Window( name, type ),
        d_sizable( true ),
        d_movable( true ),
        d_beingSized( false ),
        d_beingMoved( false ),
        d_borderThickness( DefaultSizingBorderSize ),
        d_scaleSnap( 1 ),
        d_xSnapPixelsMoved( 0 ),
        d_ySnapPixelsMoved( 0 ),
        d_lastSizingLocation( FrameWindow::SizingNone )
{
    addElasticBoxProperties();
}

//-----------------------------------------------------------------------
ElasticBox::~ElasticBox( void )
{}

//-----------------------------------------------------------------------
bool ElasticBox::isSizable( void ) const
{
    return d_sizable;
}

//-----------------------------------------------------------------------
bool ElasticBox::isMovable( void ) const
{
    return d_movable;
}

//-----------------------------------------------------------------------
float ElasticBox::getSizingBorderThickness( void ) const
{
    return d_borderThickness;
}

//-----------------------------------------------------------------------
float ElasticBox::getScaleSnap( void ) const
{
    return d_scaleSnap;
}

//-----------------------------------------------------------------------
void ElasticBox::setSizable( bool setting )
{
    d_sizable = setting;

    if ( d_beingSized )
        releaseInput();
}

//-----------------------------------------------------------------------
void ElasticBox::setMovable( bool setting )
{
    d_movable = setting;

    if ( d_beingMoved )
        releaseInput();
}

//-----------------------------------------------------------------------
void ElasticBox::setSizingBorderThickness( float pixels )
{
    d_borderThickness = pixels;
}

//-----------------------------------------------------------------------
void ElasticBox::initialiseComponents( void )
{
    // Nothing to do here at the moment; we have no components.
}

//-----------------------------------------------------------------------
void ElasticBox::setScaleSnap( float value )
{
    d_scaleSnap = value;
    d_xSnapPixelsMoved = 0.0f;
    d_ySnapPixelsMoved = 0.0f;
}

//-----------------------------------------------------------------------
void ElasticBox::moveLeftEdge( float delta )
{
    // TODO: We do not currently deal with alignments other than HA_LEFT

    URect area( d_area );
    area.d_min.d_x.d_offset += delta;
    setArea( area );
}

//-----------------------------------------------------------------------
void ElasticBox::moveRightEdge( float delta )
{
    // TODO: We do not currently deal with alignments other than HA_LEFT

    float orgWidth = d_pixelSize.d_width;

    URect area( d_area );
    area.d_max.d_x.d_offset += delta;
    setArea( area );

    // move the dragging point so mouse remains attached to edge of window
    d_dragPoint.d_x += d_pixelSize.d_width - orgWidth;
}

//-----------------------------------------------------------------------
void ElasticBox::moveTopEdge( float delta )
{
    // TODO: We do not currently deal with alignments other than VA_TOP

    URect area( d_area );
    area.d_min.d_y.d_offset += delta;
    setArea( area );
}

//-----------------------------------------------------------------------
void ElasticBox::moveBottomEdge( float delta )
{
    // TODO: We do not currently deal with alignments other than VA_TOP

    float orgHeight = d_pixelSize.d_height;

    URect area( d_area );
    area.d_max.d_y.d_offset += delta;
    setArea( area );

    // move the dragging point so mouse remains attached to edge of window
    d_dragPoint.d_y += d_pixelSize.d_height - orgHeight;
}

//-----------------------------------------------------------------------
CEGUI::FrameWindow::SizingLocation
ElasticBox::getSizingBorderAtPoint( const CEGUI::Point& pt ) const
{
    CEGUI::Rect frame( getSizingRect() );

    // we can only size if the frame is enabled and sizing is on
    if ( isSizable() )
    {
        // point must be inside the outer edge
        if ( frame.isPointInRect( pt ) )
        {
            // adjust rect to get inner edge
            frame.d_left += d_borderThickness;
            frame.d_top += d_borderThickness;
            frame.d_right -= d_borderThickness;
            frame.d_bottom -= d_borderThickness;

            // detect which edges we are on
            bool top = ( pt.d_y < frame.d_top );
            bool bottom = ( pt.d_y >= frame.d_bottom );
            bool left = ( pt.d_x < frame.d_left );
            bool right = ( pt.d_x >= frame.d_right );

            // return appropriate 'SizingLocation' value
            if ( top && left )
                return FrameWindow::SizingTopLeft;
            else if ( top && right )
                return FrameWindow::SizingTopRight;
            else if ( bottom && left )
                return FrameWindow::SizingBottomLeft;
            else if ( bottom && right )
                return FrameWindow::SizingBottomRight;
            else if ( top )
                return FrameWindow::SizingTop;
            else if ( bottom )
                return FrameWindow::SizingBottom;
            else if ( left )
                return FrameWindow::SizingLeft;
            else if ( right )
                return FrameWindow::SizingRight;
        }
    }

    // deafult: None.
    return FrameWindow::SizingNone;
}
//-----------------------------------------------------------------------
bool ElasticBox::isLeftSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const
{
    return ( ( loc == CEGUI::FrameWindow::SizingLeft ) ||
             ( loc == CEGUI::FrameWindow::SizingTopLeft ) ||
             ( loc == CEGUI::FrameWindow::SizingBottomLeft ) );
}

//-----------------------------------------------------------------------
bool ElasticBox::isRightSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const
{
    return ( ( loc == CEGUI::FrameWindow::SizingRight ) ||
             ( loc == CEGUI::FrameWindow::SizingTopRight ) ||
             ( loc == CEGUI::FrameWindow::SizingBottomRight ) );
}

//-----------------------------------------------------------------------
bool ElasticBox::isTopSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const
{
    return ( ( loc == CEGUI::FrameWindow::SizingTop ) ||
             ( loc == CEGUI::FrameWindow::SizingTopLeft ) ||
             ( loc == CEGUI::FrameWindow::SizingTopRight ) );
}

//-----------------------------------------------------------------------
bool ElasticBox::isBottomSizingLocation( CEGUI::FrameWindow::SizingLocation loc ) const
{
    return ( ( loc == CEGUI::FrameWindow::SizingBottom ) ||
             ( loc == CEGUI::FrameWindow::SizingBottomLeft ) ||
             ( loc == CEGUI::FrameWindow::SizingBottomRight ) );
}

//-----------------------------------------------------------------------
CEGUI::Rect ElasticBox::getSizingRect( void ) const
{
    return CEGUI::Rect( 0, 0, d_pixelSize.d_width, d_pixelSize.d_height );
}

//-----------------------------------------------------------------------
void ElasticBox::doSizingUpdate( const CEGUI::Point& mousePosition )
{
    // calculate deltas to use for sizing
    float delta_x = calculateSnappedXDelta( mousePosition.d_x - d_dragPoint.d_x );
    float delta_y = calculateSnappedYDelta( mousePosition.d_y - d_dragPoint.d_y );

    // sizing for left or right edges
    if ( isLeftSizingLocation( d_lastSizingLocation ) )
        moveLeftEdge( delta_x );
    else if ( isRightSizingLocation( d_lastSizingLocation ) )
        moveRightEdge( delta_x );

    // sizing for top or bottom edges
    if ( isTopSizingLocation( d_lastSizingLocation ) )
        moveTopEdge( delta_y );
    else if ( isBottomSizingLocation( d_lastSizingLocation ) )
        moveBottomEdge( delta_y );
}

//-----------------------------------------------------------------------
void ElasticBox::doMovingUpdate( const CEGUI::Point& mousePosition )
{
    // calculate amount that window has been moved in pixels
    float delta_x = calculateSnappedXDelta( mousePosition.d_x - d_dragPoint.d_x );
    float delta_y = calculateSnappedYDelta( mousePosition.d_y - d_dragPoint.d_y );

    // calculate offset
    UVector2 offset(
        cegui_absdim( delta_x ),
        cegui_absdim( delta_y ) );

    URect area(d_area);
    area.offset( offset );

    // update position
    setArea( area );
}

//-----------------------------------------------------------------------
void ElasticBox::callCursorChangeHandler( const CEGUI::Point& mousePosition )
{
    WindowEventArgs args( this );

    if ( isSizable() )
    {
        switch ( getSizingBorderAtPoint( mousePosition ) )
        {
        case FrameWindow::SizingTop:
        case FrameWindow::SizingBottom:
            onSetNorthSouthCursor( args );
            return ;

        case FrameWindow::SizingLeft:
        case FrameWindow::SizingRight:
            onSetEastWestCursor( args );
            return ;

        case FrameWindow::SizingTopLeft:
        case FrameWindow::SizingBottomRight:
            onSetNorthWestSouthEastCursor( args );
            return ;

        case FrameWindow::SizingTopRight:
        case FrameWindow::SizingBottomLeft:
            onSetNorthEastSouthWestCursor( args );
            return ;
        }
    }

    if ( isMovable() )
        onSetMoveCursor( args );
    else
        onSetNormalCursor( args );
}

//-----------------------------------------------------------------------
float ElasticBox::calculateSnappedXDelta(float delta)
{
    // update the drag point for part moves
    d_dragPoint.d_x += delta;
    // track the extent of part moves made so far.
    d_xSnapPixelsMoved += delta;

    // have we moved the equivalent of 1 whole pixel at this scale setting?
    if (std::fabs(d_xSnapPixelsMoved) > d_scaleSnap)
    {
        // this is the number of whole pixels moved if we were at scale level 1
        delta = (static_cast<int>(d_xSnapPixelsMoved) / static_cast<int>(d_scaleSnap)) * d_scaleSnap;
        // reset drag point now we are actually moving the window
        d_dragPoint.d_x -= delta;
        // retain any partial pixels moved
        d_xSnapPixelsMoved -= delta;
    }
    else
    {
        delta = 0.0f;
    }

    return delta;
}

//-----------------------------------------------------------------------
float ElasticBox::calculateSnappedYDelta(float delta)
{
    // update the drag point for part moves
    d_dragPoint.d_y += delta;
    // track the extent of part moves made so far.
    d_ySnapPixelsMoved += delta;

    // have we moved the equivalent of 1 whole pixel at this scale setting?
    if (std::fabs(d_ySnapPixelsMoved) > d_scaleSnap)
    {
        // this is the number of whole pixels moved if we were at scale level 1
        delta = (static_cast<int>(d_ySnapPixelsMoved) / static_cast<int>(d_scaleSnap)) * d_scaleSnap;
        // reset drag point now we are actually moving the window
        d_dragPoint.d_y -= delta;
        // retain any partial pixels moved
        d_ySnapPixelsMoved -= delta;
    }
    else
    {
        delta = 0.0f;
    }

    return delta;
}

//-----------------------------------------------------------------------
bool ElasticBox::testClassName_impl( const CEGUI::String& class_name ) const
{
    if ( class_name == "ElasticBox" )
        return true;
    else
        return Window::testClassName_impl( class_name );
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNormalCursor( CEGUI::WindowEventArgs& e )
{
    fireEvent( EventSetNormalCursor, e, EventNamespace );
}

//-----------------------------------------------------------------------
void ElasticBox::onSetMoveCursor( CEGUI::WindowEventArgs& e )
{
    fireEvent( EventSetMoveCursor, e, EventNamespace );
}

//-----------------------------------------------------------------------
void ElasticBox::onSetEastWestCursor( CEGUI::WindowEventArgs& e )
{
    fireEvent( EventSetEastWestCursor, e, EventNamespace );
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNorthSouthCursor( CEGUI::WindowEventArgs& e )
{
    fireEvent( EventSetNorthSouthCursor, e, EventNamespace );
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNorthEastSouthWestCursor( CEGUI::WindowEventArgs& e )
{
    fireEvent( EventSetNorthEastSouthWestCursor, e, EventNamespace );
}

//-----------------------------------------------------------------------
void ElasticBox::onSetNorthWestSouthEastCursor( CEGUI::WindowEventArgs& e )
{
    fireEvent( EventSetNorthWestSouthEastCursor, e, EventNamespace );
}

//-----------------------------------------------------------------------
void ElasticBox::onMouseMove( CEGUI::MouseEventArgs& e )
{
    // Do default processing and fire off event.
    Window::onMouseMove( e );

    // if we are not the window containing the mouse, do nothing more
    if ( System::getSingleton().getWindowContainingMouse() != this )
        return ;

    CEGUI::Point localMousePos( CoordConverter::screenToWindow( *this, e.position ) );

    if ( d_beingSized )
        doSizingUpdate( localMousePos );
    else if ( d_beingMoved )
        doMovingUpdate( localMousePos );
    else
        callCursorChangeHandler( localMousePos );

    // mark event as handled
    e.handled = true;
}

//-----------------------------------------------------------------------
void ElasticBox::onMouseButtonDown( CEGUI::MouseEventArgs& e )
{
    // call default handler to fire event
    Window::onMouseButtonDown( e );

    // we're only really interested in the left button
    if ( e.button == LeftButton )
    {
        // get position of mouse as co-ordinates local to this window.
        CEGUI::Point localPos( CoordConverter::screenToWindow( *this, e.position ) );

        // get sizing border location of the mouse
        d_lastSizingLocation = getSizingBorderAtPoint( localPos );

        if ( isSizable() && ( d_lastSizingLocation != FrameWindow::SizingNone ) )
        {
            // ensure all inputs come to us for now
            if ( captureInput() )
            {
                // setup the 'dragging' state variables
                d_beingSized = true;
                d_dragPoint = localPos;
                e.handled = true;
            }
        }
        else if ( isMovable() && ( d_lastSizingLocation == FrameWindow::SizingNone ) )
        {
            // ensure all inputs come to us for now
            if ( captureInput() )
            {
                // setup the 'dragging' state variables
                d_beingMoved = true;
                d_dragPoint = localPos;
                e.handled = true;
            }
        }
    }
}
//-----------------------------------------------------------------------
void ElasticBox::onMouseButtonUp( CEGUI::MouseEventArgs& e )
{
    // call default handler to fire event
    Window::onMouseButtonUp( e );

    if ( e.button == LeftButton && isCapturedByThis() )
    {
        // release our capture on the input data
        // (eventually this causes drag state reset)
        releaseInput();
        e.handled = true;
    }

}

//-----------------------------------------------------------------------
void ElasticBox::onCaptureLost( CEGUI::WindowEventArgs& e )
{
    Window::onCaptureLost( e );

    // reset drag states
    d_beingSized = false;
    d_beingMoved = false;
    d_xSnapPixelsMoved = 0;
    d_ySnapPixelsMoved = 0;

    e.handled = true;
}

//-----------------------------------------------------------------------
void ElasticBox::addElasticBoxProperties( void )
{
    addProperty( &d_sizableProperty );
    addProperty( &d_movableProperty );
    addProperty( &d_borderThicknessProperty );
}

ElasticBoxFactory& getElasticBoxFactory()
{
    static ElasticBoxFactory s_factory;
    return s_factory;
}
