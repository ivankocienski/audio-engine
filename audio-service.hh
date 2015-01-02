
#pragma once

#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <portaudio.h>

#include "audio-channel.hh"
#include "audio-pattern.hh"

//#include "audio.hh"

class AudioService {
private:

  PaStream *m_stream;

  std::vector<AudioChannel> m_channels;

  std::vector<audio_waveform_t> m_waveforms;

  std::vector<AudioPattern> m_patterns;

  int m_sample_rate;

public:

  enum { // waveform
    WF_SINE,
    WF_SQUARE,
    WF_NOISE,
    WF_COUNT
  };

  static const int c_sample_rate;
  static const int c_num_channels;
  static const int c_data_type;

  AudioService();
  ~AudioService();

  void init(); 
  void start(); 
  void stop();

  bool is_busy();

  audio_waveform_t & waveform( int );
  
  //void beep( int, int, float, int, int );
  AudioPattern& pattern(int);
  void play(int, int);

  friend int audio_callback( const void*, void*, unsigned long, PaStreamCallbackTimeInfo*, const PaStreamCallbackFlags, void*);

  int callback( float*, unsigned long);
};

