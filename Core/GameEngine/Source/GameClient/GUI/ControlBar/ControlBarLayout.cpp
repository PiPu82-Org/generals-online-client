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

#include "GameClient/ControlBarLayout.h"
#include "GameClient/Display.h"

ControlBarLayoutSettings TheControlBarLayoutSettings;

//-------------------------------------------------------------------------------------------------
ControlBarLayout getControlBarLayout( Real creationWidth, Real creationHeight )
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
		layout.offset.x = ( displayWidth - creationWidth * scale ) * 0.5f;
		layout.offset.y = displayHeight - creationHeight * scale;
		layout.uniform = TRUE;
	}

	return layout;
}
