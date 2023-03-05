#ifndef __BOARD_BUTTON_H__
#define __BOARD_BUTTON_H__

#include "../includes.h"

G_BEGIN_DECLS

#define BOARD_TYPE_BUTTON board_button_get_type()
#define BOARD_BUTTON_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), BOARD_TYPE_BUTTON, BoardButtonClass))

// Type definitions
typedef enum {
    BOARD_BUTTON_EMPTY_SLOT,
    BOARD_BUTTON_RED_SLOT,
    BOARD_BUTTON_RED_BALL,
    BOARD_BUTTON_BLUE_SLOT,
    BOARD_BUTTON_BLUE_BALL,
    BOARD_BUTTON_GREEN_SLOT,
    BOARD_BUTTON_GREEN_BALL,
    BOARD_BUTTON_BLACK_SLOT,
    BOARD_BUTTON_BLACK_BALL,
    BOARD_BUTTON_YELLOW_SLOT,
    BOARD_BUTTON_YELLOW_BALL,
    BOARD_BUTTON_WHITE_SLOT,
    BOARD_BUTTON_WHITE_BALL,
    BOARD_BUTTON_N_SOURCES /*< skip >*/
} BoardButtonSource;
// End type definitions

// Forward declarations
G_DECLARE_FINAL_TYPE(BoardButton, board_button, BOARD, BUTTON, GtkButton);

extern GtkWidget *board_button_new(void);
extern BoardButtonSource board_button_get_source(BoardButton *);
extern void board_button_set_source(BoardButton *, BoardButtonSource);
// End forward declarations

G_END_DECLS

#endif