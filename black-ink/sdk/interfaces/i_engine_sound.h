#pragma once
#include "../../utils/math/datatypes.h"
#include "../datatypes/c_utl_vector.h"
struct sound_info_t
{
	int			m_guid;
	void*		m_file_name_handle;
	int			m_sound_source;
	int			m_channel;
	int			m_speaker_entity;
	float		m_volume;
	float		m_last_spatialized_volume;
	float		m_radius;
	int			m_pitch;
	vec3_t* m_origin;
	vec3_t* m_direction;
	bool		m_update_positions;
	bool		m_is_sentence;
	bool		m_dry_mix;
	bool		m_speaker;
	bool		m_is_from_server;
};
class IRecipientFilter;

enum soundlevel_t
{
	SNDLVL_NONE = 0,
	SNDLVL_20dB = 20,		// rustling leaves
	SNDLVL_25dB = 25,		// whispering
	SNDLVL_30dB = 30,		// library
	SNDLVL_35dB = 35,
	SNDLVL_40dB = 40,
	SNDLVL_45dB = 45,		// refrigerator
	SNDLVL_50dB = 50,	    // 3.9    // average home
	SNDLVL_55dB = 55,	    // 3.0
	SNDLVL_IDLE = 60,	    // 2.0	
	SNDLVL_60dB = 60,	    // 2.0	// normal conversation, clothes dryer
	SNDLVL_65dB = 65,	    // 1.5	// washing machine, dishwasher
	SNDLVL_STATIC = 66,	    // 1.25
	SNDLVL_70dB = 70,	    // 1.0	// car, vacuum cleaner, mixer, electric sewing machine
	SNDLVL_NORM = 75,
	SNDLVL_75dB = 75,	    // 0.8	// busy traffic
	SNDLVL_80dB = 80,	    // 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
	SNDLVL_TALKING = 80,    // 0.7
	SNDLVL_85dB = 85,	    // 0.6	// average factory, electric shaver
	SNDLVL_90dB = 90,	    // 0.5	// screaming child, passing motorcycle, convertible ride on frw
	SNDLVL_95dB = 95,
	SNDLVL_100dB = 100,	    // 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	SNDLVL_105dB = 105,     // helicopter, power mower
	SNDLVL_110dB = 110,     // snowmobile drvrs seat, inboard motorboat, sandblasting
	SNDLVL_120dB = 120,     // auto horn, propeller aircraft
	SNDLVL_130dB = 130,     // air raid siren
	SNDLVL_GUNFIRE = 140,	// 0.27	// THRESHOLD OF PAIN, gunshot, jet engine
	SNDLVL_140dB = 140,	    // 0.2
	SNDLVL_150dB = 150,	    // 0.2
	SNDLVL_180dB = 180,		// rocket launching
};

//-----------------------------------------------------------------------------
// common pitch values
//-----------------------------------------------------------------------------
#define	PITCH_NORM		100			  // non-pitch shifted
#define PITCH_LOW		95			    // other values are possible - 0-255, where 255 is very high
#define PITCH_HIGH		120

class i_engine_sound
{
public:
	// Precache a particular sample
	virtual bool precache_sound( const char* sample, bool pre_load = false, bool is_ui_sound = false ) = 0;
	virtual bool is_sound_precached( const char* sample ) = 0;
	virtual void prefetch_sound( const char* sample ) = 0;
	virtual bool is_looping_sound( const char* sample ) = 0;

	// Just loads the file header and checks for duration (not hooked up for .mp3's yet)
	// Is accessible to server and client though
	virtual float get_sound_duration( const char* sample ) = 0;

	// Pitch of 100 is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
	// down to 1 is a lower pitch.   150 to 70 is the realistic range.
	// EmitSound with pitch != 100 should be used sparingly, as it's not quite as
	// fast (the pitchshift mixer is not native coded).

	// NOTE: setting iEntIndex to -1 will cause the sound to be emitted from the local
	// player (client-side only)
	virtual int emit_sound( IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3_t* pOrigin = 0, const vec3_t* pDirection = 0, c_utl_vector< vec3_t >* pUtlVecOrigins = 0, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 ) = 0;

	virtual int EmitSound( IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3_t* pOrigin = 0, const vec3_t* pDirection = 0, c_utl_vector< vec3_t >* pUtlVecOrigins = 0, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 ) = 0;

	virtual void EmitSentenceByIndex( IRecipientFilter& filter, int iEntIndex, int iChannel, int iSentenceIndex,
		float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3_t* pOrigin = 0, const vec3_t* pDirection = 0, c_utl_vector< vec3_t >* pUtlVecOrigins = 0, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 ) = 0;

	virtual void    stop_sound( int iEntIndex, int iChannel, const char* pSample, unsigned int nSoundEntryHash ) = 0;
	virtual void    stop_all_sounds( bool bClearBuffers ) = 0;
	virtual void    set_room_type( IRecipientFilter& filter, int roomType ) = 0;
	virtual void    set_player_dsp( IRecipientFilter& filter, int dspType, bool fastReset ) = 0;
	virtual int     emit_ambient_sound( const char* pSample, float flVolume, int iPitch = PITCH_NORM, int flags = 0, float soundtime = 0.0f ) = 0;
	virtual float   get_dist_gain_from_sound_level( soundlevel_t soundlevel, float dist ) = 0;
	virtual int		get_guid_for_last_sound_emited( ) = 0;
	virtual bool	is_sound_still_played( int guid ) = 0;
	virtual void	stop_sound_by_guid( int guid, bool bForceSync ) = 0;
	virtual void	get_valume_by_guid( int guid, float fvol ) = 0;
	virtual void    unk( ) = 0;
	virtual void	get_active_sounds( c_utl_vector<sound_info_t>& sndlist ) = 0;
	virtual void	precache_sentence_group( const char* pGroupName ) = 0;
	virtual void	notify_begin_movie_playback( ) = 0;
	virtual void	notify_end_movie_playback( ) = 0;
	virtual bool	get_sound_channel_volume( const char* sound, float& flVolumeLeft, float& flVolumeRight ) = 0;
	virtual float	get_elapsed_time_by_guid( int guid ) = 0;
};
