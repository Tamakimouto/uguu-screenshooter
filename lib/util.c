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
	atoi(i, name, 10);
	
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
		g_object_unref (save_file);
		return NULL;
	} else {
		g_object_unref (save_file);
		return savePath;
	}
	
}//temp save
