#ifndef __CHECKERS_BOARD_BUTTON_H__
#define __CHECKERS_BOARD_BUTTON_H__

#include "../includes.h"

G_BEGIN_DECLS

#define CHECKERS_TYPE_BOARD_BUTTON checkers_board_button_get_type()
#define CHECKERS_BOARD_BUTTON_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_BOARD_BUTTON, CheckersBoardButtonClass))

// Type definitions
typedef enum {
    CHECKERS_BOARD_BUTTON_EMPTY_SLOT,
    CHECKERS_BOARD_BUTTON_RED_SLOT,
    CHECKERS_BOARD_BUTTON_RED_BALL,
    CHECKERS_BOARD_BUTTON_BLUE_SLOT,
    CHECKERS_BOARD_BUTTON_BLUE_BALL,
    CHECKERS_BOARD_BUTTON_GREEN_SLOT,
    CHECKERS_BOARD_BUTTON_GREEN_BALL,
    CHECKERS_BOARD_BUTTON_BLACK_SLOT,
    CHECKERS_BOARD_BUTTON_BLACK_BALL,
    CHECKERS_BOARD_BUTTON_YELLOW_SLOT,
    CHECKERS_BOARD_BUTTON_YELLOW_BALL,
    CHECKERS_BOARD_BUTTON_WHITE_SLOT,
    CHECKERS_BOARD_BUTTON_WHITE_BALL,
    CHECKERS_BOARD_BUTTON_N_SOURCES /*< skip >*/
} CheckersBoardButtonSource;
// End type definitions

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersBoardButton, checkers_board_button, CHECKERS, BOARD_BUTTON, GtkButton);

extern GtkWidget *checkers_board_button_new(void);
extern CheckersBoardButtonSource checkers_board_button_get_source(CheckersBoardButton *);
extern void checkers_board_button_set_source(CheckersBoardButton *, CheckersBoardButtonSource);
// End forward declarations

G_END_DECLS

#endif