#ifndef __CHECKERS_BOARD_H__
#define __CHECKERS_BOARD_H__

#include "../includes.h"

G_BEGIN_DECLS

#define CHECKERS_TYPE_BOARD checkers_board_get_type()
#define CHECKERS_BOARD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), CHECKERS_TYPE_BOARD, CheckersBoardClass))
#define CHECKERS_BOARD_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_BOARD, CheckersBoardClass))

// Type definitions
typedef enum {
    CHECKERS_BOARD_EMPTY_BOARD,
    CHECKERS_BOARD_2_PLAYERS,
    CHECKERS_BOARD_4_PLAYERS,
    CHECKERS_BOARD_6_PLAYERS,
    CHECKERS_BOARD_N_PLAYER_VALUES /*< skip >*/
} CheckersBoardPlayers;
// End type definitions

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersBoard, checkers_board, CHECKERS, BOARD, GtkBox);

extern GtkWidget *checkers_board_new(void);
// End forward declarations
G_END_DECLS

#endif