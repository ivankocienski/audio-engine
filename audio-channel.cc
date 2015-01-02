
#include <iostream>
using namespace std;

#include "audio-channel.hh"

AudioChannel::AudioChannel() {
  m_sample_rate = 0;
}

void AudioChannel::init(int sr) {
  m_sample_rate = sr; 
}

float AudioChannel::next_value() {

  if( m_tone_iterator == m_tone_queue.end() ) return 0;

  float value = m_cursor->next_value();

  if( m_cursor->is_at_end() ) {

    m_tone_iterator++;

    if( m_tone_iterator == m_tone_queue.end() ) return 0;

    m_cursor = m_tone_iterator->cursor_start();

    cout << "next tone" << endl;
  }

  return value;
}

bool AudioChannel::is_busy() {
  return m_tone_iterator != m_tone_queue.end();
}

void AudioChannel::beep( audio_waveform_t &wf, float vol, int pitch, int msec ) {
  m_tone_queue.push_back( AudioTone( &wf, m_sample_rate, vol, pitch, msec ));
}

void AudioChannel::start() {
  m_tone_iterator = m_tone_queue.begin();
  m_cursor = m_tone_iterator->cursor_start();
}
