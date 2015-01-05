
#pragma once

#include <list>
//#include <mutex>

#include "audio-common.hh"

class AudioChannel {
private:

//  std::mutex m_lock;
  
  std::list<audio_pattern_buffer_t*> m_pattern_queue;

  audio_pattern_buffer_t::iterator m_pattern_it;

  int m_queue_size;
  bool m_loop;

public:

  AudioChannel();

  bool is_busy();
  void start();

  void play( audio_pattern_buffer_t& );
  void loop( audio_pattern_buffer_t& );
  
  bool is_looping();
  void stop_looping();

  void render_to( float*, int, float );
};

