#ifndef __BOARD_WINDOW_H__
#define __BOARD_WINDOW_H__

#include "../includes.h"

G_BEGIN_DECLS

#define BOARD_TYPE_WINDOW board_window_get_type()
#define BOARD_WINDOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), BOARD_TYPE_WINDOW, BoardWindowClass))

// Type definitions
typedef enum {
    BOARD_WINDOW_EMPTY_BOARD,
    BOARD_WINDOW_2_PLAYERS,
    BOARD_WINDOW_4_PLAYERS,
    BOARD_WINDOW_6_PLAYERS
} BoardWindowInitialState;
// End type definitions

// Forward declarations
G_DECLARE_FINAL_TYPE(BoardWindow, board_window, BOARD, WINDOW, GtkWindow);

extern GtkWidget *board_window_new(void);
// End forward declarations
G_END_DECLS

#endif