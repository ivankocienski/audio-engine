
#pragma once

#include <SDL/SDL.h>
#include <vector>
#include <list>

class Window {
public:

  enum {
    K_NULL,
    K_UP,
    K_DOWN,
    K_RIGHT,
    K_LEFT,
    K_ESCAPE,
    K_SPACE,
    K_ENTER,
    K_TAB,
    K_F1,
    K_F2,
    K_F3,
    K_F4,
    K_F5,
    K_F6,
    K_F7,
    K_F8,
    K_F9,
    K_F10,
    K_F11,
    K_F12,
    K_MAX
  };

private:

  static const int MAX_KEY_HISTORY = 32;

  SDL_Surface *m_screen;
  SDL_Surface *m_font;

  bool m_active;

  int m_mouse_x;
  int m_mouse_y;

  void push_key(int);

public:

  std::list<int>m_key_buffer;
  
  bool m_keys[K_MAX];

  Window();
  ~Window();

  bool open( int, int, const char * );
  void set_title( const char * );
  
  void puts( int, int, const char* );

  void clear();
  int width();
  int height();

  void apply_film( std::vector<unsigned char>& );

  bool active();
  void tick(); 
  void stop();

  int mouse_x();
  int mouse_y();

  void flush_keys();
  int inkey();

};

