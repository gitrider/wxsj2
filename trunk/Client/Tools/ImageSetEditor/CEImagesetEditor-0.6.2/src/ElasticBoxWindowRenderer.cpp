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

#include "ElasticBoxWindowRenderer.h"
#include <falagard/CEGUIFalWidgetLookFeel.h>

const CEGUI::String ElasticBoxWindowRenderer::TypeName( "CEImagesetEditor/ElasticBoxRenderer" );

ElasticBoxWindowRenderer::ElasticBoxWindowRenderer(const CEGUI::String& type) :
    CEGUI::WindowRenderer(type)
{
}

void ElasticBoxWindowRenderer::render()
{
    // get WidgetLookFeel for the assigned look.
    const CEGUI::WidgetLookFeel& wlf = getLookNFeel();
    // render imagery for current state
    wlf.getStateImagery(d_window->isActive() ? "Active" : "Inactive").render(*d_window);
}


CEGUI::WindowRenderer* ElasticBoxWRFactory::create( void )
{
    return new ElasticBoxWindowRenderer( ElasticBoxWindowRenderer::TypeName );
}

void ElasticBoxWRFactory::destroy( CEGUI::WindowRenderer* wr )
{
    delete wr;
}

ElasticBoxWRFactory& getElasticBoxWRFactory()
{
    static ElasticBoxWRFactory s_factory;
    return s_factory;
}
