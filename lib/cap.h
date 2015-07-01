#include "global.h"
#include <X11/extensions/shape.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <glib.h>
#include <unistd.h>
#include <x11/xlib.h>

GdkPixbuf *
takeScreenshot();

static GdkPixbuf *
get_window_screenshot (GdkWindow * window,
		       gboolean border);

static Window
find_wm_window (Window xid);

static GdkWindow *
get_active_window (GdkScreen * screen,
		   gboolean * needs_unref,
		   gboolean * border);
