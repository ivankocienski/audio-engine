
#include <iostream>


#include "audio.hh"
#include "window.hh"

using namespace std; 

static int selected = 0;
static unsigned char anim = 0;

static bool should_paint( int o ) {
  if( selected != o ) return true;

  return (anim >> 3) & 1;
}

int main( int arch, char** argv ) {

  try {

    AudioService audio;
    audio.init();

    Window window;
    window.open( 500, 400, "Port Audio C++ demo" );
    
    { // menu diddle
      AudioPattern ap;

      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2000, 30 );

      audio.set_pattern( 0, ap );
    }

    { // start sound
      AudioPattern ap;

      // channel, wave, volume, pitch, ms

      //BEEP( ap, WF_SINE, 1.0, 1000, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1000, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.9, 1100, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.8, 1200, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.7, 1300, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.6, 1400, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.5, 1500, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.4, 1600, 50 );

      audio.set_pattern( 1, ap );
    }


    { // echo things
      AudioPattern ap;

      // channel, wave, volume, pitch, ms

      //BEEP( ap, WF_SINE, 1.0, 1000, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.0, 1000, 100 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.8, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.8, 2000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.8, 1500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.8, 1000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.0, 1000, 100 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.6, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.6, 2000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.6, 1500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.6, 1000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.0, 1000, 100 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.3, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.3, 2000, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.3, 1500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 0.3, 1000, 30 );

      audio.set_pattern( 2, ap );
    }

    {
      // other thingy
      AudioPattern ap;

      // channel, wave, volume, pitch, ms

      //BEEP( ap, WF_SINE, 1.0, 1000, 50 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1500, 40 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1500, 40 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1500, 40 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1500, 40 );

      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1500, 40 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 2500, 30 );
      ap.beep( audio.oscillator(AudioService::WF_SINE), 1.0, 1500, 40 );

      audio.set_pattern( 3, ap );
    }


    // channel, pattern
    audio.play( 0, 1 );

    audio.start();

    while( window.active() ) {

      window.puts( 10, 10, "Port Audio Demo" );

      if( should_paint(0) ) window.puts( 20, 30, "Sound 1" );
      if( should_paint(1) ) window.puts( 20, 50, "Sound 2" );
      if( should_paint(2) ) window.puts( 20, 70, "Sound 3" );
      if( should_paint(3) ) window.puts( 20, 90, "Play Loop" );

      window.puts( 10, 150, "Press ESCAPE to quit" );

      switch( window.inkey() ) {
        case Window::K_UP:
          selected--; if( selected < 0 ) selected = 3;
          audio.play( 0, 0 );
          break;
          
        case Window::K_DOWN:
          selected++; if( selected > 3 ) selected = 0;
          audio.play( 0, 0 );
          break;

        case Window::K_ESCAPE:
          window.stop();
          break;

        case Window::K_ENTER:

          switch( selected ) {
            case 0: audio.play(0, 1); break;
            case 1: audio.play(0, 2); break;
            case 2: audio.play(0, 3); break;

            case 3: 
              if( audio.channel(1).is_looping() ) 
                audio.channel(1).stop_looping();
              else
                audio.loop(1, 1);
              break;
          }

          break;
      }


      window.tick();

      anim++;
    }

  } catch( const AudioException &x ) {

    cerr << "error: (" << x.function() << ") " << x.message() << endl;
  }

  return 0;

}
