
using namespace std;

#include "audio-pattern.hh"

AudioPattern::AudioPattern() {}

void AudioPattern::init( int sr ) {
  m_sample_rate = sr;
}

void AudioPattern::beep( audio_waveform_t &wf, float vol, int pitch, int msec ) {
  
  m_tones.push_back( AudioTone( &wf, m_sample_rate, vol, pitch, msec ));
}

std::list<AudioTone>& AudioPattern::tones() {

  return m_tones;
}
