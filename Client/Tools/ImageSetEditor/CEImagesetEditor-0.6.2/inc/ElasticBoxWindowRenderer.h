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
#ifndef _ELASTIC_BOX_WINDOW_RENDERER_H_
#define _ELASTIC_BOX_WINDOW_RENDERER_H_

// parent class 
#include <CEGUIWindowRenderer.h>

/*!
\brief
    Window renderer for ElasticBox windows.

    LookNFeel should provide the following:

    States:
        - Active       - rendering for active state.
        - Inactive     - rendering for inactive state.
*/
class ElasticBoxWindowRenderer : public CEGUI::WindowRenderer
{
public:
    //! Type name for the WindowRenderer
    static const CEGUI::String TypeName;

    ElasticBoxWindowRenderer( const CEGUI::String& type );

    void render();
};

/*!
\brief
    Factory for ElasticBox window renderer type
*/
class ElasticBoxWRFactory : public CEGUI::WindowRendererFactory
{
public:
    ElasticBoxWRFactory( void ) : CEGUI::WindowRendererFactory( ElasticBoxWindowRenderer::TypeName ) { }
    CEGUI::WindowRenderer* create( void );
    void destroy( CEGUI::WindowRenderer* wr );
};

ElasticBoxWRFactory& getElasticBoxWRFactory();

#endif // _ELASTIC_BOX_WINDOW_RENDERER_H_
