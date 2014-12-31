
#include <iostream>

#include <portaudio.h>

#include "audio.hh"

using namespace std; 

int main( int arch, char** argv ) {

  try {

    Audio audio;

    audio.init();

    for( int i = 0; i < 10; i++ ) {
      audio.beep( 1.0, 1000, 50 );
      audio.beep( 0.9, 1100, 50 );
      audio.beep( 0.8, 1200, 50 );
      audio.beep( 0.7, 1300, 50 );
      audio.beep( 0.6, 1400, 50 );
      audio.beep( 0.5, 1500, 50 );
      audio.beep( 0.4, 1600, 50 );
    }

    //audio.beep( 1.0, 1000, 4000 );

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
