/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// FILE: ControlBarLayout.cpp /////////////////////////////////////////////////////////////////////

#include "PreRTS.h"	// This must go first in EVERY cpp file in the GameEngine

#include <map>

#include "GameClient/ControlBarLayout.h"
#include "GameClient/ControlBar.h"
#include "GameClient/Display.h"
#include "GameClient/GameWindow.h"

ControlBarLayoutSettings TheControlBarLayoutSettings;

//-------------------------------------------------------------------------------------------------
ControlBarLayout getControlBarLayout( Real creationWidth, Real creationHeight, ControlBarAnchor anchor )
{
	const Real displayWidth = (Real)TheDisplay->getWidth();
	const Real displayHeight = (Real)TheDisplay->getHeight();

	ControlBarLayout layout;
	layout.scale.x = displayWidth / creationWidth;
	layout.scale.y = displayHeight / creationHeight;
	layout.offset.x = 0.0f;
	layout.offset.y = 0.0f;
	layout.uniform = FALSE;

	Bool uniform = TheControlBarLayoutSettings.mode == CONTROLBAR_LAYOUT_UNIFORM;
	if( TheControlBarLayoutSettings.mode == CONTROLBAR_LAYOUT_AUTO )
		uniform = displayWidth / displayHeight > 16.0f / 9.0f + 0.01f;

	if( uniform )
	{
		const Real userScale = clamp( 0.5f, TheControlBarLayoutSettings.scale, 1.0f );
		const Real scale = min( layout.scale.x, layout.scale.y ) * userScale;
		layout.scale.x = scale;
		layout.scale.y = scale;
		if( anchor == CONTROLBAR_ANCHOR_RIGHT )
			layout.offset.x = displayWidth - creationWidth * scale;
		else
			layout.offset.x = ( displayWidth - creationWidth * scale ) * 0.5f;
		layout.offset.y = displayHeight - creationHeight * scale;
		layout.uniform = TRUE;
	}

	return layout;
}

//-------------------------------------------------------------------------------------------------
ControlBarAnchor getControlBarAnchorForWindowFile( const AsciiString& filename )
{
	// the generals promotion menu and the build tooltip are authored around the control bar
	if( filename.compareNoCase( "ControlBar.wnd" ) == 0
		|| filename.compareNoCase( "GeneralsExpPoints.wnd" ) == 0
		|| filename.compareNoCase( "ControlBarPopupDescription.wnd" ) == 0 )
		return CONTROLBAR_ANCHOR_CENTER;

	// GenPowersShortcutBarUS.wnd, ...China.wnd, ...GLA.wnd sit at the right screen edge
	if( filename.startsWithNoCase( "GenPowersShortcutBar" ) )
		return CONTROLBAR_ANCHOR_RIGHT;

	return CONTROLBAR_ANCHOR_NONE;
}

//-------------------------------------------------------------------------------------------------
namespace
{
	struct ControlBarWindowRect
	{
		IRegion2D rect;			///< as authored in the WND file
		ICoord2D res;				///< creation resolution of the WND file
		ControlBarAnchor anchor;
	};

	typedef std::map< GameWindow *, ControlBarWindowRect > ControlBarWindowMap;

	// allocated on first use and never freed, windows may still be destroyed during static destruction
	ControlBarWindowMap *s_controlBarWindows = nullptr;

	/// screen rect of a control bar window for the current layout, same math as the WND loader
	IRegion2D getScreenRect( const ControlBarWindowRect& r )
	{
		const ControlBarLayout layout = getControlBarLayout( (Real)r.res.x, (Real)r.res.y, r.anchor );
		IRegion2D screen;
		screen.lo.x = (Int)((Real)r.rect.lo.x * layout.scale.x + layout.offset.x);
		screen.lo.y = (Int)((Real)r.rect.lo.y * layout.scale.y + layout.offset.y);
		screen.hi.x = (Int)((Real)r.rect.hi.x * layout.scale.x + layout.offset.x);
		screen.hi.y = (Int)((Real)r.rect.hi.y * layout.scale.y + layout.offset.y);
		return screen;
	}

	void relayoutControlBarWindows()
	{
		if( s_controlBarWindows == nullptr )
			return;

		for( ControlBarWindowMap::const_iterator it = s_controlBarWindows->begin(); it != s_controlBarWindows->end(); ++it )
		{
			GameWindow *window = it->first;
			const IRegion2D screen = getScreenRect( it->second );

			// window positions are relative to the parent, use the parent's new position as well
			ICoord2D parentPos = { 0, 0 };
			if( GameWindow *parent = window->winGetParent() )
			{
				ControlBarWindowMap::const_iterator parentIt = s_controlBarWindows->find( parent );
				if( parentIt != s_controlBarWindows->end() )
					parentPos = getScreenRect( parentIt->second ).lo;
				else
					parent->winGetScreenPosition( &parentPos.x, &parentPos.y );
			}

			window->winSetSize( screen.hi.x - screen.lo.x, screen.hi.y - screen.lo.y );
			window->winSetPosition( screen.lo.x - parentPos.x, screen.lo.y - parentPos.y );
		}
	}
}

//-------------------------------------------------------------------------------------------------
void registerControlBarWindow( GameWindow *window, const IRegion2D& creationRect, const ICoord2D& creationRes, ControlBarAnchor anchor )
{
	if( window == nullptr || anchor == CONTROLBAR_ANCHOR_NONE )
		return;

	if( s_controlBarWindows == nullptr )
		s_controlBarWindows = new ControlBarWindowMap;

	ControlBarWindowRect& r = (*s_controlBarWindows)[ window ];
	r.rect = creationRect;
	r.res = creationRes;
	r.anchor = anchor;
}

//-------------------------------------------------------------------------------------------------
void unregisterControlBarWindow( GameWindow *window )
{
	if( s_controlBarWindows != nullptr )
		s_controlBarWindows->erase( window );
}

//-------------------------------------------------------------------------------------------------
void setControlBarUserScale( Real scale )
{
	scale = clamp( 0.5f, scale, 1.0f );
	const Real change = scale - TheControlBarLayoutSettings.scale;
	if( change < 0.001f && change > -0.001f )
		return;

	TheControlBarLayoutSettings.scale = scale;

	// the stretched layout ignores the size factor, nothing to move
	if( TheDisplay == nullptr || !getControlBarLayout( 800.0f, 600.0f ).uniform )
		return;

	if( TheControlBar )
		TheControlBar->beginControlBarRelayout();

	relayoutControlBarWindows();

	if( TheControlBar )
		TheControlBar->endControlBarRelayout();
}
