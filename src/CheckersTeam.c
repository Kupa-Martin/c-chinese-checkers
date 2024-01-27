#include "CheckersTeam.h"
#include <gtk/gtk.h>

#ifdef DEBUG
#include "macro_utils.h"
#endif

extern CheckersTeam checkers_team_compute_next_team(CheckersTeam team, CheckersPlayers players) {
#ifdef DEBUG
#define PLAYER_HASH(player) ((player) - 1)
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
    g_assert(foundValidPlayer && "checkers::board::button::compute_next_team received an invalid player count");
    // CHECKERS_SLOT_N_TEAMS is used as a sentinel
    static const CheckersTeam validTeams[CHECKERS_N_PLAYERS-1][CHECKERS_N_TEAMS] = {
        [PLAYER_HASH(CHECKERS_2_PLAYERS)] = {
            CHECKERS_RED_TEAM,
            CHECKERS_BLACK_TEAM,
            CHECKERS_N_TEAMS
        },
        [PLAYER_HASH(CHECKERS_4_PLAYERS)] = {
            CHECKERS_WHITE_TEAM,
            CHECKERS_GREEN_TEAM,
            CHECKERS_BLUE_TEAM,
            CHECKERS_YELLOW_TEAM,
            CHECKERS_N_TEAMS
        },
        [PLAYER_HASH(CHECKERS_6_PLAYERS)] = {
            CHECKERS_RED_TEAM,
            CHECKERS_BLACK_TEAM,
            CHECKERS_WHITE_TEAM,
            CHECKERS_GREEN_TEAM,
            CHECKERS_BLUE_TEAM,
            CHECKERS_YELLOW_TEAM,
            CHECKERS_N_TEAMS
        }
    };
    gboolean foundValidTeam = false;
    for (size_t i= 0; i < ARRAY_SIZE(validTeams[0]) && i != CHECKERS_N_TEAMS; i++) {
        foundValidTeam = validTeams[PLAYER_HASH(players)][i] == team;
        if (foundValidTeam)
            break;
    }
    g_assert(foundValidTeam && "checkers::board::button::compute_next_team got an invalid team for the given players");
#undef PLAYER_HASH
#endif
    gint step = (players == CHECKERS_4_PLAYERS) ? (-2) : 2;
    guint offset = (players == CHECKERS_4_PLAYERS) ? 4 : 0;
    guint mod = (4*players);
    CheckersTeam nextTeam = (CheckersTeam)((guint)((gint)team + step) % mod + offset);
    return nextTeam;
}