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

#include "cap.h"

/**
 * Function: get_active_window
 *
 * The default (only) function run to grab the active window
 * which defaults to the entire desktop if there is no active
 * window.
 *
 * Code adapted from xfce4-screenshooter
 * Copyright © 2008-2010 Jérôme Guelfucci <jeromeg@xfce.org>
 *
 * @param screen The screen on which to take the screenshot.
 * @param needs_unref Whether or not to free
 * @param border True on window
 * @return window The window to be captured
 */
static GdkWindow *
get_active_window (GdkScreen * screen,
                   gboolean * needs_unref,
                   gboolean * border) {

    GdkWindow *window, *window2;
    window = gdk_screen_get_active_window (screen);

    if (G_UNLIKELY (window == NULL)) {
        window = gdk_get_default_root_window ();
        *needs_unref = FALSE;
        *border = FALSE;
    } else if (G_UNLIKELY (GDK_WINDOW_DESTROYED (window))) {
        g_object_unref (window);
        window = gdk_get_default_root_window ();
        *needs_unref = FALSE;
        *border = FALSE;
    } else if (gdk_window_get_type_hint (window) == GDK_WINDOW_TYPE_HINT_DESKTOP) {
        /* Grabs entire Desktop if active window is Desktop */
        g_object_unref (window);
        window = gdk_get_default_root_window ();
        *needs_unref = FALSE;
        *border = FALSE;
    } else {
        window2 = gdk_window_get_toplevel (window);
        g_object_unref (window);
        window = window2;
        *border = TRUE;
    }

    return window;

}//get active window


/**
 * Function: find_wm_window
 *
 * Only runs under certain boolean conditions.
 *
 * Code adapted from xfce4-screenshooter
 * Copyright © 2008-2010 Jérôme Guelfucci <jeromeg@xfce.org>
 *
 * @param xid
 */
static Window
find_wm_window (Window xid) {

    Window root, parent, *children;
    unsigned int nchildren;

    do {
        if (XQueryTree (gdk_x11_get_default_xdisplay(), xid, &root,
                        &parent, &children, &nchildren) == 0) {

            g_warning ("Couldn't find window manager window");
            return None;
        }

        if (root == parent)
            return xid;
        xid = parent;
    } while (TRUE);

}//find wm window


/**
 * Function: get_window_screenshot
 *
 * Draws the pixel buffer from the specified window.
 *
 * Code adapted from xfce4-screenshooter
 * Copyright © 2008-2010 Jérôme Guelfucci <jeromeg@xfce.org>
 *
 * @param window The window to draw
 * @param border Wether to add window border.
 * @return screenshot The GdkPixbuf to be temporarily saved.
 */
static GdkPixbuf *
get_window_screenshot (GdkWindow *window,
                       gboolean border) {

    gint x_orig, y_orig;
    gint width, height;
    GdkPixbuf * screenshot;
    GdkWindow * root;
    GdkRectangle rectangle;

    root = gdk_get_default_root_window ();
    if (border) {
        Window xwindow = GDK_WINDOW_XWINDOW (window);
        window = gdk_window_foreign_new (find_wm_window (xwindow));
    }

    gdk_drawable_get_size (window, &rectangle.width, &rectangle.height);
    gdk_window_get_origin (window, &rectangle.x, &rectangle.y);

    x_orig = rectangle.x;
    y_orig = rectangle.y;
    width = rectangle.width;
    height = rectangle.height;

    if (x_orig < 0) {
        width = width + x_orig;
        x_orig = 0;
    }

    if (y_orig < 0) {
        height = height + y_orig;
        y_orig = 0;
    }

    if (x_orig + width > gdk_screen_width ())
        width = gdk_screen_width () - x_orig;

    if (y_orig + height > gdk_screen_height ())
        height = gdk_screen_height () - y_orig;

    screenshot = gdk_pixbuf_get_from_drawable (NULL, root, NULL,
                                               x_orig, y_orig, 0, 0,
                                               width, height);

  /* Code adapted from gnome-screenshot:
   * Copyright (C) 2001-2006 Jonathan Blandford <jrb@alum.mit.edu>
   * Copyright (C) 2008 Cosimo Cecchi <cosimoc@gnome.org>
   */
    if (border) {
        XRectangle *rectangles;
        GdkPixbuf *tmp;
        int rectangle_count, rectangle_order, i;
        rectangles = XShapeGetRectangles (gdk_x11_get_default_xdisplay(),
                                          GDK_WINDOW_XWINDOW (window),
                                          ShapeBounding,
                                          &rectangle_count,
                                          &rectangle_order);
        if (rectangles && rectangle_count > 0 && window != root) {
            gboolean has_alpha = gdk_pixbuf_get_has_alpha (screenshot);
            tmp = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, width, height);
            gdk_pixbuf_fill (tmp, 0);
            for (i = 0; i < rectangle_count; i++) { //1
                gint rec_x, rec_y;
                gint rec_width, rec_height;
                gint y;
                rec_x = rectangles[i].x;
                rec_y = rectangles[i].y;
                rec_width = rectangles[i].width;
                rec_height = rectangles[i].height;
                if (rectangle.x < 0) {
                    rec_x += rectangle.x;
                    rec_x = MAX(rec_x, 0);
                    rec_width += rectangle.x;
                }

                if (rectangle.y < 0) {
                    rec_y += rectangle.y;
                    rec_y = MAX(rec_y, 0);
                    rec_height += rectangle.y;
                }

                if (rec_x < 0) {
                    rec_width = rec_width + rec_x;
                    rec_x = 0;
                }

                if (rec_y < 0) {
                    rec_height = rec_height + rec_y;
                    rec_y = 0;
                }

                if (x_orig + rec_x + rec_width > gdk_screen_width ())
                    rec_width = gdk_screen_width () - x_orig - rec_x;

                if (y_orig + rec_y + rec_height > gdk_screen_height ())
                    rec_height = gdk_screen_height () - y_orig - rec_y;

                for (y = rec_y; y < rec_y + rec_height; y++) { //2
                    guchar *src_pixels, *dest_pixels;
                    gint x;
                    src_pixels = gdk_pixbuf_get_pixels (screenshot)
                        + y * gdk_pixbuf_get_rowstride(screenshot)
                        + rec_x * (has_alpha ? 4 : 3);
                    dest_pixels = gdk_pixbuf_get_pixels (tmp)
                        + y * gdk_pixbuf_get_rowstride (tmp)
                        + rec_x * 4;
                    for (x = 0; x < rec_width; x++) { //3
                        *dest_pixels++ = *src_pixels++;
                        *dest_pixels++ = *src_pixels++;
                        *dest_pixels++ = *src_pixels++;
                        if (has_alpha)
                            *dest_pixels++ = *src_pixels++;
                        else
                            *dest_pixels++ = 255;
                    }//for 3
                }//for 2
            }//for 1
            g_object_unref (screenshot);
            screenshot = tmp;
        }
    }//if border
    return screenshot;
}//get window screenshot


/**
 * Function: takeScreenshot
 *
 * Sets up params and calls functions to get a screenshot,
 * free pointers if nessesary.
 *
 * @return screenshot The final shot to be locally saved to a temp directory.
 */
GdkPixbuf *
takeScreenshot () {

    GdkPixbuf *screenshot = NULL;
    GdkWindow *window = NULL;
    GdkScreen *screen;
    GdkDisplay *display;
    gboolean border;

    /* gdk_get_default_root_window () does not need to be unrefed,
     * needs_unref enables us to unref *window only if a non default
     * window has been grabbed.
     */
    gboolean needs_unref = TRUE;

    /* Get the screen on which the screenshot should be taken */
    screen = gdk_screen_get_default ();

    /* Sync the display */
    gdk_display_sync (gdk_display_get_default ());
    gdk_window_process_all_updates ();

    /* Wait 2 seconds */
    sleep (2);

    /* Get the window/desktop we want to screenshot*/
    window = get_active_window (screen, &needs_unref, &border);
    screenshot = get_window_screenshot (window, border);

    /* Free Pointers */
    if (needs_unref)
        g_object_unref (window);

    return screenshot;

}//take screenshot
