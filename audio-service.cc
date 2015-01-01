
#include <stdio.h>
#include <math.h>

#include "audio-exception.hh"
#include "audio-common.hh"
#include "audio-service.hh"

using namespace std;




/*
 * AudioService class
 *
 */

const int AudioService::c_sample_rate  = 44100;
const int AudioService::c_num_channels = 1;
const int AudioService::c_data_type    = paFloat32;

int audio_callback( const void* in_b, void* out_b, unsigned long len, const PaStreamCallbackTimeInfo* info, const PaStreamCallbackFlags flags, void *data ) {

  AudioService *audio = (AudioService*)data;
  float *buffer = (float*)out_b;

  return audio->callback( buffer, len ); 
} 

AudioService::AudioService() {
  m_stream = NULL;
}

AudioService::~AudioService() {

  PaError err;

  err = Pa_Terminate();
  CATCH_PA_ERROR( "Pa_Terminate", err );
}

int AudioService::callback( float* buffer, unsigned long buffer_size ) {

  if( m_tone_iterator == m_tone_queue.end() ) return 0;

  for( int i = 0; i < buffer_size; i++ ) {

    *buffer = m_cursor->next_value();

    if( m_cursor->is_at_end() ) {

      m_tone_iterator++;

      if( m_tone_iterator == m_tone_queue.end() ) return 0;

      m_cursor = m_tone_iterator->cursor_start();

      cout << "next tone" << endl;
    }

    buffer++;
  }

  return 0;
}

void AudioService::init() {

  PaError err;

  err = Pa_Initialize();
  CATCH_PA_ERROR( "Pa_Initialize", err );

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

  CATCH_PA_ERROR( "Pa_OpenDefaultStream", err ); 

  const PaStreamInfo *stream_info = Pa_GetStreamInfo(m_stream);
  //if( !stream_info ) throw AudioException( "Pa_GetStreamInfo", 0 );
  m_sample_rate = stream_info->sampleRate; 
  
  m_sine_wave.resize(m_sample_rate);
  float a_inc = (M_PI * 2.0) / stream_info->sampleRate;
  float ang   = 0;

  for( int i = 0; i < m_sample_rate; i++ ) {
    m_sine_wave[i] = sin(ang); 
    ang += a_inc;
  }

  cout << "sample_rate=" << m_sample_rate << endl;
}

void AudioService::start() {

  m_tone_iterator = m_tone_queue.begin();
  m_cursor = m_tone_iterator->cursor_start();

  PaError err = Pa_StartStream(m_stream);
  CATCH_PA_ERROR( "Pa_StartStream", err ); 
}

void AudioService::stop() {
  PaError err = Pa_StopStream(m_stream);
  CATCH_PA_ERROR( "Pa_StopStream", err );
}

bool AudioService::is_busy() {

  return m_tone_iterator != m_tone_queue.end();
}

void AudioService::beep( float vol, int pitch, int msec ) { 
  m_tone_queue.push_back( Tone( m_sine_wave, m_sample_rate, vol, pitch, msec )); 
}

