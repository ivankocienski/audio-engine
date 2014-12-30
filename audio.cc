
#include <stdio.h>
#include <math.h>

#include "audio.hh"

using namespace std;

#define CATCH_PA_ERROR( func, err ) if( (err) != paNoError ) throw AudioException( (func), (err) );

/*
 * audio exception class
 *
 */

AudioException::AudioException( const char* f, PaError e ) : exception() {
  m_function = f;
  m_pa_error = e;
}

//AudioException::~AudioException() {}

const std::string& AudioException::function() const {
  return m_function;
}

std::string AudioException::message() const {
  return string(Pa_GetErrorText(m_pa_error));
}


/* 
 * Tone class
 *
 */

Tone::Tone() : m_waveform( vector<float>() ) {
  m_volume      = 0;
  m_pitch       = 0;
  m_duration    = 0;
  m_sample_rate = 0;
}


Tone::Tone( const vector<float> &wf, int sr, float v, int p, int d ) : m_waveform(wf) {

  m_sample_rate = sr;

  m_volume      = v;
  m_pitch       = p;
  m_duration = (float)d / 1000.0 * (float)m_sample_rate;

  cout << "Tone( t=" << m_pitch << "  dur=" << m_duration << ")" << endl;
}

float Tone::value_at( int pos ) const {
  int rp = pos % m_waveform.size();
  return m_waveform[ rp ] * m_volume;
}

int Tone::duration() const {
  return m_duration;
}

int Tone::pos_inc() const {
  return m_pitch;
}

boost::shared_ptr<ToneCursor> Tone::cursor_start() {
  return boost::shared_ptr<ToneCursor>( new ToneCursor( *this ) );
}

// Tone Cursor - the 'playback head' for tones.

ToneCursor::ToneCursor() : m_tone(Tone()) {
  m_position = 0;
  m_wave_pos = 0;
}

ToneCursor::ToneCursor( const Tone &tone ) : m_tone(tone) { 
  m_position = 0;
  m_wave_pos = 0;
}

float ToneCursor::next_value() {

  float v = m_tone.value_at( m_wave_pos ); 
  m_position++;
  m_wave_pos += m_tone.pos_inc();
  return v;
}

bool ToneCursor::is_at_end() {
  return m_position >= m_tone.duration();
}




/*
 * Audio class
 *
 */

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
  CATCH_PA_ERROR( "Pa_Terminate", err );
}

int Audio::callback( float* buffer, unsigned long buffer_size ) {

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

void Audio::init() {

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

void Audio::start() {

  m_tone_iterator = m_tone_queue.begin();
  m_cursor = m_tone_iterator->cursor_start();

  PaError err = Pa_StartStream(m_stream);
  CATCH_PA_ERROR( "Pa_StartStream", err ); 
}

void Audio::stop() {
  PaError err = Pa_StopStream(m_stream);
  CATCH_PA_ERROR( "Pa_StopStream", err );
}

bool Audio::is_busy() {

  return m_tone_iterator != m_tone_queue.end();
}

void Audio::beep( float vol, int pitch, int msec ) { 
  m_tone_queue.push_back( Tone( m_sine_wave, m_sample_rate, vol, pitch, msec )); 
}

