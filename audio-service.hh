
#pragma once

#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <portaudio.h>

#include "audio-tone.hh"

//#include "audio.hh"

class AudioService {
private:

  PaStream *m_stream;

  std::vector<float> m_sine_wave;

  std::list<Tone> m_tone_queue;
  std::list<Tone>::iterator m_tone_iterator;

  boost::shared_ptr<ToneCursor> m_cursor;

  int m_sample_rate;
  

public:

  static const int c_sample_rate;
  static const int c_num_channels;
  static const int c_data_type;

  AudioService();
  ~AudioService();

  void init(); 
  void start(); 
  void stop();

  bool is_busy();

  void beep( float, int, int );

  friend int audio_callback( const void*, void*, unsigned long, PaStreamCallbackTimeInfo*, const PaStreamCallbackFlags, void*);

  int callback( float*, unsigned long);
};

