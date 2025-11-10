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
CG_CVAR( cg_gibs, "cg_gibs", "1", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_oldGibs, "cg_oldGibs", "0", CVAR_ARCHIVE,
	"Vanilla gibs, from 1999. "
	"When set to 1, takes priority "
	"over (almost) all the other `cg_gibs*` vars." )
CG_CVAR( cg_gibsInheritPlayerVelocity, "cg_gibsInheritPlayerVelocity", "1.0", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_gibsExtraRandomVelocity, "cg_gibsExtraRandomVelocity", "250", CVAR_ARCHIVE, NULL )
CG_CVAR( cg_gibsExtraVerticalVelocity, "cg_gibsExtraVerticalVelocity", "100", CVAR_ARCHIVE, NULL )
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
CG_CVAR( cg_bounceMarksMinImpactSpeed, "cg_bounceMarksMinImpactSpeed", "350", CVAR_ARCHIVE,
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
