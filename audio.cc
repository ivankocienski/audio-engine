
#include <stdio.h>

#include "audio.hh"

const int Audio::c_sample_rate  = 44100;
const int Audio::c_num_channels = 1;
const int Audio::c_data_type    = paFloat32;

int audio_callback( const void* in_b, void* out_b, unsigned long len, const PaStreamCallbackTimeInfo* info, const PaStreamCallbackFlags flags, void *data ) {

  Audio *audio = (Audio*)data;
  float *buffer = (float*)out_b;

  return audio->callback( buffer, len ); 
}


Audio::Audio() {
  m_stream = NULL;
}

Audio::~Audio() {

  PaError err;

  err = Pa_Terminate();
  if( err != paNoError ) throw "Pa_Terminate failed";
}

void Audio::init() {

  PaError err;

  err = Pa_Initialize();
  if( err != paNoError ) throw "initializing port audio";

  err = Pa_OpenDefaultStream(
      &m_stream,
      0,
      c_num_channels,
      c_data_type,
      c_sample_rate,
      paFramesPerBufferUnspecified,
     audio_callback,
      this
      );

  if( err != paNoError ) throw "opening stream";
}

void Audio::start() {

  PaError err;

  err = Pa_StartStream(m_stream);
  if( err != paNoError ) throw "starting stream"; 
}

void Audio::stop() {

  PaError err;

  err = Pa_StopStream(m_stream);
  if( err != paNoError ) throw "stopping stream";
}

