/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

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
//
// cg_effects.c -- these functions generate localentities, usually as a result
// of event processing

#include "cg_local.h"


/*
==================
CG_BubbleTrail

Bullets shot underwater
==================
*/
void CG_BubbleTrail( vec3_t start, vec3_t end, float spacing ) {
	vec3_t		move;
	vec3_t		vec;
	float		len;
	int			i;

	if ( cg_noProjectileTrail.integer ) {
		return;
	}

	VectorCopy (start, move);
	VectorSubtract (end, start, vec);
	len = VectorNormalize (vec);

	// advance a random amount first
	i = rand() % (int)spacing;
	VectorMA( move, i, vec, move );

	VectorScale (vec, spacing, vec);

	for ( ; i < len; i += spacing ) {
		localEntity_t	*le;
		refEntity_t		*re;

		le = CG_AllocLocalEntity();
		le->leFlags = LEF_PUFF_DONT_SCALE;
		le->leType = LE_MOVE_SCALE_FADE;
		le->startTime = cg.time;
		le->endTime = cg.time + 1000 + random() * 250;
		le->lifeRate = 1.0 / ( le->endTime - le->startTime );

		re = &le->refEntity;
		re->shaderTime = cg.time / 1000.0f;

		re->reType = RT_SPRITE;
		re->rotation = 0;
		re->radius = 3;
		re->customShader = cgs.media.waterBubbleShader;
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		re->shaderRGBA[3] = 0xff;

		le->color[3] = 1.0;

		le->pos.trType = TR_LINEAR;
		le->pos.trTime = cg.time;
		VectorCopy( move, le->pos.trBase );
		le->pos.trDelta[0] = crandom()*5;
		le->pos.trDelta[1] = crandom()*5;
		le->pos.trDelta[2] = crandom()*5 + 6;

		VectorAdd (move, vec, move);
	}
}

/*
=====================
CG_SmokePuff

Adds a smoke puff or blood trail localEntity.
=====================
*/
localEntity_t *CG_SmokePuff( const vec3_t p, const vec3_t vel, 
				   float radius,
				   float r, float g, float b, float a,
				   float duration,
				   int startTime,
				   int fadeInTime,
				   int leFlags,
				   qhandle_t hShader ) {
	static int	seed = 0x92;
	localEntity_t	*le;
	refEntity_t		*re;
//	int fadeInTime = startTime + duration / 2;

	le = CG_AllocLocalEntity();
	le->leFlags = leFlags;
	le->radius = radius;

	re = &le->refEntity;
	re->rotation = Q_random( &seed ) * 360;
	re->radius = radius;
	re->shaderTime = startTime / 1000.0f;

	le->leType = LE_MOVE_SCALE_FADE;
	le->startTime = startTime;
	le->fadeInTime = fadeInTime;
	le->endTime = startTime + duration;
	if ( fadeInTime > startTime ) {
		le->lifeRate = 1.0 / ( le->endTime - le->fadeInTime );
	}
	else {
		le->lifeRate = 1.0 / ( le->endTime - le->startTime );
	}
	le->color[0] = r;
	le->color[1] = g; 
	le->color[2] = b;
	le->color[3] = a;


	le->pos.trType = TR_LINEAR;
	le->pos.trTime = startTime;
	VectorCopy( vel, le->pos.trDelta );
	VectorCopy( p, le->pos.trBase );

	VectorCopy( p, re->origin );
	re->customShader = hShader;

	// rage pro can't alpha fade, so use a different shader
	if ( cgs.glconfig.hardwareType == GLHW_RAGEPRO ) {
		re->customShader = cgs.media.smokePuffRageProShader;
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		re->shaderRGBA[3] = 0xff;
	} else {
		re->shaderRGBA[0] = le->color[0] * 0xff;
		re->shaderRGBA[1] = le->color[1] * 0xff;
		re->shaderRGBA[2] = le->color[2] * 0xff;
		re->shaderRGBA[3] = 0xff;
	}

	re->reType = RT_SPRITE;
	re->radius = le->radius;

	return le;
}

/*
==================
CG_SpawnEffect

Player teleporting in or out
==================
*/
void CG_SpawnEffect( vec3_t org ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_FADE_RGB;
	le->startTime = cg.time;
	le->endTime = cg.time + 500;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;

#ifndef MISSIONPACK
	re->customShader = cgs.media.teleportEffectShader;
#endif
	re->hModel = cgs.media.teleportEffectModel;
	AxisClear( re->axis );

	VectorCopy( org, re->origin );
#ifdef MISSIONPACK
	re->origin[2] += 16;
#else
	re->origin[2] -= 24;
#endif
}


#ifdef MISSIONPACK
/*
===============
CG_LightningBoltBeam
===============
*/
void CG_LightningBoltBeam( vec3_t start, vec3_t end ) {
	localEntity_t	*le;
	refEntity_t		*beam;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_SHOWREFENTITY;
	le->startTime = cg.time;
	le->endTime = cg.time + 50;

	beam = &le->refEntity;

	VectorCopy( start, beam->origin );
	// this is the end point
	VectorCopy( end, beam->oldorigin );

	beam->reType = RT_LIGHTNING;
	beam->customShader = cgs.media.lightningShader;
}

/*
==================
CG_KamikazeEffect
==================
*/
void CG_KamikazeEffect( vec3_t org ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_KAMIKAZE;
	le->startTime = cg.time;
	le->endTime = cg.time + 3000;//2250;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	VectorClear(le->angles.trBase);

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;

	re->hModel = cgs.media.kamikazeEffectModel;

	VectorCopy( org, re->origin );

}

/*
==================
CG_ObeliskExplode
==================
*/
void CG_ObeliskExplode( vec3_t org, int entityNum ) {
	localEntity_t	*le;
	vec3_t origin;

	// create an explosion
	VectorCopy( org, origin );
	origin[2] += 64;
	le = CG_MakeExplosion( origin, vec3_origin,
						   cgs.media.dishFlashModel,
						   cgs.media.rocketExplosionShader,
						   600, qtrue );
	le->light = 300;
	le->lightColor[0] = 1;
	le->lightColor[1] = 0.75;
	le->lightColor[2] = 0.0;
}

/*
==================
CG_ObeliskPain
==================
*/
void CG_ObeliskPain( vec3_t org ) {
	float r;
	sfxHandle_t sfx;

	// hit sound
	r = rand() & 3;
	if ( r < 2 ) {
		sfx = cgs.media.obeliskHitSound1;
	} else if ( r == 2 ) {
		sfx = cgs.media.obeliskHitSound2;
	} else {
		sfx = cgs.media.obeliskHitSound3;
	}
	trap_S_StartSound ( org, ENTITYNUM_NONE, CHAN_BODY, sfx );
}


/*
==================
CG_InvulnerabilityImpact
==================
*/
void CG_InvulnerabilityImpact( vec3_t org, vec3_t angles ) {
	localEntity_t	*le;
	refEntity_t		*re;
	int				r;
	sfxHandle_t		sfx;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_INVULIMPACT;
	le->startTime = cg.time;
	le->endTime = cg.time + 1000;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;

	re->hModel = cgs.media.invulnerabilityImpactModel;

	VectorCopy( org, re->origin );
	AnglesToAxis( angles, re->axis );

	r = rand() & 3;
	if ( r < 2 ) {
		sfx = cgs.media.invulnerabilityImpactSound1;
	} else if ( r == 2 ) {
		sfx = cgs.media.invulnerabilityImpactSound2;
	} else {
		sfx = cgs.media.invulnerabilityImpactSound3;
	}
	trap_S_StartSound (org, ENTITYNUM_NONE, CHAN_BODY, sfx );
}

/*
==================
CG_InvulnerabilityJuiced
==================
*/
void CG_InvulnerabilityJuiced( vec3_t org ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			angles;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_INVULJUICED;
	le->startTime = cg.time;
	le->endTime = cg.time + 10000;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;

	re->hModel = cgs.media.invulnerabilityJuicedModel;

	VectorCopy( org, re->origin );
	VectorClear(angles);
	AnglesToAxis( angles, re->axis );

	trap_S_StartSound (org, ENTITYNUM_NONE, CHAN_BODY, cgs.media.invulnerabilityJuicedSound );
}

#endif

/*
==================
CG_ScorePlum
==================
*/
void CG_ScorePlum( int client, vec3_t org, int score ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			angles;
	static vec3_t lastPos;

	// only visualize for the client that scored
	if (client != cg.predictedPlayerState.clientNum || cg_scorePlum.integer == 0) {
		return;
	}

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_SCOREPLUM;
	le->startTime = cg.time;
	le->endTime = cg.time + 4000;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	
	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
	le->radius = score;
	
	VectorCopy( org, le->pos.trBase );
	if (org[2] >= lastPos[2] - 20 && org[2] <= lastPos[2] + 20) {
		le->pos.trBase[2] -= 20;
	}

	//CG_Printf( "Plum origin %i %i %i -- %i\n", (int)org[0], (int)org[1], (int)org[2], (int)Distance(org, lastPos));
	VectorCopy(org, lastPos);


	re = &le->refEntity;

	re->reType = RT_SPRITE;
	re->radius = 16;

	VectorClear(angles);
	AnglesToAxis( angles, re->axis );
}


/*
====================
CG_MakeExplosion
====================
*/
localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir, 
								qhandle_t hModel, qhandle_t shader,
								int msec, qboolean isSprite ) {
	float			ang;
	localEntity_t	*ex;
	int				offset;
	vec3_t			tmpVec, newOrigin;

	if ( msec <= 0 ) {
		CG_Error( "CG_MakeExplosion: msec = %i", msec );
	}

	// skew the time a bit so they aren't all in sync
	offset = rand() & 63;

	ex = CG_AllocLocalEntity();
	if ( isSprite ) {
		ex->leType = LE_SPRITE_EXPLOSION;

		// randomly rotate sprite orientation
		ex->refEntity.rotation = rand() % 360;
		VectorScale( dir, 16, tmpVec );
		VectorAdd( tmpVec, origin, newOrigin );
	} else {
		ex->leType = LE_EXPLOSION;
		VectorCopy( origin, newOrigin );

		// set axis with random rotate
		if ( !dir ) {
			AxisClear( ex->refEntity.axis );
		} else {
			ang = rand() % 360;
			VectorCopy( dir, ex->refEntity.axis[0] );
			RotateAroundDirection( ex->refEntity.axis, ang );
		}
	}

	ex->startTime = cg.time - offset;
	ex->endTime = ex->startTime + msec;

	// bias the time so all shader effects start correctly
	ex->refEntity.shaderTime = ex->startTime / 1000.0f;

	ex->refEntity.hModel = hModel;
	ex->refEntity.customShader = shader;

	// set origin
	VectorCopy( newOrigin, ex->refEntity.origin );
	VectorCopy( newOrigin, ex->refEntity.oldorigin );

	ex->color[0] = ex->color[1] = ex->color[2] = 1.0;

	return ex;
}


/*
=================
CG_Bleed

This is the spurt of blood when a character gets hit
=================
*/
void CG_Bleed( vec3_t origin, int entityNum ) {
	localEntity_t	*ex;

	if ( !cg_blood.integer ) {
		return;
	}

	ex = CG_AllocLocalEntity();
	ex->leType = LE_EXPLOSION;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 500;
	
	VectorCopy ( origin, ex->refEntity.origin);
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = rand() % 360;
	ex->refEntity.radius = 24;

	ex->refEntity.customShader = cgs.media.bloodExplosionShader;

	// don't show player's own blood in view
	if ( entityNum == cg.snap->ps.clientNum ) {
		ex->refEntity.renderfx |= RF_THIRD_PERSON;
	}
}



/*
==================
CG_LaunchGib

`randSeed` has no effect if `cg_oldGibs.integer == 1`
==================
*/
void CG_LaunchGib( const vec3_t origin, const vec3_t angles,
					const vec3_t velocity, const qhandle_t hModel,
					const qboolean fireTrail,
					const int randSeed ) {
	localEntity_t	*le;
	refEntity_t		*re;
	int seed = randSeed;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 5000 + random() * 3000;

	VectorCopy( origin, re->origin );
	AnglesToAxis( angles, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.6f;
	if ( !cg_oldGibs.integer ) {
		// Same calculation as in `CG_ReflectVelocity`.
		float r = ( Q_random(&seed) + Q_random(&seed) ) / 2;
		le->bounceFactor = cg_gibsBounceFactor.value;
		le->bounceFactor *= 1 - r * cg_gibsBounceFactorRandomness.value;
	}

	if (!cg_oldGibs.integer) {
		// `VectorLength` would be more precise, but this is faster
		// and good enough for randomness.
		float speedIsh = fabs(velocity[0]) + fabs(velocity[1]) + fabs(velocity[2]);
		int i;
		int mainRotationAxis = Q_rand(&seed) % 3;

		le->leFlags = LEF_TUMBLE;
		le->angles.trType = TR_LINEAR;
		le->angles.trTime = cg.time;
		VectorCopy( angles, le->angles.trBase );
		// Just a few degrees of randomness.
		le->angles.trBase[PITCH] += Q_rand(&seed)&7;
		le->angles.trBase[YAW] += Q_rand(&seed)&7;
		le->angles.trBase[ROLL] += Q_rand(&seed)&7;
		// TODO the tumble speed should probably depend on damage instead,
		// or at least on random velocity.
		for ( i = 0; i < 3; i++ ) {
			// The numbers are not based on science, but it looks like
			// having one axis be bigger than others makes rotation
			// look natural (better preserve angular momentum).
			float axisMul = mainRotationAxis == i ? 1 : 0.375;
			le->angles.trDelta[i] = speedIsh * axisMul *
				cg_gibsRotationFactor.value * Q_crandom(&seed);
		}
		if ( fireTrail ) {
			le->light = LIGHT_GIB_FIRE_TRAIL;
		}
	}

	le->leBounceSoundType = LEBS_BLOOD;
	le->leMarkType = LEMT_BLOOD;
}

// If it's a dead body playing a death animation,
// gradually transition the body position and angles from upright
// to "lying flat on the ground".
void AdjustPositionIfDeathAnimation( const lerpFrame_t *anim, vec3_t origin,
	vec3_t bodyAngles, vec3_t lookDirAngles ) {
	// 0 means that the body is fully erect,
	// 1 means it's lying flat on the ground.
	float deathAnimationProgress = 0;
	if (
		// Is this a death / dead animation?
		(anim->animationNumber & ~ANIM_TOGGLEBIT) >= BOTH_DEATH1 &&
		(anim->animationNumber & ~ANIM_TOGGLEBIT) <= BOTH_DEAD3 &&
		// More sanity checks
		anim->animation &&
		anim->animation->numFrames > 0
	) {
		const int frameOfAnimation = anim->frame - anim->animation->firstFrame;
		// Usually the body is already on the ground
		// by the time ~half of the animation is played.
		int numFramesFalling = anim->animation->numFrames * 5 / 8;
		if ( numFramesFalling == 0 ) {
			numFramesFalling = 1;
		}

		if (
			frameOfAnimation < 0 ||
			frameOfAnimation >= anim->animation->numFrames
		) {
			// Out of range. This seems to happen
			// when we haven't yet managed to start the death animation.
			// Maybe we're looking at the wrong things,
			// but this works fine.
			deathAnimationProgress = 0;
		} else {
			deathAnimationProgress =
				(float)(frameOfAnimation + 1) / numFramesFalling;
		}
		if ( deathAnimationProgress > 1 ) {
			deathAnimationProgress = 1;
		}
	}

	// TODO fix: with body sinking, gibs get stuck in the floor.
	origin[2] += deathAnimationProgress * (MINS_Z + PLAYER_WIDTH / 1.8f);
	// From upright to facing up.
	// TODO fix: but sometimes the "dead" animation is such that
	// the player is facing down.
	bodyAngles[PITCH] = 360 - deathAnimationProgress * 90;
	lookDirAngles[PITCH] += - deathAnimationProgress * 90;
	// Normalize. Doesn't seem to be necessary, but let's do it.
	if (lookDirAngles[PITCH] < 0) {
		lookDirAngles[PITCH] += 360;
	}
}
/*
===================
CG_GibPlayer

Generated a bunch of gibs launching out from the bodies location

`randSeed` should be the same for all players, and also preserved
in demo playback, so that players see the same gibs
(as long as they have the same `cg_gibs*` CVAR values).
===================
*/
#define	DEFAULT_NUM_GIBS	10
#define	GIB_VELOCITY		250
#define	GIB_JUMP			250
void CG_GibPlayer( const vec3_t playerOrigin, const vec3_t playerAngles,
					const vec3_t playerVelocity, const int knockbackSpeed,
					const lerpFrame_t *bodyAnimation, const int randSeed ) {
	int i;
	vec3_t	baseOrigin, origin, velocity;
	// Generally only the head should have pitch,
	// the rest of the body is upright.
	vec3_t	bodyAngles;
	vec3_t	lookDirAngles, angles;
	vec3_t	forward, right, up;
	// See `playerMins`, `playerMaxs`.
	// TODO we could try to check the actual `mins` and `maxs`
	// (do we have them available on the client though?),
	// to account for crounching.
	float playerHeight = 32 - MINS_Z;
	float playerRadius = PLAYER_WIDTH;
	float baseRandomVelocity =
		cg_gibsExtraRandomVelocity.value +
		cg_gibsRandomVelocityFromKnockback.value * knockbackSpeed;
	int seed = randSeed;

	vec3_t playerVelocityScaled;
	float jump =
		cg_gibsExtraVerticalVelocity.value +
		cg_gibsVerticalVelocityFromKnockback.value * knockbackSpeed;
	const float numGibsFactor =
		1 +
		cg_gibsPiecesFromKnockback.value *
		// Take 500 knockback (100 damage) as the basis, min value.
		( (knockbackSpeed - 500) < 0 ? 0 : (knockbackSpeed - 500) ) /
		1000.0f;
	int numGibs = numGibsFactor * cg_gibs.value * DEFAULT_NUM_GIBS;
	const qboolean fireTrail =
		cg_gibsFireTrailMinKnockback.integer >= 0 &&
		knockbackSpeed >= cg_gibsFireTrailMinKnockback.integer;
	qboolean skullLaunched = qfalse; // launch only one skull.

	if ( !cg_blood.integer ) {
		return;
	}

	VectorCopy( playerOrigin, baseOrigin );
	VectorCopy( playerAngles, lookDirAngles );
	VectorCopy( playerAngles, bodyAngles );
	if ( bodyAnimation ) {
		AdjustPositionIfDeathAnimation( bodyAnimation, baseOrigin, bodyAngles, lookDirAngles );
	} else {
		bodyAngles[PITCH] = 0;
	}
	AngleVectors( bodyAngles, forward, right, up );

	VectorScale( playerVelocity, cg_gibsInheritPlayerVelocity.value, playerVelocityScaled );

	if ( cg_debugGibs.integer & 0x01 ) {
		CG_Printf( "gib:" );
		CG_Printf( " "S_COLOR_YELLOW"%i"S_COLOR_WHITE" pieces",
			numGibs );
		CG_Printf( ", speeds: player: "S_COLOR_YELLOW"%.1f",
			VectorLength( playerVelocityScaled ) );
		CG_Printf( " (vertical "S_COLOR_YELLOW"%.1f"S_COLOR_WHITE")",
			playerVelocityScaled[2] );
		CG_Printf( ", random: "S_COLOR_YELLOW"%.1f",
			baseRandomVelocity );
		CG_Printf( "\n" );
	}
	if ( cg_debugGibs.integer & 0x02 ) {
		CG_Printf( "     body pitch "S_COLOR_YELLOW"%.1f",
			AngleNormalize180( bodyAngles[PITCH] ) );
		CG_Printf( " yaw "S_COLOR_YELLOW"%.1f",
			AngleNormalize360( bodyAngles[YAW] ) );
		CG_Printf( ", knockback speed "S_COLOR_YELLOW"%i", knockbackSpeed );
		CG_Printf( ", random seed "S_COLOR_YELLOW"%i", seed );
		CG_Printf( "\n" );
	}

	do {
		// Note that one gib will get launched even if `numGibs == 0`.
		// This is in line with the original behavior of `CG_GibPlayer`.

		VectorCopy( baseOrigin, origin );
		VectorMA(origin, MINS_Z + 0.95 * playerHeight, up, origin);
		VectorClear( velocity );
		VectorMA( velocity, 0.5*Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, 0.5*Q_crandom(&seed)*baseRandomVelocity, right, velocity );
		// For the skull / brain we want the random velocity
		// to never have downwards (inwards) component,
		// so we use `Q_random` instead of `Q_crandom`
		// (and less randomness overall).
		// We also do the same for other gibs,
		// but for the left / right velocity components.
		VectorMA( velocity,
			(0.25 + 0.5 * (Q_random(&seed) + Q_random(&seed))/2)
				*baseRandomVelocity,
			up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		if ( !skullLaunched && (Q_rand(&seed) & 1) ) {
			CG_LaunchGib( origin, lookDirAngles, velocity, cgs.media.gibSkull, fireTrail, Q_rand(&seed) );
			skullLaunched = qtrue;
		} else {
			CG_LaunchGib( origin, lookDirAngles, velocity, cgs.media.gibBrain, fireTrail, Q_rand(&seed) );
		}
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.65 * playerHeight, up, origin );
		VectorClear( velocity );
		velocity[0] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[1] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[2] = jump + Q_crandom(&seed)*baseRandomVelocity;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		CG_LaunchGib( origin, bodyAngles, velocity, cgs.media.gibAbdomen, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.78 * playerHeight, up, origin );
		VectorMA( origin, 0.8 * playerRadius, right, origin );
		VectorMA( origin, -0.3 * playerRadius, forward, origin );
		VectorClear( velocity );
		VectorMA( velocity, +(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[ROLL] += 70;
		angles[PITCH] += 45;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibArm, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.80 * playerHeight, up, origin );
		VectorClear( velocity );
		// Chest is a more "central" and "heavier" piece,
		// so it gets less random velocity.
		velocity[0] = 0.5*Q_crandom(&seed)*baseRandomVelocity;
		velocity[1] = 0.5*Q_crandom(&seed)*baseRandomVelocity;
		velocity[2] = jump + 0.5*Q_crandom(&seed)*baseRandomVelocity;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		CG_LaunchGib( origin, bodyAngles, velocity, cgs.media.gibChest, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.66 * playerHeight, up, origin );
		VectorMA( origin, 0.8 * playerRadius, right, origin );
		VectorMA( origin, 0.2 * playerRadius, forward, origin );
		VectorClear( velocity );
		velocity[0] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[1] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[2] = jump + Q_crandom(&seed)*baseRandomVelocity;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[PITCH] -= 80;
		angles[YAW] += 50;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibFist, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.05 * playerHeight, up, origin );
		VectorMA( origin, -0.5 * playerRadius, right, origin );
		VectorMA( origin, -0.5 * playerRadius, forward, origin );
		// This piece is offset relatively far back,
		// and without this it might get stuck in the ground
		// depending on `bodyAngles[PITCH]`,
		// so let's ensure that it's high enough.
		if ( origin[2] < playerOrigin[2] + MINS_Z + 2 ) {
			origin[2] = playerOrigin[2] + MINS_Z + 2;
		}
		VectorClear( velocity );
		velocity[0] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[1] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[2] = jump + Q_crandom(&seed)*baseRandomVelocity;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		CG_LaunchGib( origin, bodyAngles, velocity, cgs.media.gibFoot, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.65 * playerHeight, up, origin );
		VectorMA( origin, -0.6 * playerRadius, right, origin );
		VectorMA( origin, +0.2 * playerRadius, forward, origin );
		VectorClear( velocity );
		VectorMA( velocity, -(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[ROLL] -= 90;
		angles[PITCH] -= 75;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibForearm, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.57 * playerHeight, up, origin );
		VectorClear( velocity );
		velocity[0] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[1] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[2] = jump + Q_crandom(&seed)*baseRandomVelocity;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		CG_LaunchGib( origin, bodyAngles, velocity, cgs.media.gibIntestine, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.42 * playerHeight, up, origin );
		VectorMA( origin, 0.5 * playerRadius, right, origin );
		VectorMA( origin, 0.1 * playerRadius, forward, origin );
		VectorClear( velocity );
		VectorMA( velocity, +(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[ROLL] -= 30;
		angles[PITCH] -= 15;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibLeg, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.44 * playerHeight, up, origin );
		VectorMA( origin, -0.5 * playerRadius, right, origin );
		VectorMA( origin, -0.2 * playerRadius, forward, origin );
		VectorClear( velocity );
		VectorMA( velocity, -(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[PITCH] += 15;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibLeg, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		// At this point all the "vanilla" gibs are placed,
		// and the rest runs only if there are more gibs than normal.

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.78 * playerHeight, up, origin );
		VectorMA( origin, -0.7 * playerRadius, right, origin );
		VectorMA( origin, -0.2 * playerRadius, forward, origin );
		VectorClear( velocity );
		VectorMA( velocity, -(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[ROLL] += 90;
		angles[YAW] += 180;
		angles[PITCH] -= 30;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibArm, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}
		
		// Use arm models as the lower legs
		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.23 * playerHeight, up, origin );
		VectorMA( origin, +0.5 * playerRadius, right, origin );
		VectorMA( origin, +0.0 * playerRadius, forward, origin );
		VectorClear( velocity );
		VectorMA( velocity, +(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[ROLL] += 90;
		angles[PITCH] += 10;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibArm, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.25 * playerHeight, up, origin );
		VectorMA( origin, -0.5 * playerRadius, right, origin );
		VectorMA( origin, -0.5 * playerRadius, forward, origin );
		// See another similar check above.
		if ( origin[2] < playerOrigin[2] + MINS_Z + 3 ) {
			origin[2] = playerOrigin[2] + MINS_Z + 3;
		}
		VectorClear( velocity );
		VectorMA( velocity, -(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		// TODO seems not to be rotated well when bodyAngles is not upright
		// (i.e. gib a dead player). Same for some other gibs.
		angles[ROLL] += 90;
		angles[YAW] += 180;
		angles[PITCH] -= 30;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibArm, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.05 * playerHeight, up, origin );
		VectorMA( origin, +0.5 * playerRadius, right, origin );
		VectorMA( origin, +0.1 * playerRadius, forward, origin );
		VectorClear( velocity );
		velocity[0] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[1] = Q_crandom(&seed)*baseRandomVelocity;
		velocity[2] = jump + Q_crandom(&seed)*baseRandomVelocity;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[PITCH] -= 45;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibFoot, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		// In Vanilla Quake this is the left forearm,
		// but let's also put it here upside down
		VectorCopy( baseOrigin, origin );
		VectorMA( origin, MINS_Z + 0.67 * playerHeight, up, origin );
		VectorMA( origin, +0.9 * playerRadius, right, origin );
		VectorMA( origin, -0.2 * playerRadius, forward, origin );
		VectorClear( velocity );
		VectorMA( velocity, +(0.25+0.5*Q_random(&seed))*baseRandomVelocity, right, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, forward, velocity );
		VectorMA( velocity, Q_crandom(&seed)*baseRandomVelocity, up, velocity );
		velocity[2] += jump;
		VectorAdd( velocity, playerVelocityScaled, velocity );
		VectorCopy( bodyAngles, angles );
		angles[ROLL] += 85;
		angles[PITCH] += 90;
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibForearm, fireTrail, Q_rand(&seed) );
		if (--numGibs <= 0) {
			return;
		}

		for ( i = 0; i < 7; i++ ) {
			VectorCopy( baseOrigin, origin );
			VectorMA( origin, MINS_Z + 0.60 * playerHeight, up, origin );
			origin[0] += Q_crandom(&seed) * playerRadius * 0.25;
			origin[1] += Q_crandom(&seed) * playerRadius * 0.25;
			origin[2] += Q_crandom(&seed) * playerRadius * 0.25;
			VectorClear( velocity );
			velocity[0] = Q_crandom(&seed)*baseRandomVelocity;
			velocity[1] = Q_crandom(&seed)*baseRandomVelocity;
			velocity[2] = jump + Q_crandom(&seed)*baseRandomVelocity;
			VectorAdd( velocity, playerVelocityScaled, velocity );
			VectorCopy( bodyAngles, angles );
			angles[0] += Q_random(&seed) * 360;
			angles[1] += Q_random(&seed) * 360;
			angles[2] += Q_random(&seed) * 360;
			CG_LaunchGib( origin, angles, velocity, cgs.media.gibIntestine, fireTrail, Q_rand(&seed) );
			if (--numGibs <= 0) {
				return;
			}
		}
	} while (numGibs > 0);
}
void CG_GibPlayer2( const centity_t *cent, const entityState_t *es,
					const clientInfo_t *ci ) {
	// With the new proto, `cent` is the temp event entity.
	// We need to get the actual player or corpse.
	const int targNum = cgs.g_gibsNewEvGibPlayerProtocol & 0x04
		? es->otherEntityNum
		: es->number;
	const centity_t *targCent = targNum == cg.snap->ps.clientNum
		? &cg.predictedPlayerEntity
		: &cg_entities[ targNum ];
	const entityState_t *targEs = &targCent->currentState;
	const qboolean targEsValid = targCent->currentValid ||
		targCent == &cg.predictedPlayerEntity;
	const int killer = es->eventParm;

	int knockbackSpeed = cgs.g_gibsNewEvGibPlayerProtocol & 0x02
		? es->generic1 * COMBAT_EV_GIB_PLAYER_ARG_DIVISOR
		// Also check the old Better Gibs mod protocol.
		// This is to support servers and replays with the old version
		// of the Better Gibs mod.
		// Not super necessary but why not.
		: cgs.g_gibsNewEvGibPlayerProtocol & 0x01
		? es->eventParm * COMBAT_EV_GIB_PLAYER_ARG_DIVISOR
		// Just use the default knockback speed for 100 damage.
		: 100 * 1000 / COMBAT_PLAYER_MASS;

	// Apparently at this point `targEs->pos.trDelta` doesn't yet have
	// the knockback from the damage that gibbed us,
	// so we have to differentiate between self and non-self
	// during regular (non-demo non-spectator) gameplay.
	const qboolean usePredictedPs =
		targNum == cg.snap->ps.clientNum &&
		!cg.demoPlayback &&
		!(cg.snap->ps.pm_flags & PMF_FOLLOW);
	// The new protocol has `trDelta` set to the player velocity
	// at the time of gib.
	// This is different from the velocity of the player or corpse (`targCent`),
	// which might have changed (due to `Pmove()` or `BG_EvaluateTrajectory()`)
	// between the gib event and the time when the snapshot was sent.
	// This is especially important if the knockback moved the player
	// against an obstacle such as a wall or the floor, clipping their velocity.
	// See https://github.com/WofWca/quake3-better-gibs-mod/issues/3.
	const vec3_t *vel =
		!( cgs.g_gibsNewEvGibPlayerProtocol & 0x08 ) && usePredictedPs
			? (const vec3_t*)&cg.predictedPlayerState.velocity
			: &es->pos.trDelta;

	// TODO: need to check `targEsValid`?
	// Probably not a big deal though.
	lerpFrame_t torsoAnimation = targCent->pe.torso;
	vec3_t torsoAngles;

	// TODO fix: things like `origin` and `angles`
	// are not in complete sync between clients,
	// so this seed is not always the same for all players.
	int randSeed = es->number;
	randSeed = Q_rand(&randSeed) + es->clientNum;
	randSeed = Q_rand(&randSeed) + es->eventParm;
	randSeed = Q_rand(&randSeed) + cgs.levelStartTime;
	// TODO fix: this varies from client to client.
	// So for now we round it to make it in sync ~95% of the time.
	randSeed = Q_rand(&randSeed) + cg.snap->serverTime / 2048;
	if ( ci ) {
		randSeed = Q_rand(&randSeed) + ci->name[0];
	}

	// Torso animation angles seem to be in better sync
	// between the local state and how others see us,
	// and overall are closer to other player's viewangles
	// than `cent->lerpAngles`.
	// `cent->lerpAngles`, seems to sometimes be pointing
	// in a completely different direction than the player's body
	// at the time of death.
	// Moreover, for non-self pitch seems to be always
	// not very far from 0.
	// This could be related to `LookAtKiller()`.
	// Also see `CG_PlayerAngles`.
	torsoAngles[PITCH] = torsoAnimation.pitchAngle;
	torsoAngles[YAW] = torsoAnimation.yawAngle;
	torsoAngles[ROLL] = 0;

	if ( cg_debugGibs.integer & 0x04 ) {
		vec3_t diff;
		float l;

		CG_Printf("EV_GIB_PLAYER:");
		CG_Printf(" time "S_COLOR_GREEN"%i.%03is",
			cg.time / 1000, cg.time % 1000 );
		// If the server sets `EF_PLAYER_EVENT`, this is never `true`.
		if ( targNum != es->number ) {
			CG_Printf(", ent "S_COLOR_GREEN"%i",
				es->number );
		}
		CG_Printf(", targ %s%i",
			targEsValid ? S_COLOR_GREEN : S_COLOR_RED,
			targNum );

		// Yellow means a big difference, but usually it means
		// that it's an innacuracy that we fixed by using the new protocol,
		// where the position and velocity are fixed at what they were
		// the moment the player got gibbed on the server,
		// i.e. they are not interpolated.
		VectorSubtract( targEs->pos.trBase, es->pos.trBase, diff );
		l = VectorLength( diff );
		CG_Printf(", pos diff %s%.1f",
			l > 750 ? S_COLOR_RED : l > 100 ? S_COLOR_YELLOW : S_COLOR_GREEN,
			VectorLength( diff ) );
		VectorSubtract( targEs->pos.trDelta, *vel, diff );
		l = VectorLength( diff );
		CG_Printf(", vel diff %s%.1f",
			l > 1500 ? S_COLOR_RED : l > 100 ? S_COLOR_YELLOW : S_COLOR_GREEN,
			VectorLength( diff ) );

		CG_Printf(", usePredictedPs %s%i\n",
			usePredictedPs ? S_COLOR_GREEN : S_COLOR_CYAN,
			usePredictedPs );
	}

	CG_GibPlayer( cent->lerpOrigin, torsoAngles, *vel, knockbackSpeed,
		&torsoAnimation, randSeed );
}

void CG_GibPlayerOld( vec3_t playerOrigin ) {
	vec3_t	origin, angles, velocity;

	if ( !cg_blood.integer ) {
		return;
	}

	VectorClear(angles);

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	if ( rand() & 1 ) {
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibSkull, qfalse, 0 );
	} else {
		CG_LaunchGib( origin, angles, velocity, cgs.media.gibBrain, qfalse, 0 );
	}

	// allow gibs to be turned off for speed
	if ( !cg_gibs.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibAbdomen, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibArm, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibChest, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibFist, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibFoot, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibForearm, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibIntestine, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibLeg, qfalse, 0 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	CG_LaunchGib( origin, angles, velocity, cgs.media.gibLeg, qfalse, 0 );
}

/*
==================
CG_LaunchExplode
==================
*/
void CG_LaunchExplode( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 10000 + random() * 6000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.1f;

	le->leBounceSoundType = LEBS_BRASS;
	le->leMarkType = LEMT_NONE;
}

#define	EXP_VELOCITY	100
#define	EXP_JUMP		150
/*
===================
CG_BigExplode

Generated a bunch of gibs launching out from the bodies location
===================
*/
void CG_BigExplode( vec3_t playerOrigin ) {
	vec3_t	origin, velocity;

	if ( !cg_blood.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*1.5;
	velocity[1] = crandom()*EXP_VELOCITY*1.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.0;
	velocity[1] = crandom()*EXP_VELOCITY*2.0;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.5;
	velocity[1] = crandom()*EXP_VELOCITY*2.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
}

