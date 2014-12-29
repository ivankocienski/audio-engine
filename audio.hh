
#pragma once

#include <portaudio.h>

class Audio {
private:

  PaStream *m_stream;



public:

  static const int c_sample_rate;
  static const int c_num_channels;
  static const int c_data_type;

  Audio();
  ~Audio();

  void init(); 
  void start(); 
  void stop();

  friend int audio_callback( const void*, void*, unsigned long, PaStreamCallbackTimeInfo*, const PaStreamCallbackFlags, void*);

  int callback( float*, unsigned long);
};

