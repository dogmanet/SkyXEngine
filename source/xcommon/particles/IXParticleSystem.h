#ifndef __IXPARTICLESYSTEM_H
#define __IXPARTICLESYSTEM_H

#include <gdefines.h>
#include <xcommon/util/IXMinMaxCurve.h>

//##########################################################################

// {811217A3-FEEB-468D-AF58-8640CF758683}
#define IXPARTICLESYSTEM_GUID DEFINE_XGUID(0x811217a3, 0xfeeb, 0x468d, 0xaf, 0x58, 0x86, 0x40, 0xcf, 0x75, 0x86, 0x83)
#define IXPARTICLESYSTEM_VERSION 1

//##########################################################################

// IXParticleEffect
// IXParticleEffectEmitter

// IXParticlePlayer
// IXParticleEmitter

//! The action to perform when the Particle System is offscreen.
enum XPARTICLE_CULLING_MODE
{
	XPCM_AUTO,        //!< For looping effects, the simulation is paused when offscreen, and for one-shot effects, the simulation will continue playing.
	XPCM_SMART_PAUSE, //!< Pause the Particle System simulation when it is offscreen, and perform an extra simulation when the system comes back onscreen, creating the impression that it was never paused.
	XPCM_PAUSE,       //!< Pause the Particle System simulation when it is offscreen.
	XPCM_ALWAYS_RUN   //!< Continue simulating the Particle System when it is offscreen.
};

//! Control how particles are removed from the Particle System.
enum XPARTICLE_RING_BUFFER_MODE
{
	XPRBM_DISABLED, //!< Particles are removed when their age exceeds their lifetime.
	XPRBM_PAUSE,    //!< Particle ages pause at the end of their lifetime until they need to be removed. Particles are removed when creating new particles would exceed the Max Particles property.
	XPRBM_LOOP      //!< Particle ages loop until they need to be removed. Particles are removed when creating new particles would exceed the Max Particles property.
};

//! The space to simulate particles in.
enum XPARTICLE_SIMULATION_SPACE
{
	XPSS_LOCAL, //!< Simulate particles in local space.
	XPSS_WORLD, //!< Simulate particles in world space.
	//XPSS_CUSTOM //!< Simulate particles relative to a custom transform component, defined by ParticleSystem.MainModule.customSimulationSpace.
};

//
enum XPARTICLE_SHAPE
{
	XPS_SPHERE,
	XPS_HEMISPHERE,
	XPS_CONE,
	XPS_BOX,
	XPS_CIRCLE,
	XPS_EDGE,
	XPS_TORUS,
	XPS_RECTANGLE
};

//! The part of the cone to emit particles from
enum XPARTICLE_SHAPE_CONE_EMIT_FROM
{
	XPSCEF_BASE,
	XPSCEF_VOLUME
};

//! Select the part of the box to emit from
enum XPARTICLE_SHAPE_BOX_EMIT_FROM
{
	XPSBEF_EDGE,
	XPSBEF_SHELL,
	XPSBEF_VOLUME
};

enum XPARTICLE_SHAPE_ARC_MODE
{
	XPSAM_RANDOM,
	XPSAM_LOOP,
	XPSAM_PING_PONG,
	//XPSAM_PING_LOOP,
	XPSAM_BURST_SPREAD
};

class IXParticleEffectEmitterGenericData
{
public:
	//! The length of time the emitter runs
	/*-*/XMETHOD_GETSET(Duration, float, fValue);

	//! If enabled, the emitter starts again at the end of its duration time and continues to repeat the cycle
	/*-*/XMETHOD_GETSET(Looping, bool, yesNo);

	//! If enabled, the emitter is initialized as though it had already completed a full cycle (only works if Looping is also enabled).
	/*-*/XMETHOD_GETSET(Prewarm, bool, yesNo);

	//! Delay in seconds before the emitter starts emitting once enabled
	XMETHOD_GETSET(StartDelay, float, fValue);

	//! The initial lifetime for particles.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartLifetimeCurve);

	//! The initial speed of each particle in the appropriate direction.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartSpeedCurve);

	//! The initial size of each particle.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartSizeCurve);

	//! Makes some particles spin in the opposite direction
	XMETHOD_GETSET(FlipRotation, float, fValue);

	//! Configure whether the Particle System will still be simulated each frame, when it is offscreen
	/*-*/XMETHOD_GETSET(CullingMode, XPARTICLE_CULLING_MODE, mode);

	//! A scale that this Particle System applies to gravity, defined by Physics.gravity.
	/*-*/XMETHOD_GETSET(GravityModifier, float, fValue);

	//! The maximum number of particles to emit.
	XMETHOD_GETSET(MaxParticles, UINT, uCount);

	//! Configure the Particle System to not kill its particles when their lifetimes are exceeded
	/*-*/XMETHOD_GETSET(RingBufferMode, XPARTICLE_RING_BUFFER_MODE, mode);

	//! When ParticleSystem.MainModule.ringBufferMode is set to loop, this value defines the proportion of the particle life that loops.
	/*-*/XMETHOD_2CONST(IXMinMaxCurve*, getRingBufferLoopRangeCurve);

	//! This selects the space in which to simulate particles. It can be either world or local space.
	/*!*/XMETHOD_GETSET(SimulationSpace, XPARTICLE_SIMULATION_SPACE, simulationSpace);

	//! The initial color of particles when the Particle System first spawns them.
	XMETHOD_GETSET_REF(StartColor, float4_t, vColor);

	//! The initial rotation of particles when the Particle System first spawns them.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartRotationCurve);

	//! A flag to enable 3D particle rotation.
	XMETHOD_GETSET(StartRotationSeparate, bool, yesNo);

	//! The initial rotation of particles around the x-axis when emitted.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartRotationXCurve);

	//! The initial rotation of particles around the y-axis when the Particle System first spawns them.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartRotationYCurve);

	//! The initial rotation of particles around the z-axis when the Particle System first spawns them.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartRotationZCurve);
	
	//! A flag to enable specifying particle size individually for each axis.
	XMETHOD_GETSET(StartSizeSeparate, bool, yesNo);

	//! The initial rotation of particles around the x-axis when emitted.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartSizeXCurve);

	//! The initial rotation of particles around the y-axis when emitted.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartSizeYCurve);

	//! The initial rotation of particles around the z-axis when emitted.
	XMETHOD_2CONST(IXMinMaxCurve*, getStartSizeZCurve);
};

class IXParticleBurst
{
public:
	//! Set the time (in seconds, after the Particle System begins playing) at which to emit the burst.
	XMETHOD_GETSET(Time, float, fValue);

	//! Set a value for how many times to play the burst.
	XMETHOD_GETSET(Cycles, UINT, uValue);

	//! Controls how likely it is that each burst event spawns particles. A higher value makes the system produce more particles, and a value of 1 guarantees that the system produces particles.
	XMETHOD_GETSET(Probability, float, fValue);

	//! Set a value for the number of particles that may be emitted.
	XMETHOD_2CONST(IXMinMaxCurve*, getCountCurve);

	//! Set a value for the time (in seconds) between when each cycle of the burst is triggered.
	XMETHOD_2CONST(IXMinMaxCurve*, getIntervalCurve);
};

class IXParticleEffectEmitterEmissionData
{
public:
	//! The number of particles emitted per second.
	XMETHOD_2CONST(IXMinMaxCurve*, getRatePerSecondCurve);

	//! The number of particles emitted per meter of distance moved.
	XMETHOD_2CONST(IXMinMaxCurve*, getRatePerMeterCurve);

	//! A burst is an event which spawns particles. These settings allow particles to be emitted at specified times.
	//!{
	XMETHOD_GETSET(BurstsCount, UINT, uCount);

	XMETHOD_2CONST(IXParticleBurst*, getBurstAt, UINT uIndex);
	//!}
};

class IXParticleEffectEmitterShapeData
{
public:
	/* *** Shape module ***
	* This module defines the the volume or surface from which particles
	* can be emitted, and the direction of the start velocity. The Shape property defines the shape of the emission volume, and the rest of the module properties vary depending on the Shape you choose.
	* 
	* All shapes (except Mesh) have properties that define their dimensions, such as the Radius property. To edit these, drag the handles on the wireframe emitter shape in the Scene
	* view. The choice of shape affects the region from which particles can be launched, but also the initial direction of the particles. For example, a Sphere emits particles outward in all directions, a Cone emits a diverging stream of particles.
	* 
	* The section below details the properties for each Shape
	
	XPARTICLE_SHAPE shape // The shape of the emission volume
		Sphere // Uniform particle emission in all directions.
		Hemisphere // Uniform particle emission in all directions on one side of a plane.
			float fRadius // The radius of the circular aspect of the shape.
			float fRadiusThickness // The proportion of the volume that emits particles. A value of 0 emits particles from the outer surface of the shape. A value of 1 emits particles from the entire volume. Values in between will use a proportion of the volume.
			
		Cone // Emit particles from the base or body of a cone. The particles diverge in proportion to their distance from the cone’s center line.
			float fAngle // The angle of the cone at its point. An angle of 0 produces a cylinder while an angle of 90 gives a flat disc.
			float fRadius // The radius of the circular aspect of the shape.
			float fRadiusThickness // The proportion of the volume that emits particles.A value of 0 emits particles from the outer surface of the shape. A value of 1 emits particles from the entire volume. Values in between will use a proportion of the volume.
			float fArc // The angular portion of a full circle that forms the emitter’s shape.
				XPARTICLE_SHAPE_ARC_MODE mode // Define how Unity generates particles around the arc of the shape. When set to Random, Unity generates particles randomly around the arc. If using Loop, Unity generates particles sequentially around the arc of the shape, and loops back to the start at the end of each cycle. Ping-Pong is the same as Loop, except each consecutive loop happens in the opposite direction to the last. Finally, Burst Spread mode distributes particle generation evenly around the shape. This can give you an even spread of particles, compared to the default randomized behavior, where particles may clump together unevenly. Burst Spread is best used with burst emissions.
				float fSpread // The discrete intervals around the arc where particles may be generated. For example, a value of 0 allows particles to spawn anywhere around the arc, and a value of 0.1 only spawns particles at 10% intervals around the shape.
				??? Speed // The speed the emission position moves around the arc. Using the small black drop-down menu next to the value field, set this to Constant for the value to always remain the same, or Curve for the value to change over time. This option is only available if Mode is set to something other than Random
			float fLength // The length of the cone. This only applies when the Emit from: property is set to Volume.
			XPARTICLE_SHAPE_CONE_EMIT_FROM emitFrom // The part of the cone to emit particles from: Base or Volume.
			
		Box // Emit particles from the edge, surface, or body of a box shape. The particles move in the emitter object’s forward (Z) direction.
			XPARTICLE_SHAPE_BOX_EMIT_FROM emitFrom // Select the part of the box to emit from: Edge, Shell, or Volume.
			
		Circle // Uniform particle emission from the center or edge of a circle. The particles move only in the plane of the circle.
			float fRadius // The radius of the circular aspect of the shape.
			float fRadiusThickness // The proportion of the volume that emits particles. A value of 0 emits particles from the outer edge of the circle. A value of 1 emits particles from the entire area. Values in between will use a proportion of the area.
			float fArc // The angular portion of a full circle that forms the emitter’s shape.
				XPARTICLE_SHAPE_ARC_MODE mode // Define how Unity generates particles around the arc of the shape. When set to Random, Unity generates particles randomly around the arc. If using Loop, Unity generates particles sequentially around the arc of the shape, and loops back to the start at the end of each cycle. Ping-Pong is the same as Loop, except each consecutive loop happens in the opposite direction to the last. Finally, Burst Spread mode distributes particle generation evenly around the shape. This can give you an even spread of particles, compared to the default randomized behavior, where particles may clump together unevenly. Burst Spread is best used with burst emissions.
				float fSpread // The discrete intervals around the arc where particles may be generated. For example, a value of 0 allows particles to spawn anywhere around the arc, and a value of 0.1 only spawns particles at 10% intervals around the shape.
				??? Speed // The speed the emission position moves around the arc. Using the small black drop-down menu next to the value field, set this to Constant for the value to always remain the same, or Curve for the value to change over time. This option is only available if Mode is set to something other than Random
				
		Edge // Emit particles from a line segment. The particles move in the emitter object’s upward (Y) direction.
			float fRadius // The radius property is used to define the length of the edge.
				XPARTICLE_SHAPE_ARC_MODE mode // Define how Unity generates particles around the arc of the shape. When set to Random, Unity generates particles randomly around the arc. If using Loop, Unity generates particles sequentially around the arc of the shape, and loops back to the start at the end of each cycle. Ping-Pong is the same as Loop, except each consecutive loop happens in the opposite direction to the last. Finally, Burst Spread mode distributes particle generation evenly around the shape. This can give you an even spread of particles, compared to the default randomized behavior, where particles may clump together unevenly. Burst Spread is best used with burst emissions.
				float fSpread // The discrete intervals around the arc where particles may be generated. For example, a value of 0 allows particles to spawn anywhere around the arc, and a value of 0.1 only spawns particles at 10% intervals around the shape.
				??? Speed // The speed the emission position moves around the arc. Using the small black drop-down menu next to the value field, set this to Constant for the value to always remain the same, or Curve for the value to change over time. This option is only available if Mode is set to something other than Random
				
		Torus // Emit particles from a torus. The particles move outwards from the ring of the Torus.
			float fRadius // The radius of the main donut ring.
			float fDonusRadius // The thickness of the outer donut ring.
			float fRadiusThickness // The proportion of the volume that emits particles. A value of 0 emits particles from the outer edge of the circle. A value of 1 emits particles from the entire area. Values in between will use a proportion of the area.
			float fArc // The angular portion of a full circle that forms the emitter’s shape.
				XPARTICLE_SHAPE_ARC_MODE mode // Define how Unity generates particles around the arc of the shape. When set to Random, Unity generates particles randomly around the arc. If using Loop, Unity generates particles sequentially around the arc of the shape, and loops back to the start at the end of each cycle. Ping-Pong is the same as Loop, except each consecutive loop happens in the opposite direction to the last. Finally, Burst Spread mode distributes particle generation evenly around the shape. This can give you an even spread of particles, compared to the default randomized behavior, where particles may clump together unevenly. Burst Spread is best used with burst emissions.
				float fSpread // The discrete intervals around the arc where particles may be generated. For example, a value of 0 allows particles to spawn anywhere around the arc, and a value of 0.1 only spawns particles at 10% intervals around the shape.
				??? Speed // The speed the emission position moves around the arc. Using the small black drop-down menu next to the value field, set this to Constant for the value to always remain the same, or Curve for the value to change over time. This option is only available if Mode is set to something other than Random
				
		Rectangle // Emits particles from a rectangle. The particles move up from the rectangle.
		
	bool bAlignToDirection // Orient particles based on their initial direction of travel. This can be useful if you want to simulate, for example, chunks of car paint flying off a car’s bodywork during a collision. If the orientation is not satisfactory, you can also override it by applying a Start Rotation value in the Main module.
	float fRandomizeDirection // Blend particle directions towards a random direction. When set to 0, this setting has no effect. When set to 1, the particle direction is completely random.
	float fSpherizeDirection // Blend particle directions towards a spherical direction, where they travel outwards from the center of their Transform. When set to 0, this setting has no effect. When set to 1, the particle direction points outwards from the center (behaving identically to when the Shape is set to Sphere).
	float fRandomizePosition // Move particles by a random amount, up to the specified value. When this is set to 0, this setting has no effect. Any other value will apply some randomness to the spawning positions of the particles.
	*/

	//! The shape of the emission volume
	XMETHOD_GETSET(Shape, XPARTICLE_SHAPE, shape);

	//! The radius of the circular aspect of the shape.
	XMETHOD_GETSET(Radius, float, fValue);

	//! The proportion of the volume that emits particles. A value of 0 emits particles from the outer surface of the shape. A value of 1 emits particles from the entire volume. Values in between will use a proportion of the volume.
	XMETHOD_GETSET(RadiusThickness, float, fValue);

	//! The angle of the cone at its point. An angle of 0 produces a cylinder while an angle of 90 gives a flat disc.
	XMETHOD_GETSET(Angle, float, fValue);

	//! The angular portion of a full circle that forms the emitter’s shape.
	XMETHOD_GETSET(Arc, float, fValue);

	//! Define how Unity generates particles around the arc of the shape. When set to Random, Unity generates particles randomly around the arc. If using Loop, Unity generates particles sequentially around the arc of the shape, and loops back to the start at the end of each cycle. Ping-Pong is the same as Loop, except each consecutive loop happens in the opposite direction to the last. Finally, Burst Spread mode distributes particle generation evenly around the shape. This can give you an even spread of particles, compared to the default randomized behavior, where particles may clump together unevenly. Burst Spread is best used with burst emissions.
	XMETHOD_GETSET(ArcMode, XPARTICLE_SHAPE_ARC_MODE, mode);

	//! The discrete intervals around the arc where particles may be generated. For example, a value of 0 allows particles to spawn anywhere around the arc, and a value of 0.1 only spawns particles at 10% intervals around the shape.
	XMETHOD_GETSET(ArcSpread, float, fValue);

	//! The speed the emission position moves around the arc. Using the small black drop-down menu next to the value field, set this to Constant for the value to always remain the same, or Curve for the value to change over time. This option is only available if Mode is set to something other than Random
	XMETHOD_2CONST(IXMinMaxCurve*, getArcSpeedCurve);

	//! The length of the cone. This only applies when the Emit from: property is set to Volume.
	XMETHOD_GETSET(Length, float, fValue);

	//! The part of the cone to emit particles from: Base or Volume.
	XMETHOD_GETSET(ConeEmitFrom, XPARTICLE_SHAPE_CONE_EMIT_FROM, emitFrom);

	//! Select the part of the box to emit from: Edge, Shell, or Volume.
	XMETHOD_GETSET(BoxEmitFrom, XPARTICLE_SHAPE_BOX_EMIT_FROM, emitFrom);

	//! The thickness of the outer donut ring.
	XMETHOD_GETSET(DonutRadius, float, fValue);

	//! Orient particles based on their initial direction of travel. This can be useful if you want to simulate, for example, chunks of car paint flying off a car’s bodywork during a collision. If the orientation is not satisfactory, you can also override it by applying a Start Rotation value in the Main module.
	/*!*/XMETHOD_GETSET(AlignToDirection, bool, yesNo);

	//! Blend particle directions towards a random direction. When set to 0, this setting has no effect. When set to 1, the particle direction is completely random.
	XMETHOD_GETSET(RandomizeDirection, float, fValue);

	//! Blend particle directions towards a spherical direction, where they travel outwards from the center of their Transform. When set to 0, this setting has no effect. When set to 1, the particle direction points outwards from the center (behaving identically to when the Shape is set to Sphere).
	XMETHOD_GETSET(SpherizeDirection, float, fValue);

	//! Move particles by a random amount, up to the specified value. When this is set to 0, this setting has no effect. Any other value will apply some randomness to the spawning positions of the particles.
	XMETHOD_GETSET(RandomizePosition, float, fValue);

	XMETHOD_GETSET_REF(Size, float3_t, vSize);
};

class IXParticleEffectEmitterVelocityLifetimeData
{
public:
	/* *** Velocity over Lifetime module ***
	* The Velocity over Lifetime module allows you to control the velocity of particles over their lifetime.
	*
	* To create particles that drift in a particular direction, use the Linear X, Y and Z curves.
	* 
	* To create effects with particles that spin around a center position, use the Orbital velocity values. Additionally, you can propel particles towards or away from a center position using the Radial velocity values. You can define a custom center of rotation for each particle by using the Offset value.
	* 
	* You can also use this module to adjust the speed of the particles in the Particle System
	without affecting their direction, by leaving all the above values at zero and only modifying the Speed Modifier value.
	*/

	virtual bool XMETHODCALLTYPE isEnabled() const = 0;
	virtual void XMETHODCALLTYPE enable(bool yesNo) = 0;

	//! Linear velocity of particles in the X, Y and Z axes.
	//!{
	XMETHOD_2CONST(IXMinMaxCurve*, getLinearXCurve);
	XMETHOD_2CONST(IXMinMaxCurve*, getLinearYCurve);
	XMETHOD_2CONST(IXMinMaxCurve*, getLinearZCurve);
	//!}

	//! Specifies whether the Linear X, Y, Z axes refer to local or world space.
	XMETHOD_GETSET(SimulationSpace, XPARTICLE_SIMULATION_SPACE, simulationSpace);

	//! Orbital velocity of particles around the X, Y and Z axes.
	//!{
	XMETHOD_2CONST(IXMinMaxCurve*, getOrbitalXCurve);
	XMETHOD_2CONST(IXMinMaxCurve*, getOrbitalYCurve);
	XMETHOD_2CONST(IXMinMaxCurve*, getOrbitalZCurve);
	//!}

	//! The position of the center of orbit, for orbiting particles.
	//!{
	XMETHOD_2CONST(IXMinMaxCurve*, getOffsetXCurve);
	XMETHOD_2CONST(IXMinMaxCurve*, getOffsetYCurve);
	XMETHOD_2CONST(IXMinMaxCurve*, getOffsetZCurve);
	//!}

	//! Radial velocity of particles away from/towards the center position.
	XMETHOD_2CONST(IXMinMaxCurve*, getRadialCurve);

	//! Applies a multiplier to the speed of particles, along/around their current direction of travel.
	XMETHOD_2CONST(IXMinMaxCurve*, getSpeedModifierCurve);
};

class IXParticleEffectEmitterLimitVelocityLifetimeData
{
public:
	/* *** Limit Velocity Over Lifetime module ***
	* This module controls how the speed of particles is reduced over their lifetime.
	* 
	* This module is very useful for simulating air resistance that slows the particles, especially when a decreasing curve is used to lower the speed limit over time. For example, an explosion or firework initially bursts at great speed but the particles emitted from it rapidly slow down as they pass through the air.
	*
	* The Drag option offers a more physically accurate simulation of air resistance by offering options to apply varying amounts of resistance based on the size and speed of the particles.
	*/

	virtual bool XMETHODCALLTYPE isEnabled() const = 0;
	virtual void XMETHODCALLTYPE enable(bool yesNo) = 0;

	//! Splits the axes up into separate X, Y and Z components.
	/*-*/XMETHOD_GETSET(SeparateAxes, bool, yesNo);

	//! Selects whether the speed limitation refers to local or world space. This option is only available when Separate Axes is enabled.
	/*-*/XMETHOD_GETSET(SimulationSpace, XPARTICLE_SIMULATION_SPACE, simulationSpace);

	//! Sets the speed limit of the particles.
	/*-*/XMETHOD_2CONST(IXMinMaxCurve*, getSpeedCurve);

	//! The fraction by which a particle’s speed is reduced when it exceeds the speed limit.
	/*-*/XMETHOD_2CONST(IXMinMaxCurve*, getDampenCurve);

	//! Applies linear drag to the particle velocities.
	/*-*/XMETHOD_2CONST(IXMinMaxCurve*, getDragCurve);

	//! When enabled, larger particles are affected more by the drag coefficient.
	/*-*/XMETHOD_GETSET(MultiplyBySize, bool, yesNo);

	//! When enabled, faster particles are affected more by the drag coefficient.
	/*-*/XMETHOD_GETSET(MultiplyByVelocity, bool, yesNo);
};


class IXParticleEffectEmitter: public IXUnknown
{
public:
	XMETHOD_2CONST(IXParticleEffectEmitterGenericData*, getGenericData);
	XMETHOD_2CONST(IXParticleEffectEmitterEmissionData*, getEmissionData);
	XMETHOD_2CONST(IXParticleEffectEmitterShapeData*, getShapeData);
	XMETHOD_2CONST(IXParticleEffectEmitterVelocityLifetimeData*, getVelocityLifetimeData);
	XMETHOD_2CONST(IXParticleEffectEmitterLimitVelocityLifetimeData*, getLimitVelocityLifetimeData);


	/* *** Inherit Velocity module ***
	
	*/
	
	/* *** Force Over Lifetime module ***
	
	*/
	
	/* *** Color Over Lifetime module ***
	
	*/
	
	/* *** Color By Speed module ***
	
	*/
	
	/* *** Size over Lifetime module ***
	
	*/
	
	/* *** Size by Speed module ***
	
	*/
	
	/* *** Rotation Over Lifetime module ***
	
	*/
	
	/* *** Rotation By Speed module ***
	
	*/
	
	/* *** Collision module ***
	
	*/
	
	/* *** Sub Emitters module ***
	
	*/
	
	/* *** Lights module ***
	
	*/
	
	/* *** Trails module ***
	
	*/
	
	/* *** Renderer module ***
	
	*/
	
	/* *** Lifetime by Emitter Speed module ***
	
	*/
		
	/*
	+Main module
	+Emission module
	Shape Module
	Renderer module
	
	Velocity over Lifetime module
	Limit Velocity Over Lifetime module
	Inherit Velocity module
	
	Force Over Lifetime module
	
	Color Over Lifetime module
	Color By Speed module
	
	Size over Lifetime module
	Size by Speed module
	
	Rotation Over Lifetime module
	Rotation By Speed module
	
	Collision module
	
	Sub Emitters module
	
	Lights module
	
	Trails module
	
	
	Lifetime by Emitter Speed
	*/
	
	// pos, rot
};

class IXParticleEffect: public IXUnknown
{
public:
	virtual UINT XMETHODCALLTYPE getEmitterCount() = 0;
	virtual void XMETHODCALLTYPE setEmitterCount(UINT uCount) = 0;
	virtual IXParticleEffectEmitter* XMETHODCALLTYPE getEmitterAt(UINT uIdx) = 0;
	
	virtual bool XMETHODCALLTYPE save() = 0;
};

class IXParticlePlayer: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE play() = 0;
	virtual void XMETHODCALLTYPE pause() = 0;
	virtual void XMETHODCALLTYPE stop(bool bClear = false) = 0;
	virtual void XMETHODCALLTYPE clear() = 0;
	
	virtual void XMETHODCALLTYPE simulate(float fTime, bool bRestart = false) = 0;
	
	virtual bool XMETHODCALLTYPE isEmitting() = 0;
	virtual bool XMETHODCALLTYPE isPaused() = 0;
	virtual bool XMETHODCALLTYPE isPlaying() = 0;
	virtual bool XMETHODCALLTYPE isStopped() = 0;
	virtual bool XMETHODCALLTYPE isAlive() = 0;
	
	virtual UINT XMETHODCALLTYPE getParticleCount() = 0;
	virtual float XMETHODCALLTYPE getTime() = 0;
	
	virtual float3_t XMETHODCALLTYPE getPos() = 0;
	virtual void XMETHODCALLTYPE setPos(const float3_t &vPos) = 0;

	virtual SMQuaternion XMETHODCALLTYPE getOrient() = 0;
	virtual void XMETHODCALLTYPE setOrient(const SMQuaternion &qRot) = 0;
};

class IXParticleSystem: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE newEffect(const char *szName, IXParticleEffect **ppOut) = 0;
	virtual bool XMETHODCALLTYPE getEffect(const char *szName, IXParticleEffect **ppOut) = 0;
	
	virtual void XMETHODCALLTYPE newEffectPlayer(IXParticleEffect *pEffect, IXParticlePlayer **ppOut) = 0;
	// virtual void XMETHODCALLTYPE newEffectEmitter(IXParticleEffect *pEffect, IXParticleEmitter **ppOut) = 0;
};

#endif
