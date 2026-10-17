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

// FILE: ControlBarLayout.h ///////////////////////////////////////////////////////////////////////
// Scale and offset that map a control bar layout (authored for a creation resolution such as 800x600)
// onto the current display. The original game stretches the control bar independently in x and y,
// which on ultrawide displays (21:9, 32:9) makes it extremely wide. In uniform mode the control bar
// is scaled by a single factor, centered horizontally and anchored to the bottom of the screen.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Lib/BaseType.h"

enum ControlBarLayoutMode
{
	CONTROLBAR_LAYOUT_STRETCH = 0,	///< original behavior: independent x/y scale
	CONTROLBAR_LAYOUT_AUTO,					///< uniform only when the display is wider than 16:9
	CONTROLBAR_LAYOUT_UNIFORM,			///< always uniform
};

struct ControlBarLayoutSettings
{
	ControlBarLayoutMode mode = CONTROLBAR_LAYOUT_AUTO;
	Real scale = 1.0f;	///< additional size factor for uniform mode, clamped to [0.5, 1.0]
};

extern ControlBarLayoutSettings TheControlBarLayoutSettings;

struct ControlBarLayout
{
	Coord2D scale;
	Coord2D offset;	///< added to every scaled screen position
	Bool uniform;		///< TRUE when the uniform (centered) layout is active
};

/// Layout for a control bar authored for the given creation resolution.
ControlBarLayout getControlBarLayout( Real creationWidth, Real creationHeight );
