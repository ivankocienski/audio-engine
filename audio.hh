
#pragma once

#include <exception>
#include <string>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <portaudio.h>

class AudioException : public std::exception {
private:

  std::string m_function;
  PaError m_pa_error;

public:

  AudioException( const char*, PaError );
  ~AudioException() throw() {}

  const std::string& function() const;

  std::string message() const;
};

class ToneCursor;

class Tone {
private:

  const std::vector<float> &m_waveform;
  int m_sample_rate;

  float m_volume;
  int m_pitch;
  int m_duration; // milliseconds

public:

  Tone();
  Tone( const std::vector<float>&, int, float, int, int );

  float value_at(int) const;
  int duration() const;
  int pos_inc() const;

  boost::shared_ptr<ToneCursor> cursor_start();
};

class ToneCursor {
private:

  const Tone &m_tone;
  int m_position;
  int m_wave_pos;

public:

  ToneCursor();
  ToneCursor( const Tone& );

  float next_value();
  bool is_at_end();
};


class Audio {
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

  Audio();
  ~Audio();

  void init(); 
  void start(); 
  void stop();

  bool is_busy();

  void beep( float, int, int );

  friend int audio_callback( const void*, void*, unsigned long, PaStreamCallbackTimeInfo*, const PaStreamCallbackFlags, void*);

  int callback( float*, unsigned long);
};

