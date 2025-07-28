#pragma once
#include <slint_window.h>

struct wl_display;
struct wl_surface;

class LayerShell {
  public:
    LayerShell(slint::Window* window);
}
