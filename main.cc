
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>

#include <portaudio.h>

using namespace std;

/* class PortAudio {
 * private:
 * 
 * 
 * public:
 * 
 *   PortAudio() {
 * 
 *   }
 * };
 */

static const int c_sample_rate  = 44100;
static const int c_num_channels = 1;
static const int c_data_type    = paFloat32;

static vector<float>sine_wave;

int g_channel_sample_rate = 0;

int wave_pos = 0;
int wave_add = 1000;
int wave_len;

class Tone {
private:

  float m_volume;
  int m_pitch;
  int m_duration; // milliseconds

  int m_position;
public:

  Tone( float v, int p, int d ) {
    m_volume   = v;
    m_pitch    = p;
    //m_duration = (1.0 / m_channel_sample_rate) * d;
    m_duration = (1.0 / (float)c_sample_rate) * (float)d;

    m_position = 0;
  }

  float value() {

    float v;

    if( is_done() ) return 0;

    v = sine_wave[ m_position ] * m_volume;

    m_position += m_pitch;
    if( m_position >= wave_len ) m_position -= wave_len ;

    return v;
  }

  bool is_done() {
    return m_position >= m_duration;
  }
};

Tone tone( 1, 500, 1000 );

extern "C" {
static int callback( const void* in_buff, void* out_buff, unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* timing_info, const PaStreamCallbackFlags flags, void *data ) {

  float *buffer = (float*)out_buff;

  for( unsigned int i = 0; i < frames_per_buffer; i++) {
    
    *buffer = tone.value();

    buffer++;
  }

  return 0;
}
}


int main( int arch, char** argv ) {


  PaError err;
  PaStream *stream;
  const PaStreamInfo *stream_info;


  try {
    err = Pa_Initialize();
    if( err != paNoError ) throw "initializing port audio";

    err = Pa_OpenDefaultStream(
        &stream,
        0,
        c_num_channels,
        c_data_type,
        c_sample_rate,
        paFramesPerBufferUnspecified,
        callback,
        NULL
        );

    if( err != paNoError ) throw "opening stream";

    stream_info = Pa_GetStreamInfo(stream);
    if( !stream_info ) throw "getting stream info";
    
    g_channel_sample_rate = stream_info->sampleRate;

    cout << "sample rate " << stream_info->sampleRate << endl;

    sine_wave.resize(g_channel_sample_rate);
    float a_inc = (M_PI * 2.0) / stream_info->sampleRate;
    float ang   = 0;

    for( int i = 0; i < (int)stream_info->sampleRate; i++ ) {
      sine_wave[i] = 0.5 + 0.5 * sin(ang); 
      ang += a_inc;
    }

    wave_len = sine_wave.size(); 

    err = Pa_StartStream(stream);
    if( err != paNoError ) throw "starting stream"; 

    while( true ) {
      Pa_Sleep( 50 );

      if( tone.is_done() ) break;
    }

    err = Pa_StopStream(stream);
    if( err != paNoError ) throw "stopping stream";

  } catch( const char * msg ) {

    cerr << "error: (" << msg << ") " << Pa_GetErrorText(err) << endl;
  }

  err = Pa_Terminate();
  if( err != paNoError ) {
    cerr << "Pa_Terminate failed: " << Pa_GetErrorText(err) << endl;
  }

  return 0;

}
