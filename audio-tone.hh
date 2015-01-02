
#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

//#include "audio.hh"
#include "audio-common.hh"

class ToneCursor;

class AudioTone {
private:

  const audio_waveform_t *m_waveform;
  int m_sample_rate;

  float m_volume;
  int m_pitch;
  int m_duration; // milliseconds

  int m_start_taper;
  int m_stop_taper;
  float m_taper_value;
  float m_taper_inc;

public:

  AudioTone();
  AudioTone( const audio_waveform_t *, int, float, int, int );

  float value_at( int, int) const;
  int duration() const;
  int pos_inc() const;

  boost::shared_ptr<ToneCursor> cursor_start();
};

class ToneCursor {
private:

  const AudioTone &m_tone;
  int m_position;
  int m_wave_pos;

public:

  ToneCursor();
  ToneCursor( const AudioTone& );

  float next_value();
  bool is_at_end();
};

