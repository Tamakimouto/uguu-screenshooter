/**
 * uguu-screenshooter
 * Copyright © 2015 Anthony Zheng <Tamakimouto@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "global.h"
#include <X11/extensions/shape.h>
#include <gdk/gdkx.h>
#include <unistd.h>
#include <X11/Xlib.h>

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
