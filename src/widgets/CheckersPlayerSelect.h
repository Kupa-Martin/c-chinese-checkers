#ifndef __CHECKERS_PLAYER_SELECT_H__
#define __CHECKERS_PLAYER_SELECT_H__

#include "../includes.h"

G_BEGIN_DECLS

#define CHECKERS_TYPE_PLAYER_SELECT checkers_player_select_get_type()
#define CHECKERS_PLAYER_SELECT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_PLAYER_SELECT, CheckersPlayerSelectClass))

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersPlayerSelect, checkers_player_select, CHECKERS, PLAYER_SELECT, GtkBox);

extern GtkWidget *checkers_player_select_new(void);
// End forward declarations

G_END_DECLS

#endif