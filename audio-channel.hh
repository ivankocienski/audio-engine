
#pragma once

#include <list>

#include "audio-common.hh"
#include "audio-tone.hh"
#include "audio-pattern.hh"

class AudioChannel {
private:
  
  std::list<AudioTone> m_tone_queue;

  boost::shared_ptr<ToneCursor> m_cursor;

public:

  AudioChannel();

  float next_value();

  bool is_busy();
  void start();

  //void beep( audio_waveform_t&, float, int, int );

  void play( AudioPattern& );
};

