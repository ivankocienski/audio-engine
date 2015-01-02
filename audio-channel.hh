
#pragma once

#include <list>

#include "audio-common.hh"
#include "audio-tone.hh"

class AudioChannel {
private:

  std::list<AudioTone> m_tone_queue;
  std::list<AudioTone>::iterator m_tone_iterator;

  boost::shared_ptr<ToneCursor> m_cursor;

  int m_sample_rate;  

public:

  AudioChannel();

  void init(int);

  float next_value();

  bool is_busy();
  void start();

  void beep( audio_waveform_t&, float, int, int );
};

