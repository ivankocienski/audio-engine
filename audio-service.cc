
#include <stdio.h>
#include <math.h>

#include "audio-exception.hh"
#include "audio-common.hh"
#include "audio-service.hh"

using namespace std;




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

  while( buffer_size ) {

    float accumulator = 0;

    for( vector<AudioChannel>::iterator it = m_channels.begin(); it != m_channels.end(); it++ ) 
      accumulator += it->next_value();

    *buffer = accumulator;

    buffer++;
    buffer_size--;
  }

  return 0;
}

float frand() {
  return (float)rand() / (float)RAND_MAX;
}

float sfrand() {
  return 1.0 - (2.0 * frand());
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

  m_waveforms.resize(WF_COUNT);

  
  const PaStreamInfo *stream_info = Pa_GetStreamInfo(m_stream);
  //if( !stream_info ) throw AudioException( "Pa_GetStreamInfo", 0 );
  int m_sample_rate = stream_info->sampleRate; 
  
  for( int i = 0; i < WF_COUNT; i++ )
    m_waveforms[i].resize(m_sample_rate);

  float a_inc = (M_PI * 2.0) / stream_info->sampleRate;
  float ang   = 0;

  // TODO: use of iterators here?
  for( int i = 0; i < m_sample_rate; i++ ) {
    m_waveforms[WF_SINE  ][i] = sin(ang); 
    m_waveforms[WF_SQUARE][i] = (i < (m_sample_rate / 2 )) ? -1 : 1;
    m_waveforms[WF_NOISE ][i] = sfrand();
    ang += a_inc;
  }

  m_channels.resize(1);

  cout << "m_sample_rate=" << m_sample_rate << endl;
}

void AudioService::start() {

  for( vector<AudioChannel>::iterator it = m_channels.begin(); it != m_channels.end(); it++ )
    it->start();

  PaError err = Pa_StartStream(m_stream);
  CATCH_PA_ERROR( "Pa_StartStream", err ); 
}

void AudioService::stop() {
  PaError err = Pa_StopStream(m_stream);
  CATCH_PA_ERROR( "Pa_StopStream", err );
}

bool AudioService::is_busy() {

  for( vector<AudioChannel>::iterator it = m_channels.begin(); it != m_channels.end(); it++ )
    if( it->is_busy() ) return true;

  return false;
}

/* void AudioService::beep( int ch, int wf, float vol, int pitch, int msec ) { 
 *   m_channels[ch].beep( m_waveforms[wf], vol, pitch, msec );
 * }
 */

AudioPattern& AudioService::pattern(int i) {
  m_patterns[i].init(m_sample_rate);
  return m_patterns[i];
}

void AudioService::play( int ch, int pn ) {
  m_channels[ch].play( m_patterns[pn] );
}

audio_waveform_t & AudioService::waveform( int w ) {
  return m_waveforms[w];
}
