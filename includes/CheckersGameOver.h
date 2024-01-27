#ifndef __CHECKERS_GAME_OVER_H__
#define __CHECKERS_GAME_OVER_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersGameOver, checkers_game_over, CHECKERS, GAME_OVER, GtkBox);

#define CHECKERS_TYPE_GAME_OVER checkers_game_over_get_type()
#define CHECKERS_GAME_OVER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_GAME_OVER, CheckersGameOverClass))


extern GtkWidget *checkers_game_over_new(void);
// End forward declarations
G_END_DECLS

#endif