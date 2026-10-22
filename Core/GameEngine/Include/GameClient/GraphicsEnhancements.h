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

// FILE: GraphicsEnhancements.h ///////////////////////////////////////////////////////////////////
// Optional visual improvements of the friends edition. Every one of them can be switched off in
// settings.json ("graphics" section). They only change what is drawn on the local machine,
// never the game logic, so players with different settings can play together.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Lib/BaseType.h"

struct GraphicsEnhancementSettings
{
	Bool terrainHD = TRUE;					///< use Art/TerrainHD when present (only in the terrain HD build)
	Bool smoothShroud = TRUE;				///< draw the shroud with soft, round edges instead of one texel per cell
	Int maxParticles = 10000;				///< raise the particle limit to at least this value, 0 keeps the game's value
	Int tankTrackFadeDelayMs = 300000;	///< tank tracks stay visible at least this long, 0 keeps the game's value
};

extern GraphicsEnhancementSettings TheGraphicsEnhancementSettings;
