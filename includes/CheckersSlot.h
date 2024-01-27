#ifndef __CHECKERS_SLOT_H__
#define __CHECKERS_SLOT_H__

#include "enum_types.h"
#include "CheckersTeam.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

// Forward declarations
G_DECLARE_FINAL_TYPE(CheckersSlot, checkers_slot, CHECKERS, SLOT, GtkButton);

#define CHECKERS_TYPE_SLOT checkers_slot_get_type()
#define CHECKERS_SLOT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), CHECKERS_TYPE_SLOT, CheckersSlotClass))
#define CHECKERS_SLOT(slot) (G_TYPE_CHECK_INSTANCE_CAST ((slot), CHECKERS_TYPE_SLOT, CheckersSlot))

// Type definitions

typedef enum CheckersSlotOccupancy {
    CHECKERS_SLOT_UNOCCUPIED = 0,
    CHECKERS_SLOT_OCCUPIED = 1,
    CHECKERS_SLOT_N_OCCUPANCIES = 2, /*< skip >*/
    CHECKERS_SLOT_OCCUPANCY_MASK = 1 /*< skip >*/
} CheckersSlotOccupancy; 

typedef enum {
    CHECKERS_SLOT_RED_SLOT = CHECKERS_SLOT_UNOCCUPIED | CHECKERS_RED_TEAM,
    CHECKERS_SLOT_RED_MARBLE = CHECKERS_SLOT_OCCUPIED | CHECKERS_RED_TEAM,
    CHECKERS_SLOT_BLACK_SLOT = CHECKERS_SLOT_UNOCCUPIED | CHECKERS_BLACK_TEAM,
    CHECKERS_SLOT_BLACK_MARBLE = CHECKERS_SLOT_OCCUPIED | CHECKERS_BLACK_TEAM,
    CHECKERS_SLOT_WHITE_SLOT = CHECKERS_SLOT_UNOCCUPIED | CHECKERS_WHITE_TEAM,
    CHECKERS_SLOT_WHITE_MARBLE = CHECKERS_SLOT_OCCUPIED | CHECKERS_WHITE_TEAM,
    CHECKERS_SLOT_GREEN_SLOT = CHECKERS_SLOT_UNOCCUPIED | CHECKERS_GREEN_TEAM,
    CHECKERS_SLOT_GREEN_MARBLE = CHECKERS_SLOT_OCCUPIED | CHECKERS_GREEN_TEAM,
    CHECKERS_SLOT_BLUE_SLOT = CHECKERS_SLOT_UNOCCUPIED | CHECKERS_BLUE_TEAM,
    CHECKERS_SLOT_BLUE_MARBLE = CHECKERS_SLOT_OCCUPIED | CHECKERS_BLUE_TEAM,
    CHECKERS_SLOT_YELLOW_SLOT = CHECKERS_SLOT_UNOCCUPIED | CHECKERS_YELLOW_TEAM,
    CHECKERS_SLOT_YELLOW_MARBLE = CHECKERS_SLOT_OCCUPIED | CHECKERS_YELLOW_TEAM,
    CHECKERS_SLOT_EMPTY_SLOT = CHECKERS_SLOT_UNOCCUPIED | CHECKERS_NO_TEAM,
    CHECKERS_SLOT_N_STATES = 13 /*< skip >*/
} CheckersSlotState;
// End type definitions

#define CHECKERS_SLOT_TEAM_FROM_STATE(state) ((CheckersTeam)((state) & CHECKERS_TEAM_MASK))
#define CHECKERS_SLOT_OCCUPANCY_FROM_STATE(state) ((CheckersSlotState)((state) & CHECKERS_SLOT_OCCUPANCY_MASK))

extern GtkWidget *checkers_slot_new(void);
extern CheckersSlotState checkers_slot_get_state(CheckersSlot *);
extern void checkers_slot_set_state(CheckersSlot *, CheckersSlotState);
extern CheckersTeam checkers_slot_get_team(CheckersSlot *self);
extern guint checkers_slot_get_row(CheckersSlot *self);
extern guint checkers_slot_get_column(CheckersSlot *self);
extern gboolean checkers_slot_is_highlighted(CheckersSlot *self);
extern void checkers_slot_set_highlighted(CheckersSlot *self, gboolean highlighted);
extern gboolean checkers_slot_is_selected(CheckersSlot *self);
extern void checkers_slot_set_selected(CheckersSlot *self, gboolean selected);
// End forward declarations

G_END_DECLS

#endif