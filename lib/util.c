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

#include "util.h"
#include <time.h>

/**
 * Function: copyToClipboard
 *
 * Copies the given url to the default copy/paste clipboard.
 *
 * @param url The string to be copied.
 */
void
copyToClipboard (char * url){

    GtkClipboard * board;
    board = gtk_clipboard_get (GDK_SELECTION_PRIMARY);
    gtk_clipboard_set_text (board, url, -1);

}//copy to clipboard



/**
 * Function: tempSave
 *
 * Temporarily saves the screenshot to the local machine.
 *
 * @param savePath The path the the temporary file
 */
gchar *
tempSave(GdkPixbuf * screenshot) {

    GFile * tempDir = g_file_new_for_path(g_get_tmp_dir());
    const gchar * tempUri = g_file_get_uri(tempDir);

    srand(time(NULL));
    int i = rand() % 1000 + 100;
    gchar * name;
    itoa(i, name, 10);

    gchar * saveUri = g_build_filename (tempUri, name, NULL);
    GFile * saveFile = g_file_new_for_uri (saveUri);

    GError * error = NULL;
    gchar * savePath = g_file_get_path (saveFile);

    if (G_UNLIKELY (!gdk_pixbuf_save (screenshot, savePath, "png", &error, NULL))) {
        if (error) {
            screenshooter_error ("%s", error->message);
            g_error_free (error);
        }
        g_free (savePath);
        g_object_unref (saveFile);
        return NULL;
    } else {
        g_object_unref (saveFile);
        return savePath;
    }

}//temp save
