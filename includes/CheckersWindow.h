#ifndef __CHECKERS_WINDOW_H__
#define __CHECKERS_WINDOW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersWindow, checkers_window, CHECKERS, WINDOW, GtkWindow);

#define CHECKERS_TYPE_WINDOW checkers_window_get_type()
#define CHECKERS_WINDOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_WINDOW, CheckersWindowClass))
#define CHECKERS_WINDOW(window) (G_TYPE_CHECK_INSTANCE_CAST ((window), CHECKERS_TYPE_WINDOW, CheckersWindow))


extern GtkWidget *checkers_window_new(void);
// End forward declarations
G_END_DECLS

#endif