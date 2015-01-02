
#pragma once

#include <list>

#include "audio-tone.hh"

class AudioPattern {
private:

  std::list<AudioTone> m_tones;

  int m_sample_rate;

public:

  AudioPattern();

  void init(int);

  void beep( audio_waveform_t&, float, int, int );

  std::list<AudioTone>& tones();
};

