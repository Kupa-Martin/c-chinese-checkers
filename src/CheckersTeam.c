#include "CheckersTeam.h"
#include "internal/CheckersTeam.h"
#include "internal/CheckersPlayers.h"
#include "macro_utils.h"
#include <gtk/gtk.h>

// Forward declarations
#ifdef DEBUG
static gboolean checkers_team_validate_team_given_players(CheckersTeam team, CheckersPlayers players);
// End forward declarations

// Function definitons
static gboolean checkers_team_validate_team_given_players(CheckersTeam team, CheckersPlayers players) {
    gboolean validPlayers = f_checkers_players_validate_players(players);
    if (!validPlayers) 
        return false;
#define PLAYER_HASH(player) ((player) - 1)
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
#undef PLAYER_HASH
    return foundValidTeam;
}

gboolean f_checkers_team_validate_team(CheckersTeam team) {
    return checkers_team_validate_team_given_players(team, CHECKERS_6_PLAYERS);
}
#endif /* DEBUG */

extern CheckersTeam checkers_team_compute_next_team(CheckersTeam team, CheckersPlayers players) {
    g_assert(checkers_team_validate_team_given_players(team, players) && "checkers::board::button::compute_next_team got an invalid team for the given players");
    gint step = (players == CHECKERS_4_PLAYERS) ? (-2) : 2;
    guint offset = (players == CHECKERS_4_PLAYERS) ? 4 : 0;
    guint mod = (4*players);
    CheckersTeam nextTeam = (CheckersTeam)((guint)((gint)team + step) % mod + offset);
    return nextTeam;
}

extern CheckersTeam checkers_team_compute_opponent(CheckersTeam team) {
    g_assert(f_checkers_team_validate_team(team) && "checkers::team::compute_opponent received invalid team");
    gboolean isOdd = team & 2;
    return (CheckersTeam)((gint)team + 2*(isOdd ? -1 : 1));
}