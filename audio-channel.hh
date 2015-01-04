
#pragma once

#include <list>

#include "audio-common.hh"

class AudioChannel {
private:

  std::list<audio_pattern_buffer_t*> m_pattern_queue;

  audio_pattern_buffer_t::iterator m_pattern_it;

public:

  AudioChannel();

  float next_value();

  bool is_busy();
  void start();

  void play( audio_pattern_buffer_t& );
};

