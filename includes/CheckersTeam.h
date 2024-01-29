#ifndef __CHECKERS_TEAM_H__
#define __CHECKERS_TEAM_H__

#include "enum_types.h"
#include "CheckersPlayers.h"

typedef enum CheckersTeam {
    CHECKERS_RED_TEAM = 0,
    CHECKERS_BLACK_TEAM = 2,
    CHECKERS_WHITE_TEAM = 4,
    CHECKERS_GREEN_TEAM = 6,
    CHECKERS_BLUE_TEAM = 8,
    CHECKERS_YELLOW_TEAM = 10,
    CHECKERS_NO_TEAM = 12,
    CHECKERS_N_TEAMS = 7, /*< skip >*/
    CHECKERS_TEAM_MASK = 14 /*< skip >*/
} CheckersTeam;

extern CheckersTeam checkers_team_compute_next_team(CheckersTeam team, CheckersPlayers players);
extern CheckersTeam checkers_team_compute_opponent(CheckersTeam team);

#endif /* __CHECKERS_TEAM_H__ */