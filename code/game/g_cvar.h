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

#ifdef G_CVAR_TABLE
#define G_CVAR( var, name, defaultVal, flags, trackChange, _description ) \
	{ &var, name, defaultVal, flags, 0, trackChange, qfalse },
#define G_CVAR_EXT( var, name, defaultVal, flags, modificationCount, trackChange, teamShader, _description ) \
	{ &var, name, defaultVal, flags, modificationCount, trackChange, teamShader },
#endif

#ifdef G_CVAR_DECLARATION
#define G_CVAR( var, name, defaultVal, flags, trackChange, _description ) \
	vmCvar_t	var;
#define G_CVAR_EXT( var, name, defaultVal, flags, modificationCount, trackChange, teamShader, _description ) \
	vmCvar_t	var;
#endif

#ifdef G_CVAR_EXTERN
#define G_CVAR( var, name, defaultVal, flags, trackChange, _description ) \
	extern	vmCvar_t	var;
#define G_CVAR_EXT( var, name, defaultVal, flags, modificationCount, trackChange, teamShader, _description ) \
	extern	vmCvar_t	var;
#endif

#define NO_FLAGS 0
// trackChange
#define TRACK qtrue
// Don't trackChange
#define NO_TRACK qfalse

// don't override the cheat state set by the system
G_CVAR( g_cheats, "sv_cheats", "", NO_FLAGS, NO_TRACK, NULL )

G_CVAR( g_restarted, "g_restarted", "0", CVAR_ROM, NO_TRACK, NULL )

// latched vars
G_CVAR( g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_USERINFO | CVAR_LATCH, NO_TRACK, NULL )

// allow this many total, including spectators
G_CVAR( g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, NO_TRACK, NULL )
// allow this many active
G_CVAR( g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, NO_TRACK, NULL )

// change anytime vars
G_CVAR( g_dmflags, "dmflags", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, TRACK, NULL )
G_CVAR( g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, TRACK, NULL )
G_CVAR( g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, TRACK, NULL )
G_CVAR( g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, TRACK, NULL )

G_CVAR( g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, NO_TRACK, NULL )

G_CVAR( g_friendlyFire, "g_friendlyFire", "0", CVAR_ARCHIVE, TRACK, NULL )

G_CVAR( g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE, NO_TRACK, NULL )
G_CVAR( g_teamForceBalance, "g_teamForceBalance", "0", CVAR_ARCHIVE, NO_TRACK, NULL )

G_CVAR( g_warmup, "g_warmup", "20", CVAR_ARCHIVE, TRACK, NULL )
G_CVAR( g_doWarmup, "g_doWarmup", "0", CVAR_ARCHIVE, TRACK, NULL )
G_CVAR( g_logfile, "g_log", "games.log", CVAR_ARCHIVE, NO_TRACK, NULL )
G_CVAR( g_logfileSync, "g_logsync", "0", CVAR_ARCHIVE, NO_TRACK, NULL )

G_CVAR( g_password, "g_password", "", CVAR_USERINFO, NO_TRACK, NULL )

G_CVAR( g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, NO_TRACK, NULL )
G_CVAR( g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, NO_TRACK, NULL )

G_CVAR( g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, NO_TRACK, NULL )

G_CVAR( g_dedicated, "dedicated", "0", NO_FLAGS, NO_TRACK, NULL )

G_CVAR( g_speed, "g_speed", "320", NO_FLAGS, TRACK, NULL )
G_CVAR( g_gravity, "g_gravity", "800", NO_FLAGS, TRACK, NULL )
G_CVAR( g_knockback, "g_knockback", "1000", NO_FLAGS, TRACK, NULL )
G_CVAR( g_quadfactor, "g_quadfactor", "3", NO_FLAGS, TRACK, NULL )
G_CVAR( g_weaponRespawn, "g_weaponrespawn", "5", NO_FLAGS, TRACK, NULL )
G_CVAR( g_weaponTeamRespawn, "g_weaponTeamRespawn", "30", NO_FLAGS, TRACK, NULL )
G_CVAR( g_forcerespawn, "g_forcerespawn", "20", NO_FLAGS, TRACK, NULL )
G_CVAR( g_inactivity, "g_inactivity", "0", NO_FLAGS, TRACK, NULL )
G_CVAR( g_debugMove, "g_debugMove", "0", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_debugDamage, "g_debugDamage", "0", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_debugAlloc, "g_debugAlloc", "0", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_motd, "g_motd", "", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_blood, "com_blood", "1", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_oldGibs, "g_oldGibs", "0", CVAR_ARCHIVE, NO_TRACK, NULL )
G_CVAR( g_gibsMissileDirectionKnockbackWeight, "g_gibsMissileDirectionKnockbackWeight", "0.5", CVAR_ARCHIVE, NO_TRACK,
	"When gibbing someone, how much the movement direction of a missile "
	"affects the knockback direction, "
	"as opposed to the direction from the center of the explosion.\n"
	"This makes sure that if the missile hits the player's feet, "
	"the gibs will fly up, so that there is not a big difference "
	"between hitting the player's feet and the ground not far "
	"from the player's feet.\n"
	"\n"
	"Setting to 1 restores the old (vanilla) behavior.\n"
	"\n"
	"Note that this affects not just the gibs "
	"but also the camera velocity of the gibbed player." )
G_CVAR( g_gibsNewEvGibPlayerParmProtocol, "g_gibsNewEvGibPlayerParmProtocol", "1", CVAR_SYSTEMINFO | CVAR_ARCHIVE, NO_TRACK, NULL )
G_CVAR( g_gibsOnCollisionMinSpeed, "g_gibsOnCollisionMinSpeed", "1000", CVAR_ARCHIVE, NO_TRACK,
	"If a player's speed abruptly changes by this much or more, "
	"as a result of collision with something (e.g. floor, wall), "
	"deal `g_gibsOnCollisionBaseDamage` or more damage to them "
	"(damage increases with the square of the speed)" )
G_CVAR( g_gibsOnCollisionBaseDamage, "g_gibsOnCollisionBaseDamage", "150", CVAR_ARCHIVE, NO_TRACK,
	"See `g_gibsOnCollisionMinSpeed`. "
	"Higher damage normally results in gibs getting spread more broadly." )
G_CVAR( g_gibsOnCollisionAffectLivePlayers, "g_gibsOnCollisionAffectLivePlayers", "0.0", NO_FLAGS, TRACK,
	"This one affects gameplay significantly, and is basically just for fun.\n"
	"Try setting this to 1 and jumping off the railgun platform "
	"to the lowest level on q3dm19." )
G_CVAR( g_gibsOnCollisionInheritPlayerVelocity, "g_gibsOnCollisionInheritPlayerVelocity", "1.0", CVAR_ARCHIVE, NO_TRACK,
	"Scale the speed of gibs "
	"when gibbing as a result of `g_gibsOnCollision*`.\n"
	"When set to 0, the gibs will have the velocity that the player has "
	"immediately *after* the collision (i.e. basically 0), "
	"and not *before* the collision. "
	"Also see `cg_gibsInheritPlayerVelocity`." )

G_CVAR( g_podiumDist, "g_podiumDist", "80", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_podiumDrop, "g_podiumDrop", "70", NO_FLAGS, NO_TRACK, NULL )

G_CVAR( g_allowVote, "g_allowVote", "1", CVAR_ARCHIVE, NO_TRACK, NULL )
G_CVAR( g_listEntity, "g_listEntity", "0", NO_FLAGS, NO_TRACK, NULL )

#ifdef MISSIONPACK
G_CVAR( g_obeliskHealth, "g_obeliskHealth", "2500", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_obeliskRegenPeriod, "g_obeliskRegenPeriod", "1", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_obeliskRegenAmount, "g_obeliskRegenAmount", "15", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_obeliskRespawnDelay, "g_obeliskRespawnDelay", "10", CVAR_SERVERINFO, NO_TRACK, NULL )

G_CVAR( g_cubeTimeout, "g_cubeTimeout", "30", NO_FLAGS, NO_TRACK, NULL )
G_CVAR_EXT( g_redteam, "g_redteam", "Stroggs", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, TRACK, qtrue, NULL )
G_CVAR_EXT( g_blueteam, "g_blueteam", "Pagans", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, TRACK, qtrue, NULL )
G_CVAR( g_singlePlayer, "ui_singlePlayerActive", "", NO_FLAGS, NO_TRACK, NULL )

G_CVAR( g_enableDust, "g_enableDust", "0", CVAR_SERVERINFO, TRACK, NULL )
G_CVAR( g_enableBreath, "g_enableBreath", "0", CVAR_SERVERINFO, TRACK, NULL )
G_CVAR( g_proxMineTimeout, "g_proxMineTimeout", "20000", NO_FLAGS, NO_TRACK, NULL )
#endif
G_CVAR( g_smoothClients, "g_smoothClients", "1", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_autoAttack, "g_autoAttack", "0", CVAR_SYSTEMINFO, NO_TRACK, NULL )
G_CVAR( pmove_fixed, "pmove_fixed", "0", CVAR_SYSTEMINFO, NO_TRACK, NULL )
G_CVAR( pmove_msec, "pmove_msec", "8", CVAR_SYSTEMINFO, NO_TRACK, NULL )

//unlagged - server options
// some new server-side variables
G_CVAR( g_delagHitscan, "g_delagHitscan", "1", CVAR_ARCHIVE | CVAR_SERVERINFO, TRACK, NULL )
G_CVAR( g_unlaggedVersion, "g_unlaggedVersion", "2.0", CVAR_ROM | CVAR_SERVERINFO, NO_TRACK, NULL )
G_CVAR( g_truePing, "g_truePing", "1", CVAR_ARCHIVE, TRACK, NULL )
// server admins can adjust this if they *believe* the lightning
// gun is too powerful with lag compensation
G_CVAR( g_lightningDamage, "g_lightningDamage", "8", NO_FLAGS, TRACK, NULL )
// it's CVAR_SYSTEMINFO so the client's sv_fps will be automagically set to its value.
// this is for convenience - using "sv_fps.integer" is nice :)
G_CVAR( sv_fps, "sv_fps", "20", CVAR_SYSTEMINFO | CVAR_ARCHIVE, NO_TRACK, NULL )
//unlagged - server options

G_CVAR( g_rankings, "g_rankings", "0", NO_FLAGS, NO_TRACK, NULL )
G_CVAR( g_localTeamPref, "g_localTeamPref", "", NO_FLAGS, NO_TRACK, NULL )

#undef G_CVAR
#undef G_CVAR_EXT
#undef NO_FLAGS
#undef TRACK
#undef NO_TRACK
