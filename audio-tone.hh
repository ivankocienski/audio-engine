
#include <vector>
#include <boost/shared_ptr.hpp>

//#include "audio.hh"

class ToneCursor;

class Tone {
private:

  const std::vector<float> &m_waveform;
  int m_sample_rate;

  float m_volume;
  int m_pitch;
  int m_duration; // milliseconds

  int m_start_taper;
  int m_stop_taper;
  float m_taper_value;
  float m_taper_inc;

public:

  Tone();
  Tone( const std::vector<float>&, int, float, int, int );

  float value_at( int, int) const;
  int duration() const;
  int pos_inc() const;

  boost::shared_ptr<ToneCursor> cursor_start();
};

class ToneCursor {
private:

  const Tone &m_tone;
  int m_position;
  int m_wave_pos;

public:

  ToneCursor();
  ToneCursor( const Tone& );

  float next_value();
  bool is_at_end();
};

