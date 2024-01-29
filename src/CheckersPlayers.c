#include "CheckersPlayers.h"
#include "internal/CheckersPlayers.h"
#include "macro_utils.h"
#include <gtk/gtk.h>

#ifdef DEBUG
gboolean f_checkers_players_validate_players(CheckersPlayers players) {
    static const CheckersPlayers validPlayers[CHECKERS_N_PLAYERS-1] = {
        CHECKERS_2_PLAYERS,
        CHECKERS_4_PLAYERS,
        CHECKERS_6_PLAYERS
    };
    gboolean foundValidPlayer = false;
    for (size_t i= 0; i < ARRAY_SIZE(validPlayers); i++) {
        foundValidPlayer = players == validPlayers[i];
        if (foundValidPlayer)
            break;
    }
    return foundValidPlayer;
}
#endif /* DEBUG */