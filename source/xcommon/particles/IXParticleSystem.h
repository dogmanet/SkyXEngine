#ifndef __IXPARTICLESYSTEM_H
#define __IXPARTICLESYSTEM_H

#include <gdefines.h>

//##########################################################################

// {811217A3-FEEB-468D-AF58-8640CF758683}
#define IXPARTICLESYSTEM_GUID DEFINE_XGUID(0x811217a3, 0xfeeb, 0x468d, 0xaf, 0x58, 0x86, 0x40, 0xcf, 0x75, 0x86, 0x83)
#define IXPARTICLESYSTEM_VERSION 1

//##########################################################################

// IXParticleEffect
// IXParticleEffectEmitter

// IXParticlePlayer
// IXParticleEmitter

class IXParticleEffectEmitter: public IXUnknown
{
public:
	/* *** Main module ***
	float duration // The length of time the emitter runs
	bool looping // If enabled, the emitter starts again at the end of its duration time and continues to repeat the cycle
	bool prewarm // If enabled, the emitter is initialized as though it had already completed a full cycle (only works if Looping is also enabled).
	float startDelay // Delay in seconds before the emitter starts emitting once enabled
	MinMaxCurve startLifetime // The initial lifetime for particles.
	MinMaxCurve startSpeed // The initial speed of each particle in the appropriate direction
	MinMaxCurve startSize // The initial size of each particle
	float flipRotation // Makes some particles spin in the opposite direction.
	XPARTICLE_CULLING_MODE cullingMode // Configure whether the Particle System will still be simulated each frame, when it is offscreen.
	float gravityModifier // A scale that this Particle System applies to gravity, defined by Physics.gravity.
	UINT maxParticles // The maximum number of particles to emit.
	XPARTICLE_RING_BUFFER_MODE ringBufferMode // Configure the Particle System to not kill its particles when their lifetimes are exceeded.
	MinMaxCurve ringBufferLoopRange // When ParticleSystem.MainModule.ringBufferMode is set to loop, this value defines the proportion of the particle life that loops.
	XPARTICLE_SIMULATION_SPACE simulationSpace // This selects the space in which to simulate particles. It can be either world or local space.
	float3_t startColor // The initial color of particles when the Particle System first spawns them.
	MinMaxCurve startRotation // The initial rotation of particles when the Particle System first spawns them.
	bool startRotationSeparate // A flag to enable 3D particle rotation.
	MinMaxCurve startRotationX // The initial rotation of particles around the x-axis when emitted.
	MinMaxCurve startRotationY // The initial rotation of particles around the y-axis when the Particle System first spawns them.
	MinMaxCurve startRotationZ // The initial rotation of particles around the z-axis when the Particle System first spawns them
	MinMaxCurve startSize // The initial size of particles when the Particle System first spawns them.
	bool startSizeSeparate // A flag to enable specifying particle size individually for each axis.
	MinMaxCurve startSizeX // The initial size of particles along the x-axis when the Particle System first spawns them.
	MinMaxCurve startSizeY // The initial size of particles along the y-axis when the Particle System first spawns them.
	MinMaxCurve startSizeZ // The initial size of particles along the z-axis when the Particle System first spawns them.
	*/
	
	/* *** Emission module ***
	MinMaxCurve ratePerSecond // The number of particles emitted per second.
	MinMaxCurve ratePerMeter // The number of particles emitted per meter of distance moved.
	
	Bursts	A burst is an event which spawns particles. These settings allow particles to be emitted at specified times.
		float Time // Set the time (in seconds, after the Particle System begins playing) at which to emit the burst.
		MinMaxCurve Count // Set a value for the number of particles that may be emitted.
		UINT Cycles // Set a value for how many times to play the burst.
		MinMaxRange Interval // Set a value for the time (in seconds) between when each cycle of the burst is triggered.
		float Probability // Controls how likely it is that each burst event spawns particles. A higher value makes the system produce more particles, and a value of 1 guarantees that the system produces particles.
	*/
	
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
	
	/* *** Velocity over Lifetime module ***
	* The Velocity over Lifetime module allows you to control the velocity of particles over their lifetime.
	*
	* To create particles that drift in a particular direction, use the Linear X, Y and Z curves.
	* 
	* To create effects with particles that spin around a center position, use the Orbital velocity values. Additionally, you can propel particles towards or away from a center position using the Radial velocity values. You can define a custom center of rotation for each particle by using the Offset value.
	* 
	* You can also use this module to adjust the speed of the particles in the Particle System
	without affecting their direction, by leaving all the above values at zero and only modifying the Speed Modifier value.
	
	MinMaxCurve Linear X, Y, Z // Linear velocity of particles in the X, Y and Z axes.
		XPARTICLE_SIMULATION_SPACE Space // Specifies whether the Linear X, Y, Z axes refer to local or world space.
	MinMaxCurve Orbital X, Y, Z // Orbital velocity of particles around the X, Y and Z axes.
	MinMaxCurve Offset X, Y, Z // The position of the center of orbit, for orbiting particles.
		MinMaxCurve Radial // Radial velocity of particles away from/towards the center position.
	MinMaxCurve Speed Modifier // Applies a multiplier to the speed of particles, along/around their current direction of travel.
	*/
	
	/* *** Limit Velocity Over Lifetime module ***
	* This module controls how the speed of particles is reduced over their lifetime.
	* 
	* This module is very useful for simulating air resistance that slows the particles, especially when a decreasing curve is used to lower the speed limit over time. For example, an explosion or firework initially bursts at great speed but the particles emitted from it rapidly slow down as they pass through the air.
	*
	* The Drag option offers a more physically accurate simulation of air resistance by offering options to apply varying amounts of resistance based on the size and speed of the particles.
	
	bool Separate Axes // Splits the axes up into separate X, Y and Z components.
		XPARTICLE_SIMULATION_SPACE Space // Selects whether the speed limitation refers to local or world space. This option is only available when Separate Axes is enabled.
	MinMaxCurve Speed // Sets the speed limit of the particles.
		MinMaxCurve Dampen // The fraction by which a particle’s speed is reduced when it exceeds the speed limit.
	MinMaxCurve Drag // Applies linear drag to the particle velocities.
		bool Multiply by Size // When enabled, larger particles are affected more by the drag coefficient.
		bool Multiply by Velocity // When enabled, faster particles are affected more by the drag coefficient.
	
	*/
	
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
