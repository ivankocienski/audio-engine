
#include <iostream>


#include "audio.hh"

using namespace std; 

int main( int arch, char** argv ) {

  try {

    AudioService audio;

    audio.init();

    
    {
      AudioPattern &ap( audio.pattern(0) );

      // channel, wave, volume, pitch, ms

      ap.beep( audio.waveform(AudioService::WF_SINE), 1.0, 1000, 50 );
      ap.beep( audio.waveform(AudioService::WF_SINE), 0.9, 1100, 50 );
      ap.beep( audio.waveform(AudioService::WF_SINE), 0.8, 1200, 50 );
      ap.beep( audio.waveform(AudioService::WF_SINE), 0.7, 1300, 50 );
      ap.beep( audio.waveform(AudioService::WF_SINE), 0.6, 1400, 50 );
      ap.beep( audio.waveform(AudioService::WF_SINE), 0.5, 1500, 50 );
      ap.beep( audio.waveform(AudioService::WF_SINE), 0.4, 1600, 50 );
    }

    // channel, pattern
    audio.play( 0, 0 );
    audio.play( 0, 0 );
    audio.play( 0, 0 );
    audio.play( 0, 0 );
    audio.play( 0, 0 );

    audio.start();

    while( true ) {

      Pa_Sleep( 50 );

      if( !audio.is_busy() ) break;
    }

  } catch( const AudioException &x ) {

    cerr << "error: (" << x.function() << ") " << x.message() << endl;
  }

  return 0;

}
