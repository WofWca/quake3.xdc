/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2023 LegendaryGuard
Copyright (C) 2026 WofWca

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#ifdef CG_CVAR_TABLE
#define CG_CVAR( var, name, defaultVal, flags, _description ) \
	{ &var, name, defaultVal, flags },
#endif

#ifdef CG_CVAR_DECLARATION
#define CG_CVAR( var, name, defaultVal, flags, _description ) \
	vmCvar_t	var;
#endif

#ifdef CG_CVAR_EXTERN
#define CG_CVAR( var, name, defaultVal, flags, _description ) \
	extern	vmCvar_t	var;
#endif

CG_CVAR( cg_ignore, "cg_ignore", "0", 0, NULL )	// used for debugging
CG_CVAR( cg_autoswitch, "cg_autoswitch", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawGun, "cg_drawGun", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_zoomFov, "cg_zoomfov", "22.5", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_fov, "cg_fov", "90", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_viewsize, "cg_viewsize", "100", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_shadows, "cg_shadows", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_gibs, "cg_gibs", "1.0", CVAR_ARCHIVE,
	"Make a bloody explosion when a player takes a lot of damage.\n"
	"The bigger the value the more pieces." )
CG_CVAR( cg_oldGibs, "cg_oldGibs", "0", CVAR_ARCHIVE,
	"Vanilla gibs, from 1999. "
	"When set to 1, takes priority "
	"over (almost) all the other `cg_gibs*` vars." )
CG_CVAR( cg_gibsInheritPlayerVelocity, "cg_gibsInheritPlayerVelocity", "1.0", CVAR_ARCHIVE,
	"If the player is moving fast, their gibs will also move fast. "
	"And the player will move fast when they receive damage (knockback). "
	"The most important var of the Better Gibs mod.\n"
	"\n"
	"It's possible to set a value other than 1 or 0, "
	"but if you simply want the gibs to have more speed, "
	"you're probably after `cg_gibsKnockback` or other `cg_gibs` vars." )
CG_CVAR( cg_gibsKnockback, "cg_gibsKnockback", "1.0", 0,
	"Scale the knockback when gibbing a player, "
	"as if they received more (or less) damage.\n"
	"On vanilla servers this only has effect on random and vertical velocity "
	"but not on directional velocity.\n"
	"Remember that this also affects own gibs, so having a value too high "
	"will make gibs fly faster than the camera." )
CG_CVAR( cg_gibsExtraKnockback, "cg_gibsExtraKnockback", "100", 0,
	"Like `cg_gibsKnockback`, but adds a *fixed* amount of knockback. "
	"100 of knockback corresponds to 20 of damage.\n"
	"This basically sets a \"baseline\" amount of knockback, "
	"making all shots feel powerful, "
	"while not scaling the already high speed the gibs would gain "
	"from Quad shots." )
CG_CVAR( cg_gibsPiecesFromKnockback, "cg_gibsPiecesFromKnockback", "1.1", CVAR_ARCHIVE,
	"For every 100 of knockback speed above 500 (100 damage), "
	"add this many pieces of gibs, times the value of `cg_gibs`.\n"
	"This makes for more pieces when someone gets gibbed by Quad.\n"
	"\n"
	"With higher damage gibs fly further apart, so one starts to see gibs "
	"as individual pieces instead of a \"cloud\", which IMO is not good. "
	"Increasing the amount of gibs with damage fixes this. "
	"And overall it makes it more rewarding to deal more damage.\n"
	"Q: \"What sense does it make to have more gibs? "
	"Did the player got bigger from taking more damage??\":\n"
	"A: No, they simlpy got split into more pieces." )
CG_CVAR( cg_gibsLinearVelocityFromKnockback, "cg_gibsLinearVelocityFromKnockback", "1", 0,
	"Scale how fast the gibs move away from damage (knockback).\n"
	"This sounds similar to `cg_gibsInheritPlayerVelocity`, "
	"but this var doesn't scale the velocity that the player already had "
	"when they received the damage." )
CG_CVAR( cg_gibsVelocityFromExplosionFraction, "cg_gibsVelocityFromExplosionFraction", "0.5", 0,
	"When getting gibbed as a result of an explosion, "
	"calculate knockback velocity for each gib piece separately, "
	"pointing away from the explosion, "
	"as opposed to the knockback direction being the same for all gibs, "
	"like with non-explosion weapons. "
	"As a result, the closer the explosion the bigger the spread." )
CG_CVAR( cg_gibsExplosionDistanceOffset, "cg_gibsExplosionDistanceOffset", "-3.0", 0,
	"When getting gibbed as a result of an explosion, "
	"pretend that the explosion happened this many units farther "
	"(when positive, or closer, when negative) from the center of the body.\n"
	"Has no effect when `cg_gibsVelocityFromExplosionFraction 0`.\n"
	"For comparison, player height and width are 56 and 30 units.\n"
	"This lets you increase the spread. "
	"This doesn't affect the damage and knockback.\n" )
CG_CVAR( cg_gibsRandomVelocityFromKnockback, "cg_gibsRandomVelocityFromKnockback", "0.5", CVAR_ARCHIVE,
	"Scale how much random velocity (spread) the gibs gain "
	"from damage (knockback)." )
CG_CVAR( cg_gibsVerticalVelocityFromKnockback, "cg_gibsVerticalVelocityFromKnockback", "0.2", CVAR_ARCHIVE,
	"Scale how much vertical velocity the gibs gain from damage (knockback)." )
CG_CVAR( cg_gibsExtraRandomVelocity, "cg_gibsExtraRandomVelocity", "100", CVAR_ARCHIVE,
	"Like `cg_gibsRandomVelocityFromKnockback`, "
	"but add a *fixed* amount of random velocity to each piece.\n"
	"Ensures that even if the player was gibbed with the machine gun, "
	"the gibs would still fly apart somewhat." )
CG_CVAR( cg_gibsExtraVerticalVelocity, "cg_gibsExtraVerticalVelocity", "50", CVAR_ARCHIVE,
	"Like `cg_gibsVerticalVelocityFromKnockback`, "
	"but adds a *fixed* amount of vertical speed to each piece" )
CG_CVAR( cg_gibsPlayerSpeedFromKnockback, "cg_gibsPlayerSpeedFromKnockback", "-0.25", 0,
	"How much to slow down the player "
	"depending on how much damage (knockback) they took.\n"
	"This makes the original player velocity have less effect "
	"on the final gib velocity, thus giving more emphasis to the damage direction.\n"
	"\n"
	"Unlike reducing `cg_gibsInheritPlayerVelocity`, "
	"this works better if the player is already moving at high speed "
	"(e.g. on a jump pad), or falling. "
	"This will not reduce player velocity if the knockback was low "
	"or if damage had no direction (e.g. falling).\n"
	"\n"
	"Has no effect on vanilla servers." )
CG_CVAR( cg_gibsPlayerSpeedFromKnockbackMaxFraction, "cg_gibsPlayerSpeedFromKnockbackMaxFraction", "-0.5", 0,
	"See `cg_gibsPlayerSpeedFromKnockback`. "
	"If knockback is big, this helps keep at least some fraction "
	"of player speed. "
	"For example, if this is -0.75, we'll always keep at least 25% "
	"of original player velocity." )
CG_CVAR( cg_gibsDuration, "cg_gibsDuration", "5000", 0,
	"How long (in milliseconds) gibs stay around "
	"before sinking into the ground." )
CG_CVAR( cg_gibsOriginalOrigin, "cg_gibsOriginalOrigin", "0.0", 0,
	"Whether to use the exact position where the player got gibbed "
	"as the initial gibs position, instead of the position "
	"of the gibbed player on the screen, i.e. the position of the player "
	"on the next server frame (lerpOrigin).\n"
	"\n"
	"Can be a fractional value between 0 and 1, "
	"but you probably want either 0 or 1.\n"
	"To get vanilla behavior, set to 0.\n"
	"\n"
	"Has no effect on vanilla servers (`g_gibsNewEvGibPlayerProtocol 0`).\n"
	"Basically has no effect when `cg_gibsNoLerpDelay 1`.\n"
	"\n"
	"Using the original position might make it seem\n"
	"like the player \"telepoted\" one snapshot back (50ms at `snaps 20`)\n"
	"if they're already moving at high speed." )
CG_CVAR( cg_gibsNoLerpDelay, "cg_gibsNoLerpDelay", "1", 0,
	"Don't wait for next snapshot to arrive (normally 50ms) "
	"before gibbing players.\n"
	"Otherwise after taking a shot the player's body will stay intact "
	"for the said 50ms, then get gibbed.\n"
	"Consequently, this affects the position from which the gibs are launched, "
	"due to the gibbed player (or, rather, their camera) "
	"moving during those 50ms. See `cg_gibsOriginalOrigin`.\n"
	"\n"
	"Note that when looking at another player (or missile) gibbing someone "
	"it will look like the player gets gibbed "
	"50ms before the actual shot is fired.\n"
	"That is, the gib event will happen "
	"ahead of the displayed state of the game.\n"
	"\n"
	"Set this to 3 (2nd bit) to also do this on vanilla servers.\n"
	"Set this to 5 (3rd bit) to keep the body visible "
	"until the next snapshot. That is, both the body and the gibs "
	"will be visible together for ~50ms.\n"
	"Set this to 9 (4th bit) to play some other events, "
	"such as explosions, railgun trails, bullet hits, ahead of time.\n"
	"Set bits 5 and up to play even more events ahead of time.\n"
	"To get vanilla behavior, set to 0." )
CG_CVAR( cg_gibsStopPlayerSounds, "cg_gibsStopPlayerSounds", "3", 0,
	"Stop player's sounds (pain, taunts) when they get gibbed.\n"
	"Doesn't work for self as of writing. "
	"Set 1st bit to apply this to other players. "
	"Set 2nd bit to still try to apply this to self." )
CG_CVAR( cg_gibsMaxSimultaneousBounceSounds, "cg_gibsMaxSimultaneousBounceSounds", "3", 0,
	"Limit the amount of \"splat\" sounds per a few milliseconds" )
CG_CVAR( cg_gibsBounceFactor, "cg_gibsBounceFactor", "0.4", CVAR_ARCHIVE,
	"Fraction of speed that gibs preserve when bouncing off a surface" )
CG_CVAR( cg_gibsBounceFactorRandomness, "cg_gibsBounceFactorRandomness", "0.5", CVAR_ARCHIVE,
	"Fraction of `cg_gibsBounceFactor` that is random. "
	"This makes gibs look less uniform, and spread a little more." )
CG_CVAR( cg_gibsRotationFactor, "cg_gibsRotationFactor", "1.0", CVAR_ARCHIVE,
	"Scale how fast gib pieces will rotate, depending on damage (knockback)." )
// Not archived because experimental
CG_CVAR( cg_gibsBloodTrailPeriod, "cg_gibsBloodTrailPeriod", "150", 0,
	"How often in milliseconds gibs leave blood \"clouds\"" )
CG_CVAR( cg_gibsFireTrailPeriod, "cg_gibsFireTrailPeriod", "30", 0,
	"How often in milliseconds gibs leave fire \"clouds\".\n"
	"Remember to set `cg_gibsFireTrailMinKnockback` >= 0 for this to work" )
CG_CVAR( cg_gibsFireTrailDuration, "cg_gibsFireTrailDuration", "1024", 0,
	"How long in milliseconds gibs leave fire trails for.\n"
	"Remember to set `cg_gibsFireTrailMinKnockback` >= 0 for this to work" )
CG_CVAR( cg_gibsFireTrailMinKnockback, "cg_gibsFireTrailMinKnockback", "-1", 0,
	"How much knockback the player must receive "
	"for their gibs to leave fire trails.\n"
	"100 damage is 500 knockback.\n"
	"`-1` to disable." )
CG_CVAR( cg_gibsBetterCameraOnGib, "cg_gibsBetterCameraOnGib", "1", CVAR_USERINFO | CVAR_ARCHIVE,
	"When you get gibbed, makes your camera get knocked farther away "
	"thanks to it being smaller than your player's bounding box, "
	"and thus not having friction with the ground until it falls down.\n"
	"Mostly noticeable when getting gibbed with a shotgun or a railgun.\n"
	"Doesn't work on vanilla servers." )
CG_CVAR( cg_draw2D, "cg_draw2D", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawStatus, "cg_drawStatus", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawTimer, "cg_drawTimer", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawFPS, "cg_drawFPS", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawSnapshot, "cg_drawSnapshot", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_draw3dIcons, "cg_draw3dIcons", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawIcons, "cg_drawIcons", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawAmmoWarning, "cg_drawAmmoWarning", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawAttacker, "cg_drawAttacker", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawCrosshair, "cg_drawCrosshair", "4", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawCrosshairNames, "cg_drawCrosshairNames", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawRewards, "cg_drawRewards", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawScores, "cg_drawScores", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawPickups, "cg_drawPickups", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawWeaponBar, "cg_drawWeaponBar", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_drawStatusHead, "cg_drawStatusHead", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_statusScale, "cg_statusScale", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_fovAspectAdjust, "cg_fovAspectAdjust", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_fovGunAdjust, "cg_fovGunAdjust", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_stretch, "cg_stretch", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_crosshairSize, "cg_crosshairSize", "24", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_crosshairHealth, "cg_crosshairHealth", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_crosshairX, "cg_crosshairX", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_crosshairY, "cg_crosshairY", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_brassTime, "cg_brassTime", "2500", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_simpleItems, "cg_simpleItems", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_addMarks, "cg_marks", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_bounceMarksMinImpactSpeed, "cg_bounceMarksMinImpactSpeed", "200", CVAR_ARCHIVE,
	"Note that ~290 corresponds to a free fall with no bounce from player height." )
CG_CVAR( cg_bounceSoundMinImpactSpeed, "cg_bounceSoundMinImpactSpeed", "450", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_lagometer, "cg_lagometer", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_railTrailTime, "cg_railTrailTime", "400", CVAR_ARCHIVE, NULL )
// CG_CVAR( cg_gun_frame, "cg_gun_frame", "", CVAR_CHEAT, NULL )
CG_CVAR( cg_gun_x, "cg_gunX", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_gun_y, "cg_gunY", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_gun_z, "cg_gunZ", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_centertime, "cg_centertime", "3", CVAR_CHEAT, NULL )
CG_CVAR( cg_runpitch, "cg_runpitch", "0.002", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_runroll, "cg_runroll", "0.005", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_bobup, "cg_bobup", "0.005", CVAR_CHEAT, NULL )
CG_CVAR( cg_bobpitch, "cg_bobpitch", "0.002", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_bobroll, "cg_bobroll", "0.002", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_swingSpeed, "cg_swingSpeed", "0.3", CVAR_CHEAT, NULL )
CG_CVAR( cg_animSpeed, "cg_animspeed", "1", CVAR_CHEAT, NULL )
CG_CVAR( cg_debugAnim, "cg_debuganim", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_debugPosition, "cg_debugposition", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_debugEvents, "cg_debugevents", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_debugGibs, "cg_debugGibs", "0", CVAR_CHEAT,
	"Flag-like variable: set to 1 for basic debug logging, "
	"set to 1023 (or -1) for mega debugging" )
CG_CVAR( cg_errorDecay, "cg_errordecay", "100", 0, NULL )
CG_CVAR( cg_nopredict, "cg_nopredict", "0", 0, NULL )
CG_CVAR( cg_noPlayerAnims, "cg_noplayeranims", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_showmiss, "cg_showmiss", "0", 0, NULL )
CG_CVAR( cg_footsteps, "cg_footsteps", "1", CVAR_CHEAT, NULL )
CG_CVAR( cg_tracerChance, "cg_tracerchance", "0.4", CVAR_CHEAT, NULL )
CG_CVAR( cg_tracerWidth, "cg_tracerwidth", "1", CVAR_CHEAT, NULL )
CG_CVAR( cg_tracerLength, "cg_tracerlength", "100", CVAR_CHEAT, NULL )
CG_CVAR( cg_thirdPersonRange, "cg_thirdPersonRange", "40", CVAR_CHEAT, NULL )
CG_CVAR( cg_thirdPersonAngle, "cg_thirdPersonAngle", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_thirdPerson, "cg_thirdPerson", "0", 0, NULL )
CG_CVAR( cg_teamChatTime, "cg_teamChatTime", "3000", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_teamChatHeight, "cg_teamChatHeight", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_forceModel, "cg_forceModel", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_predictItems, "cg_predictItems", "1", CVAR_ARCHIVE, NULL )
#ifdef MISSIONPACK
CG_CVAR( cg_deferPlayers, "cg_deferPlayers", "0", CVAR_ARCHIVE, NULL )
#else
CG_CVAR( cg_deferPlayers, "cg_deferPlayers", "1", CVAR_ARCHIVE, NULL )
#endif
CG_CVAR( cg_drawTeamOverlay, "cg_drawTeamOverlay", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_teamOverlayUserinfo, "teamoverlay", "0", CVAR_ROM | CVAR_USERINFO, NULL )
CG_CVAR( cg_stats, "cg_stats", "0", 0, NULL )
CG_CVAR( cg_drawFriend, "cg_drawFriend", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_teamChatsOnly, "cg_teamChatsOnly", "0", CVAR_ARCHIVE, NULL )
#ifdef MISSIONPACK
CG_CVAR( cg_noVoiceChats, "cg_noVoiceChats", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_noVoiceText, "cg_noVoiceText", "0", CVAR_ARCHIVE, NULL )
#endif
// the following variables are created in other parts of the system,
// but we also reference them here
CG_CVAR( cg_buildScript, "com_buildScript", "0", 0, NULL )	// force loading of all possible data amd error on failures
CG_CVAR( cg_paused, "cl_paused", "0", CVAR_ROM, NULL )
CG_CVAR( cg_blood, "com_blood", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, NULL )
#ifdef MISSIONPACK
CG_CVAR( cg_redTeamName, "g_redteam", DEFAULT_REDTEAM_NAME, CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO, NULL )
CG_CVAR( cg_blueTeamName, "g_blueteam", DEFAULT_BLUETEAM_NAME, CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO, NULL )
CG_CVAR( cg_currentSelectedPlayer, "cg_currentSelectedPlayer", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_currentSelectedPlayerName, "cg_currentSelectedPlayerName", "", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_singlePlayer, "ui_singlePlayerActive", "0", CVAR_USERINFO, NULL )
CG_CVAR( cg_enableDust, "g_enableDust", "0", CVAR_SERVERINFO, NULL )
CG_CVAR( cg_enableBreath, "g_enableBreath", "0", CVAR_SERVERINFO, NULL )
CG_CVAR( cg_singlePlayerActive, "ui_singlePlayerActive", "0", CVAR_USERINFO, NULL )
CG_CVAR( cg_recordSPDemo, "ui_recordSPDemo", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_recordSPDemoName, "ui_recordSPDemoName", "", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_obeliskRespawnDelay, "g_obeliskRespawnDelay", "10", CVAR_SERVERINFO, NULL )
CG_CVAR( cg_hudFiles, "cg_hudFiles", "ui/hud.txt", CVAR_ARCHIVE, NULL )
#endif
CG_CVAR( cg_cameraOrbit, "cg_cameraOrbit", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_cameraOrbitDelay, "cg_cameraOrbitDelay", "50", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_timescaleFadeEnd, "cg_timescaleFadeEnd", "1", 0, NULL )
CG_CVAR( cg_timescaleFadeSpeed, "cg_timescaleFadeSpeed", "0", 0, NULL )
CG_CVAR( cg_timescale, "timescale", "1", 0, NULL )
CG_CVAR( cg_scorePlum, "cg_scorePlums", "1", CVAR_USERINFO | CVAR_ARCHIVE, NULL )
//unlagged - smooth clients #2
// this is done server-side now
// CG_CVAR( cg_smoothClients, "cg_smoothClients", "0", CVAR_USERINFO | CVAR_ARCHIVE, NULL )
//unlagged - smooth clients #2
// TODO: actually detect touchscreen controls and enable this only if they are being used currently, instead of with a cvar
CG_CVAR( cg_autoAttack, "cg_autoAttack", "0", CVAR_USERINFO, NULL )
CG_CVAR( cg_cameraMode, "com_cameraMode", "0", CVAR_CHEAT, NULL )

CG_CVAR( pmove_fixed, "pmove_fixed", "0", CVAR_SYSTEMINFO, NULL )
CG_CVAR( pmove_msec, "pmove_msec", "8", CVAR_SYSTEMINFO, NULL )
#ifdef MISSIONPACK
CG_CVAR( cg_smallFont, "ui_smallFont", "0.25", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_bigFont, "ui_bigFont", "0.4", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_noTaunt, "cg_noTaunt", "0", CVAR_ARCHIVE, NULL )
#endif
CG_CVAR( cg_noProjectileTrail, "cg_noProjectileTrail", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_oldRail, "cg_oldRail", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_oldRocket, "cg_oldRocket", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_oldPlasma, "cg_oldPlasma", "1", CVAR_ARCHIVE, NULL )
//unlagged - client options
CG_CVAR( cg_delag, "cg_delag", "1", CVAR_ARCHIVE | CVAR_USERINFO, NULL )
CG_CVAR( cg_debugDelag, "cg_debugDelag", "0", CVAR_USERINFO | CVAR_CHEAT, NULL )
CG_CVAR( cg_drawBBox, "cg_drawBBox", "0", CVAR_CHEAT, NULL )
CG_CVAR( cg_cmdTimeNudge, "cg_cmdTimeNudge", "0", CVAR_ARCHIVE | CVAR_USERINFO, NULL )
// this will be copied from the server
// CG_CVAR( sv_fps, "sv_fps", "20", CVAR_SYSTEMINFO, NULL )
CG_CVAR( cg_projectileNudge, "cg_projectileNudge", "0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_optimizePrediction, "cg_optimizePrediction", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_latentSnaps, "cg_latentSnaps", "0", CVAR_USERINFO | CVAR_CHEAT, NULL )
CG_CVAR( cg_latentCmds, "cg_latentCmds", "0", CVAR_USERINFO | CVAR_CHEAT, NULL )
CG_CVAR( cg_plOut, "cg_plOut", "0", CVAR_USERINFO | CVAR_CHEAT, NULL )
//unlagged - client options
CG_CVAR( cg_trueLightning, "cg_trueLightning", "0.0", CVAR_ARCHIVE, NULL )
// CG_CVAR( cg_pmove_fixed, "cg_pmove_fixed", "0", CVAR_USERINFO | CVAR_ARCHIVE, NULL )

#undef CG_CVAR
