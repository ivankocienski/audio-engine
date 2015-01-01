
#pragma once

#include <portaudio.h>

#define CATCH_PA_ERROR( func, err ) if( (err) != paNoError ) throw AudioException( (func), (err) );
