#ifndef __CHECKERS_BOARD_BUTTON_H__
#define __CHECKERS_BOARD_BUTTON_H__

#include "../enum_types.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersBoardButton, checkers_board_button, CHECKERS, BOARD_BUTTON, GtkButton);

#define CHECKERS_TYPE_BOARD_BUTTON checkers_board_button_get_type()
#define CHECKERS_BOARD_BUTTON_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_BOARD_BUTTON, CheckersBoardButtonClass))
#define CHECKERS_BOARD_BUTTON(board_button) (G_TYPE_CHECK_INSTANCE_CAST ((board_button), CHECKERS_TYPE_BOARD_BUTTON, CheckersBoardButton))

// Type definitions

typedef enum CheckersBoardButtonOccupancy {
    CHECKERS_BOARD_BUTTON_UNOCCUPIED = 0,
    CHECKERS_BOARD_BUTTON_OCCUPIED = 1,
    CHECKERS_BOARD_BUTTON_N_OCCUPANCIES = 2, /*< skip >*/
    CHECKERS_BOARD_BUTTON_OCCUPANCY_MASK = 1 /*< skip >*/
} CheckersBoardButtonOccupancy; 

typedef enum CheckersBoardButtonTeam {
    CHECKERS_BOARD_BUTTON_RED_TEAM = 0,
    CHECKERS_BOARD_BUTTON_BLUE_TEAM = 2,
    CHECKERS_BOARD_BUTTON_GREEN_TEAM = 4,
    CHECKERS_BOARD_BUTTON_BLACK_TEAM = 6,
    CHECKERS_BOARD_BUTTON_YELLOW_TEAM = 8,
    CHECKERS_BOARD_BUTTON_WHITE_TEAM = 10,
    CHECKERS_BOARD_BUTTON_NEUTRAL = 12,
    CHECKERS_BOARD_BUTTON_N_TEAMS = 7, /*< skip >*/
    CHECKERS_BOARD_BUTTON_TEAM_MASK = 14 /*< skip >*/
} CheckersBoardButtonTeam;

typedef enum {
    CHECKERS_BOARD_BUTTON_RED_SLOT = CHECKERS_BOARD_BUTTON_UNOCCUPIED | CHECKERS_BOARD_BUTTON_RED_TEAM,
    CHECKERS_BOARD_BUTTON_RED_MARBLE = CHECKERS_BOARD_BUTTON_OCCUPIED | CHECKERS_BOARD_BUTTON_RED_TEAM,
    CHECKERS_BOARD_BUTTON_BLUE_SLOT = CHECKERS_BOARD_BUTTON_UNOCCUPIED | CHECKERS_BOARD_BUTTON_BLUE_TEAM,
    CHECKERS_BOARD_BUTTON_BLUE_MARBLE = CHECKERS_BOARD_BUTTON_OCCUPIED | CHECKERS_BOARD_BUTTON_BLUE_TEAM,
    CHECKERS_BOARD_BUTTON_GREEN_SLOT = CHECKERS_BOARD_BUTTON_UNOCCUPIED | CHECKERS_BOARD_BUTTON_GREEN_TEAM,
    CHECKERS_BOARD_BUTTON_GREEN_MARBLE = CHECKERS_BOARD_BUTTON_OCCUPIED | CHECKERS_BOARD_BUTTON_GREEN_TEAM,
    CHECKERS_BOARD_BUTTON_BLACK_SLOT = CHECKERS_BOARD_BUTTON_UNOCCUPIED | CHECKERS_BOARD_BUTTON_BLACK_TEAM,
    CHECKERS_BOARD_BUTTON_BLACK_MARBLE = CHECKERS_BOARD_BUTTON_OCCUPIED | CHECKERS_BOARD_BUTTON_BLACK_TEAM,
    CHECKERS_BOARD_BUTTON_YELLOW_SLOT = CHECKERS_BOARD_BUTTON_UNOCCUPIED | CHECKERS_BOARD_BUTTON_YELLOW_TEAM,
    CHECKERS_BOARD_BUTTON_YELLOW_MARBLE = CHECKERS_BOARD_BUTTON_OCCUPIED | CHECKERS_BOARD_BUTTON_YELLOW_TEAM,
    CHECKERS_BOARD_BUTTON_WHITE_SLOT = CHECKERS_BOARD_BUTTON_UNOCCUPIED | CHECKERS_BOARD_BUTTON_WHITE_TEAM,
    CHECKERS_BOARD_BUTTON_WHITE_MARBLE = CHECKERS_BOARD_BUTTON_OCCUPIED | CHECKERS_BOARD_BUTTON_WHITE_TEAM,
    CHECKERS_BOARD_BUTTON_EMPTY_SLOT = CHECKERS_BOARD_BUTTON_UNOCCUPIED | CHECKERS_BOARD_BUTTON_NEUTRAL,
    CHECKERS_BOARD_BUTTON_N_STATES = 13 /*< skip >*/
} CheckersBoardButtonState;
// End type definitions

#define CHECKERS_BOARD_BUTTON_TEAM_FROM_STATE(state) ((CheckersBoardButtonTeam)((state) & CHECKERS_BOARD_BUTTON_TEAM_MASK))
#define CHECKERS_BOARD_BUTTON_OCCUPANCY_FROM_STATE(state) ((CheckersBoardButtonState)((state) & CHECKERS_BOARD_BUTTON_OCCUPANCY_MASK))

extern GtkWidget *checkers_board_button_new(void);
extern CheckersBoardButtonState checkers_board_button_get_state(CheckersBoardButton *);
extern void checkers_board_button_set_state(CheckersBoardButton *, CheckersBoardButtonState);
extern CheckersBoardButtonTeam checkers_board_button_get_team(CheckersBoardButton *self);
extern guint checkers_board_button_get_row(CheckersBoardButton *self);
extern guint checkers_board_button_get_column(CheckersBoardButton *self);
extern gboolean checkers_board_button_is_highlighted(CheckersBoardButton *self);
extern void checkers_board_button_set_highlighted(CheckersBoardButton *self, gboolean highlighted);
extern gboolean checkers_board_button_is_selected(CheckersBoardButton *self);
extern void checkers_board_button_set_selected(CheckersBoardButton *self, gboolean selected);
// End forward declarations

G_END_DECLS

#endif