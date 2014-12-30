
#include <iostream>

#include <portaudio.h>

#include "audio.hh"

using namespace std; 

int main( int arch, char** argv ) {

  try {

    Audio audio;

    audio.init();

    audio.beep( 1.0, 1000, 500 );
    audio.beep( 0.9, 1100, 500 );
    audio.beep( 0.8, 1200, 500 );
    audio.beep( 0.7, 1300, 500 );
    audio.beep( 0.6, 1400, 500 );
    audio.beep( 0.5, 1500, 500 );
    audio.beep( 0.4, 1600, 500 );

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
