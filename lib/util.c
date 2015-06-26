#include "global.h"

void
copyToClipboard (char * url){

  GtkClipboard * board;  
  board = gtk_clipboard_get (GDK_SELECTION_PRIMARY);
  gtk_clipboard_set_text (board, url, -1);

}//copy to clipboard
