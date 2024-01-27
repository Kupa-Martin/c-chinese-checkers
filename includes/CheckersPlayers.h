#ifndef __CHECKERS_PLAYERS_H__
#define __CHECKERS_PLAYERS_H__

#include "enum_types.h"

typedef enum {
    CHECKERS_NO_PLAYERS,
    CHECKERS_2_PLAYERS,
    CHECKERS_4_PLAYERS,
    CHECKERS_6_PLAYERS,
    CHECKERS_N_PLAYERS /*< skip >*/
} CheckersPlayers;

#endif /* __CHECKERS_PLAYERS_H__ */