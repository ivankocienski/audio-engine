
#include "audio-channel.hh"

AudioChannel::AudioChannel() { }

float AudioChannel::next_value() {

  if( !is_busy() ) return 0;

  float value = *m_pattern_it;

  m_pattern_it++;

  if( m_pattern_it == m_pattern_queue.front()->end() ) {
    m_pattern_queue.pop_front();
    start();
  }

  return value;
}

bool AudioChannel::is_busy() {
  return m_pattern_queue.size() > 0;
}

void AudioChannel::start() {
  if( is_busy() )
    m_pattern_it = m_pattern_queue.front()->begin();
}

void AudioChannel::play( audio_pattern_buffer_t& pb ) { 
  m_pattern_queue.push_back( &pb );
}
