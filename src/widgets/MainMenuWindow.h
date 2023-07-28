#ifndef __MAIN_MENU_WINDOW_H__
#define __MAIN_MENU_WINDOW_H__

#include "../includes.h"

G_BEGIN_DECLS

#define MAIN_MENU_TYPE_WINDOW main_menu_window_get_type()
#define MAIN_MENU_WINDOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), MAIN_MENU_TYPE_WINDOW, MainMenuClass))

// Forward declarations
G_DECLARE_FINAL_TYPE(MainMenuWindow, main_menu_window, MAIN_MENU, WINDOW, GtkWindow);

extern GtkWidget *main_menu_window_new(void);
// End forward declarations

G_END_DECLS

#endif