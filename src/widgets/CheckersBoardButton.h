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
typedef enum {
    CHECKERS_BOARD_BUTTON_EMPTY_SLOT,
    CHECKERS_BOARD_BUTTON_RED_SLOT,
    CHECKERS_BOARD_BUTTON_RED_MARBLE,
    CHECKERS_BOARD_BUTTON_BLUE_SLOT,
    CHECKERS_BOARD_BUTTON_BLUE_MARBLE,
    CHECKERS_BOARD_BUTTON_GREEN_SLOT,
    CHECKERS_BOARD_BUTTON_GREEN_MARBLE,
    CHECKERS_BOARD_BUTTON_BLACK_SLOT,
    CHECKERS_BOARD_BUTTON_BLACK_MARBLE,
    CHECKERS_BOARD_BUTTON_YELLOW_SLOT,
    CHECKERS_BOARD_BUTTON_YELLOW_MARBLE,
    CHECKERS_BOARD_BUTTON_WHITE_SLOT,
    CHECKERS_BOARD_BUTTON_WHITE_MARBLE,
    CHECKERS_BOARD_BUTTON_N_SOURCES /*< skip >*/
} CheckersBoardButtonSource;
// End type definitions


extern GtkWidget *checkers_board_button_new(void);
extern CheckersBoardButtonSource checkers_board_button_get_source(CheckersBoardButton *);
extern void checkers_board_button_set_source(CheckersBoardButton *, CheckersBoardButtonSource);
// End forward declarations

G_END_DECLS

#endif