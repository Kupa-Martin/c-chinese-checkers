#ifndef __CHECKERS_BOARD_H__
#define __CHECKERS_BOARD_H__

#include "../enum_types.h"
#include "CheckersBoardButton.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersBoard, checkers_board, CHECKERS, BOARD, GtkBox);

#define CHECKERS_TYPE_BOARD (checkers_board_get_type())
#define CHECKERS_BOARD(board) (G_TYPE_CHECK_INSTANCE_CAST ((board), CHECKERS_TYPE_BOARD, CheckersBoard))
#define CHECKERS_BOARD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), CHECKERS_TYPE_BOARD, CheckersBoardClass))
#define CHECKERS_BOARD_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_BOARD, CheckersBoardClass))

// Type definitions
typedef enum {
    CHECKERS_BOARD_EMPTY_BOARD,
    CHECKERS_BOARD_2_PLAYERS,
    CHECKERS_BOARD_4_PLAYERS,
    CHECKERS_BOARD_6_PLAYERS,
    CHECKERS_BOARD_N_PLAYERS /*< skip >*/
} CheckersBoardPlayers;
// End type definitions


extern GtkWidget *checkers_board_new(void);
extern guint checkers_board_get_slots(gsize row);
extern gboolean checkers_board_is_game_active(CheckersBoard *self);
extern void checkers_board_set_game_active(CheckersBoard *self, gboolean gameActive);
extern CheckersBoardButtonTeam checkers_board_get_current_turn_team(CheckersBoard *self);
extern CheckersBoardButton *checkers_board_get_selected_slot(CheckersBoard *self);
extern void checkers_board_unselect_slot(CheckersBoard *self);
extern void checkers_board_mark_slot_selected(CheckersBoard *self, CheckersBoardButton *slot);
extern void checkers_board_move_selected_slot(CheckersBoard *self, CheckersBoardButton *destination);
// End forward declarations
G_END_DECLS

#endif