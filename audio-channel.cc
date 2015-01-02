
#include <iostream>
using namespace std;

#include "audio-channel.hh"

AudioChannel::AudioChannel() { }

float AudioChannel::next_value() {

  if( !m_cursor ) return 0;

  float value = m_cursor->next_value();

  if( m_cursor->is_at_end() ) {

    m_tone_queue.pop_front();
    if(!m_tone_queue.size()) {
      m_cursor.reset();
      return 0;
    }

    m_cursor = m_tone_queue.front().cursor_start();

    cout << "next tone" << endl;
  }

  return value;
}

bool AudioChannel::is_busy() {
  return (bool)m_cursor;
}

void AudioChannel::start() {
  m_cursor = m_tone_queue.front().cursor_start();
}

void AudioChannel::play( AudioPattern& ap ) {
  
  for( list<AudioTone>::iterator it = ap.tones().begin(); it != ap.tones().end(); it++ )
    m_tone_queue.push_back(*it);
}
