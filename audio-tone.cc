
#include "audio-tone.hh"

using namespace std;

Tone::Tone() : m_waveform( vector<float>() ) {
  m_volume      = 0;
  m_pitch       = 0;
  m_duration    = 0;
  m_sample_rate = 0;
}


Tone::Tone( const vector<float> &wf, int sr, float v, int p, int d ) : m_waveform(wf) {

  m_sample_rate = sr;

  m_volume      = v;
  m_pitch       = p;

  float m = (float)d / 1000.0;


  m_duration = m * (float)m_sample_rate;

  int taper_len = m * 20.0; // 10ms

  m_start_taper = taper_len;
  m_stop_taper  = m_duration - taper_len;
  m_taper_value = 0;
  m_taper_inc   = 1.0 / (float)taper_len;

  cout << "m_taper_inc=" << m_taper_inc << endl;
  cout << "m_start_taper=" << m_start_taper << endl;
  cout << "m_stop_taper=" << m_stop_taper << endl;

}

float Tone::value_at( int tone_pos, int wave_pos ) const {

  int rp = wave_pos % m_waveform.size();

  float v = m_waveform[ rp ] * m_volume;

  if( tone_pos < m_start_taper ) { 
    float vv = (float)tone_pos * (float)m_taper_inc; 
    v *= vv;
    //cout << "start " << vv << endl;
  }

  if( tone_pos >= m_stop_taper ) {
    float vv = (float)(m_duration - tone_pos) * (float)m_taper_inc; 
    v *= vv;
   // cout << "stop" << vv << endl;
  }

  return v;
}

int Tone::duration() const {
  return m_duration;
}

int Tone::pos_inc() const {
  return m_pitch;
}

boost::shared_ptr<ToneCursor> Tone::cursor_start() {
  return boost::shared_ptr<ToneCursor>( new ToneCursor( *this ) );
}

// Tone Cursor - the 'playback head' for tones.

ToneCursor::ToneCursor() : m_tone(Tone()) {
  m_position = 0;
  m_wave_pos = 0;
}

ToneCursor::ToneCursor( const Tone &tone ) : m_tone(tone) { 
  m_position = 0;
  m_wave_pos = 0;
}

float ToneCursor::next_value() {

  float v = m_tone.value_at( m_position, m_wave_pos ); 
  m_position++;
  m_wave_pos += m_tone.pos_inc();
  return v;
}

bool ToneCursor::is_at_end() {
  return m_position >= m_tone.duration();
}
