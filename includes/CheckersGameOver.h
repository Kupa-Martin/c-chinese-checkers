#ifndef __CHECKERS_GAME_OVER_H__
#define __CHECKERS_GAME_OVER_H__

#include "CheckersTeam.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersGameOver, checkers_game_over, CHECKERS, GAME_OVER, GtkBox);

#define CHECKERS_TYPE_GAME_OVER checkers_game_over_get_type()
#define CHECKERS_GAME_OVER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_GAME_OVER, CheckersGameOverClass))
#define CHECKERS_GAME_OVER(game_over) (G_TYPE_CHECK_INSTANCE_CAST ((game_over), CHECKERS_TYPE_GAME_OVER, CheckersGameOver))


extern GtkWidget *checkers_game_over_new(void);
extern void checkers_game_over_set_winner(CheckersGameOver *self, CheckersTeam winner);
extern CheckersTeam checkers_game_over_get_winner(CheckersGameOver *self);
// End forward declarations
G_END_DECLS

#endif