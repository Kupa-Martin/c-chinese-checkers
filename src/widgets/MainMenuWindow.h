#ifndef __MAIN_MENU_WINDOW_H__
#define __MAIN_MENU_WINDOW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

// Forward declarations
G_DECLARE_FINAL_TYPE(MainMenuWindow, main_menu_window, MAIN_MENU, WINDOW, GtkWindow);

#define MAIN_MENU_TYPE_WINDOW main_menu_window_get_type()
#define MAIN_MENU_WINDOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MAIN_MENU_TYPE_WINDOW, MainMenuClass))

extern GtkWidget *main_menu_window_new(void);
// End forward declarations

G_END_DECLS

#endif