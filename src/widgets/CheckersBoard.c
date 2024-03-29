#include "CheckersBoard.h"
#include "CheckersWindow.h"
#include "CheckersTeam.h"
#include "internal/CheckersTeam.h"
#include "marshals.h"
#include "macro_utils.h"
#include <math.h>

// the first part (coords excluded) of the id property of `CheckersSlot`s widgets in ../../resources/markup/CheckersBoard.ui
#define SLOT_ID "checkersslot"

enum PropertyId {
    PLAYERS_PROPERTY = 1,
    SLOT_RADIUS_PROPERTY,
    GAME_ACTIVE_PROPERTY,
    CURRENT_TURN_TEAM_PROPERTY,
    SELECTED_SLOT_PROPERTY,
    N_PROPERTIES
};

enum SignalId {
    GAME_OVER_SIGNAL,
    N_SIGNALS
};

typedef enum CheckersBoardDirection {
    TOP_LEFT,
    TOP_RIGHT,
    LEFT,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    N_DIRECTIONS
} CheckersBoardDirection;

#define X_SLOTS(X) \
    X(1) X(2) X(3) X(4) X(13) X(12) X(11) X(10) X(9) X(10) X(11) X(12) X(13) X(4) X(3) X(2) X(1)

#define LIST_SLOTS(slots) slots,
static const guint slotsPerRow[] = { X_SLOTS(LIST_SLOTS) };
#undef LIST_SLOTS
#define SUM_SLOTS(slots) +slots
enum {
    /* The number of `BoardButton`s in the board */
    N_SLOTS = 0 + X_SLOTS(SUM_SLOTS),
    CHECKERS_BOARD_ROWS = ARRAY_SIZE(slotsPerRow)
};
#undef SUM_SLOTS
/** The sum of all slots in preceding rows */
static const guint slotSum[CHECKERS_BOARD_ROWS] = { 0, 1, 3, 6, 10, 23, 35, 46, 56, 65, 75, 86, 98, 111, 115, 118, 120 };
static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];

// Type definitions
// First 3 letters indicate color, last letter indicates wether it's a ball (B) or a slot (S)
#define ETYS CHECKERS_SLOT_EMPTY_SLOT
#define REDS CHECKERS_SLOT_RED_SLOT
#define REDM CHECKERS_SLOT_RED_MARBLE
#define BLUS CHECKERS_SLOT_BLUE_SLOT
#define BLUM CHECKERS_SLOT_BLUE_MARBLE
#define GRES CHECKERS_SLOT_GREEN_SLOT
#define GREM CHECKERS_SLOT_GREEN_MARBLE
#define BLKS CHECKERS_SLOT_BLACK_SLOT
#define BLKM CHECKERS_SLOT_BLACK_MARBLE
#define YLWS CHECKERS_SLOT_YELLOW_SLOT
#define YLWM CHECKERS_SLOT_YELLOW_MARBLE
#define WHTS CHECKERS_SLOT_WHITE_SLOT
#define WHTM CHECKERS_SLOT_WHITE_MARBLE

static const CheckersSlotState initialBoardState[][N_SLOTS] = {
    [CHECKERS_NO_PLAYERS] =
    {
                                                        BLKS,
                                                    BLKS,    BLKS,
                                                BLKS,    BLKS,    BLKS,
                                            BLKS,    BLKS,    BLKS,   BLKS,
        YLWS,   YLWS,   YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,   GRES,   GRES,
            YLWS,   YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,   GRES,
                YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,
                    YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,
                        ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,
                    WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,
                WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,
            WHTS,   WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,   BLUS,
        WHTS,   WHTS,   WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,   BLUS,   BLUS,
                                            REDS,    REDS,    REDS,   REDS,
                                                REDS,    REDS,    REDS,
                                                    REDS,    REDS,
                                                        REDS
    },
    [CHECKERS_2_PLAYERS] = 
    {
                                                        BLKM,
                                                    BLKM,    BLKM,
                                                BLKM,    BLKM,    BLKM,
                                            BLKM,    BLKM,    BLKM,   BLKM,
        YLWS,   YLWS,   YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,   GRES,   GRES,
            YLWS,   YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,   GRES,
                YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,
                    YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,
                        ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,
                    WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,
                WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,
            WHTS,   WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,   BLUS,
        WHTS,   WHTS,   WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,   BLUS,   BLUS,
                                            REDM,    REDM,    REDM,   REDM,
                                                REDM,    REDM,    REDM,
                                                    REDM,    REDM,
                                                        REDM
    },
    [CHECKERS_4_PLAYERS] =
    {
                                                        BLKS,
                                                    BLKS,    BLKS,
                                                BLKS,    BLKS,    BLKS,
                                            BLKS,    BLKS,    BLKS,   BLKS,
        YLWM,   YLWM,   YLWM,   YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,   GREM,   GREM,   GREM,
            YLWM,   YLWM,   YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,   GREM,   GREM,
                YLWM,   YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,   GREM,
                    YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,
                        ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,
                    WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,
                WHTM,   WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,   BLUM,
            WHTM,   WHTM,   WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,   BLUM,   BLUM,
        WHTM,   WHTM,   WHTM,   WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,   BLUM,   BLUM,   BLUM,
                                            REDS,    REDS,    REDS,   REDS,
                                                REDS,    REDS,    REDS,
                                                    REDS,    REDS,
                                                        REDS
    },
    [CHECKERS_6_PLAYERS] = 
    {
                                                        BLKM,
                                                    BLKM,    BLKM,
                                                BLKM,    BLKM,    BLKM,
                                            BLKM,    BLKM,    BLKM,   BLKM,
        YLWM,   YLWM,   YLWM,   YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,   GREM,   GREM,   GREM,
            YLWM,   YLWM,   YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,   GREM,   GREM,
                YLWM,   YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,   GREM,
                    YLWM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREM,
                        ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,
                    WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,
                WHTM,   WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,   BLUM,
            WHTM,   WHTM,   WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,   BLUM,   BLUM,
        WHTM,   WHTM,   WHTM,   WHTM,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUM,   BLUM,   BLUM,   BLUM,
                                            REDM,    REDM,    REDM,   REDM,
                                                REDM,    REDM,    REDM,
                                                    REDM,    REDM,
                                                        REDM
    }
};
G_STATIC_ASSERT(ARRAY_SIZE(initialBoardState) == CHECKERS_N_PLAYERS);
enum { 
    SLOTS_PER_TEAM = 10 
};
#undef ETYS
#undef REDS
#undef REDM
#undef BLUS
#undef BLUM
#undef GRES
#undef GREM
#undef BLKS
#undef BLKM
#undef YLWS
#undef YLWM
#undef WHTS
#undef WHTM

#define TEAM_HASH(team) ((team) >> 1)
struct _CheckersBoard {
    GtkBox parent_instance;

    // Properties
    GtkEventControllerFocus *focusController;
    CheckersPlayers players;
    gint slotRadius;
    gboolean gameActive;
    CheckersTeam currentTurnTeam;
    CheckersSlot *selectedSlot;
    CheckersSlot *highlightedSlots[N_DIRECTIONS];
    CheckersSlot *slotsPerTeam[CHECKERS_N_TEAMS-1][SLOTS_PER_TEAM];
    // Child widgets
    GtkBox *rows[CHECKERS_BOARD_ROWS];
    CheckersSlot *slots[N_SLOTS];
};

struct _CheckersBoardClass {
    GtkBoxClass parent_class;
};

// End type definitions

// Friendly accesses
#ifdef DEBUG
extern gboolean (*const priv_checkers_team_validate_team)(CheckersTeam);
#endif /* DEBUG */
// End friendly accesses

// Forward declaration
G_DEFINE_TYPE(CheckersBoard, checkers_board, GTK_TYPE_BOX);

static void checkers_board_init(CheckersBoard *);
static void checkers_board_class_init(CheckersBoardClass *);
static void checkers_board_dispose(GObject *);
static void checkers_board_finalize(GObject *);
static void checkers_board_get_property(GObject *, guint, GValue *, GParamSpec *);
static void checkers_board_set_property(GObject *, guint, const GValue *, GParamSpec *);
static gint checkers_board_closure_compute_spacing(CheckersBoard *, gint);
static gint checkers_board_find_destination_index(CheckersBoard *self, CheckersSlot *moveFrom, CheckersBoardDirection direction);
static void checkers_board_find_and_populate_moves(CheckersBoard *self, CheckersSlot *moveFrom);
static CheckersSlot *checkers_board_find_move(CheckersBoard *self, CheckersSlot *moveFrom, CheckersBoardDirection direction);
static void checkers_board_lost_focus(GtkEventControllerFocus *self, gpointer);
static gboolean checkers_board_check_win_condition(CheckersBoard *self, CheckersTeam team);
static void checkers_board_on_game_over(CheckersBoard *self);
static void checkers_board_set_selected_slot(CheckersBoard *self, CheckersSlot *selectedSlot);
// End forward declaration

static void checkers_board_init(CheckersBoard *self) {
    self->players = CHECKERS_NO_PLAYERS;
    self->gameActive = false;
    self->selectedSlot = NULL;
    for (size_t i= 0; i < N_DIRECTIONS; i++)  
       self->highlightedSlots[i] = NULL; 
     
    gtk_widget_init_template(GTK_WIDGET(self));
    self->focusController = GTK_EVENT_CONTROLLER_FOCUS(gtk_event_controller_focus_new());
    g_signal_connect(self->focusController, "leave", G_CALLBACK(checkers_board_lost_focus), NULL);
    gtk_widget_add_controller(GTK_WIDGET(self), GTK_EVENT_CONTROLLER(self->focusController));
    
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/resources/styles/CheckersBoard.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    size_t slotsPerTeamCounter[CHECKERS_N_TEAMS-1] = {0};
    // Should be reference counted but im too dumb to find it in the source code
    GtkExpression *selfExpr = gtk_constant_expression_new(CHECKERS_TYPE_BOARD, self);
    GtkExpression *slotRadiusExpr = gtk_property_expression_new(CHECKERS_TYPE_BOARD, selfExpr, "slot-radius");
    for (guint row= 0; row < CHECKERS_BOARD_ROWS; row++) {
        guint precedingSlotsSum = slotSum[row];
        GtkBox *rowContainer = self->rows[row];
        for (guint i= 0; i < slotsPerRow[row]; i++) {
            guint slotIndex = precedingSlotsSum + i;
            CheckersSlotState state = initialBoardState[self->players][slotIndex];
            CheckersTeam team = CHECKERS_SLOT_TEAM_FROM_STATE(state);  
            /** The column of the slot if the board were a 17 by 13 bidimensional array */
            guint column = i + ((13 - slotsPerRow[row]) >> 1);
            CheckersSlot *slot = CHECKERS_SLOT(g_object_new(CHECKERS_TYPE_SLOT, "team", team, "state", state, "row", row, "column", column, NULL));
            if (team != CHECKERS_NO_TEAM) {
                CheckersTeam opponentTeam = checkers_team_compute_opponent(team);
                self->slotsPerTeam[TEAM_HASH(opponentTeam)][slotsPerTeamCounter[TEAM_HASH(opponentTeam)]++] = slot; 
            }
            self->slots[slotIndex] = slot;
            gtk_expression_bind(slotRadiusExpr, slot, "radius", NULL);
            enum { BUFFER_SIZE = ARRAY_SIZE(SLOT_ID "00-00") };
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, SLOT_ID "%02u-%02u", row, i);
            gtk_widget_set_name(GTK_WIDGET(slot), buffer);
            gtk_box_append(rowContainer, GTK_WIDGET(slot));
        }
    }

    g_object_unref(cssProvider);	
    return;
}

static void checkers_board_class_init(CheckersBoardClass *klass) {
    GtkWidgetClass *widgetClass = GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->finalize = checkers_board_finalize;
    objectClass->set_property = checkers_board_set_property;
    objectClass->get_property = checkers_board_get_property;
    objectClass->dispose = checkers_board_dispose;

    properties[PLAYERS_PROPERTY] = g_param_spec_enum(
        "players", 
        "players", 
        "How many players are in the current game", 
        CHECKERS_TYPE_PLAYERS, 
        CHECKERS_NO_PLAYERS, 
        G_PARAM_READWRITE
    );
    properties[CURRENT_TURN_TEAM_PROPERTY] = g_param_spec_enum(
        "current-turn-team", 
        "current-turn-team", 
        "The team to which the curent turn belongs to", 
        CHECKERS_TYPE_TEAM, 
        CHECKERS_NO_TEAM, 
        G_PARAM_READABLE
    );
    properties[SLOT_RADIUS_PROPERTY] = g_param_spec_int(
        "slot-radius",
        "slot-radius",
        "The radius of the board's buttons",
        0,
        G_MAXINT,
        20,
        G_PARAM_READWRITE
    );
    properties[GAME_ACTIVE_PROPERTY] = g_param_spec_boolean(
        "game-active",
        "game-active",
        "Whether a game is ongoing",
        false,
        G_PARAM_READWRITE
    );
    properties[SELECTED_SLOT_PROPERTY] = g_param_spec_object(
        "selected-slot",
        "selected-slot",
        "The slot currently selected",
        CHECKERS_TYPE_SLOT,
        G_PARAM_READWRITE
    );
    g_object_class_install_properties(objectClass, N_PROPERTIES, properties);
    signals[GAME_OVER_SIGNAL] = g_signal_new(
        "game-over",
        CHECKERS_TYPE_BOARD,
        G_SIGNAL_RUN_FIRST,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__ENUM,
        G_TYPE_NONE,
        1,
        CHECKERS_TYPE_TEAM
    );
    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/resources/markup/CheckersBoard.ui");
 
    for (size_t i= 0; i < CHECKERS_BOARD_ROWS; i++) {
        char boxId[ARRAY_SIZE("row00")];
        snprintf(boxId, ARRAY_SIZE(boxId), "row%zu", i+1);
        glong structOffset = G_STRUCT_OFFSET(CheckersBoard, rows) + ((glong)(i*sizeof(GtkBox *)));
        gtk_widget_class_bind_template_child_full(widgetClass, boxId, FALSE, structOffset);
    }
    gtk_widget_class_bind_template_callback(widgetClass, checkers_board_closure_compute_spacing);
    return;
}

static void checkers_board_dispose(GObject *object) {
    gtk_widget_dispose_template(GTK_WIDGET(object), CHECKERS_TYPE_WINDOW);
    G_OBJECT_CLASS(checkers_board_parent_class)->dispose(object);
    return;
}

static void checkers_board_finalize(GObject *object) {
    G_OBJECT_CLASS(checkers_board_parent_class)->finalize(object);
    return;
}

static void checkers_board_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec) {
    CheckersBoard *self = CHECKERS_BOARD(object);

    switch (propertyId) {
        case PLAYERS_PROPERTY: {
            g_value_set_enum(value, (gint)self->players);
            return;
        }
        case SLOT_RADIUS_PROPERTY: {
            g_value_set_int(value, self->slotRadius);
            return;
        }
        case GAME_ACTIVE_PROPERTY: {
            g_value_set_boolean(value, self->gameActive);
            return;
        }
        case CURRENT_TURN_TEAM_PROPERTY: {
            g_value_set_enum(value, (gint)self->currentTurnTeam); 
            return;
        }
        case SELECTED_SLOT_PROPERTY: {
            g_value_set_object(value, self->selectedSlot);
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static void checkers_board_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    CheckersBoard *self = CHECKERS_BOARD(object);
    
    switch (propertyId) {
        case PLAYERS_PROPERTY: {
            self->players = (CheckersPlayers)g_value_get_enum(value);
            const CheckersSlotState *data = initialBoardState[self->players];
            for (size_t i= 0; i < N_SLOTS; i++) 
                checkers_slot_set_state(self->slots[i], data[i]);
            return;
        }
        case SLOT_RADIUS_PROPERTY: {
            self->slotRadius = g_value_get_int(value);
            return;
        }
        case GAME_ACTIVE_PROPERTY: {
            bool gameActive = g_value_get_boolean(value);
            g_assert((gameActive != self->gameActive) && "Setting Checkers::Board::gameActive to its own value");
            self->gameActive = gameActive;
            self->currentTurnTeam = gameActive ? CHECKERS_RED_TEAM : CHECKERS_NO_TEAM;
            for (size_t i= 0; i < N_SLOTS; i++) 
                gtk_widget_set_can_focus(GTK_WIDGET(self->slots[i]), gameActive);
            return;
        }
        case CURRENT_TURN_TEAM_PROPERTY: {
            self->currentTurnTeam = (CheckersTeam)g_value_get_enum(value);
            return;
        }
        case SELECTED_SLOT_PROPERTY: {
            self->selectedSlot = (CheckersSlot *)g_value_get_object(value);
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static gint checkers_board_closure_compute_spacing(CheckersBoard *self, gint boardButtonDiameter) {
    /**
     * Consider:
     *                     CheckersSlot 
     *                          /|\
     *                         / | \
     *                        /  |  \  2(r+c)
     *                       / 2r|   \
     *                      /    |    \
     *        CheckersSlot ------------- CheckersSlot
     *                     (r+c)   (r+c)
     * 
     * Where 
     *  2c is the spacing between buttons in a row (what we need to compute)
     *  r is the radius of the buttons (i.e Checkers::Slot::radius, passed as boardButtonDiameter)
     * 
     * From this follows:
     * 2c = 2(sqrt(4/3)-1) * r
    */ 
    return (gint)round(boardButtonDiameter * 0.15470053);
    //                   2r          * (sqrt(4/3)-1)
}

/**
 * Finds the destination index for moving slot moveFrom one step in the direction specified by direction.
 * returns:
 *  -1 if the destination is out of bounds
*/
static gint checkers_board_find_destination_index(CheckersBoard *self, CheckersSlot *moveFrom, CheckersBoardDirection direction) { 
    static const struct {
        gint deltaRow;
        gint deltaColumn;
    } directions[N_DIRECTIONS] = {
        [TOP_LEFT] = {.deltaRow = -1, .deltaColumn = -1},
        [TOP_RIGHT] = {.deltaRow = -1, .deltaColumn = 0},
        [LEFT] = {.deltaRow = 0, .deltaColumn = -1},
        [RIGHT] = {.deltaRow = 0, .deltaColumn = 1},
        [BOTTOM_LEFT] = {.deltaRow = 1, .deltaColumn = -1},
        [BOTTOM_RIGHT] = {.deltaRow = 1, .deltaColumn = 0}
    };
    g_assert(direction < N_DIRECTIONS);
    g_assert(moveFrom != NULL);
    guint row = checkers_slot_get_row(moveFrom);
    guint column = checkers_slot_get_column(moveFrom);
    gint destinationRow = (gint)row + directions[direction].deltaRow;
    gboolean rowOutOfBounds = destinationRow < 0 || destinationRow >= CHECKERS_BOARD_ROWS;
    /* columns have an offset for easier navigation that treats the board as a 17 by 13 bidimensional array */
    gboolean rowIsOdd = row & 1;
    gint columnOffset = (gint)((13 - slotsPerRow[destinationRow * !rowOutOfBounds]) >> 1) - (rowIsOdd && destinationRow != (gint)row);
    gint destinationColumn = (gint)column + directions[direction].deltaColumn - columnOffset;
    gboolean columnOutOfBounds = destinationColumn < 0 || destinationColumn >= (gint)slotsPerRow[destinationRow * !rowOutOfBounds];
    gboolean outOfBounds = rowOutOfBounds || columnOutOfBounds;
    if (outOfBounds)
        return -1;
    guint destinationIndex = slotSum[destinationRow] + (guint)destinationColumn;
    return (gint)destinationIndex;
}

static CheckersSlot *checkers_board_find_move(CheckersBoard *self, CheckersSlot *moveFrom, CheckersBoardDirection direction) {
    g_assert(moveFrom != NULL);
    g_assert(direction < N_DIRECTIONS);
    bool jumpingOver = false;
    tryAgain:;
    gint destIdx = checkers_board_find_destination_index(self, moveFrom, direction);
    bool outOfBounds = destIdx < 0;
    if (outOfBounds)  
        return NULL; 
    guint destinationIndex = (guint)destIdx;
    CheckersSlot *destinationSlot = self->slots[destinationIndex];
    g_assert(destinationSlot != NULL);
    CheckersSlotState destinationState = checkers_slot_get_state(destinationSlot);
    bool isAvailable = CHECKERS_SLOT_OCCUPANCY_FROM_STATE(destinationState) == CHECKERS_SLOT_UNOCCUPIED;
    if (isAvailable) 
        return destinationSlot;
    if (jumpingOver)
        return NULL; /* Can only jump over one marble */
    /* attempt to jump over the occupied slot */
    jumpingOver = true;
    moveFrom = destinationSlot;
    goto tryAgain;
}

static void checkers_board_find_and_populate_moves(CheckersBoard *self, CheckersSlot *moveFrom) {
    g_assert(moveFrom != NULL);
    guint moveCounter = 0; 
    for (CheckersBoardDirection direction= 0; direction < N_DIRECTIONS; direction++) {
        CheckersSlot *destinationSlot = checkers_board_find_move(self, moveFrom, direction);
        bool noMovesAvailable = destinationSlot == NULL;
        if (noMovesAvailable) 
            continue;
        checkers_slot_set_highlighted(destinationSlot, true);
        self->highlightedSlots[moveCounter++] = destinationSlot;
    }
    if (moveCounter < N_DIRECTIONS)
        self->highlightedSlots[moveCounter] = NULL;
}

static void checkers_board_lost_focus(GtkEventControllerFocus *self, gpointer data) {
    GtkWidget *w = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(self));
    CheckersBoard *board = CHECKERS_BOARD(w);
    if (board->selectedSlot != NULL) 
        checkers_board_unselect_slot(board);
}

static gboolean checkers_board_check_win_condition(CheckersBoard *self, CheckersTeam team) {
    g_assert(f_checkers_team_validate_team(team) && "checkers::board::check_win_condition received an invalid team");
    gboolean teamWon = true;
    for (size_t i= 0; i < SLOTS_PER_TEAM; i++) {
        CheckersSlot *opponentSlot = self->slotsPerTeam[TEAM_HASH(team)][i];
        gboolean slotTaken = checkers_slot_get_state(opponentSlot) == (CHECKERS_SLOT_OCCUPIED | team);
        if (!slotTaken) {
            teamWon = false;
            break;
        } 
    }
    return teamWon;
}

static void checkers_board_on_game_over(CheckersBoard *self) {
#ifdef DEBUG
    gboolean winConditionMet = false;
    for (CheckersTeam team= 0; team < CHECKERS_NO_TEAM; team+=2) {
        winConditionMet = checkers_board_check_win_condition(self, team);
        if (winConditionMet)
            break;
    }
    g_assert(winConditionMet && "Called checkers::board::on_game_over when game wasnt over");
#endif /* DEBUG */
    g_signal_emit(self, signals[GAME_OVER_SIGNAL], 0, self->currentTurnTeam); 
    checkers_board_set_game_active(self, false);
}

static void checkers_board_set_selected_slot(CheckersBoard *self, CheckersSlot *selectedSlot) {
    g_object_set(self, "selected-slot", selectedSlot, NULL); 
}

extern GtkWidget *checkers_board_new(void) {
    return g_object_new(CHECKERS_TYPE_BOARD, NULL);
}

extern guint checkers_board_get_slots(gsize row) {
#ifdef DEBUG
    if (row > CHECKERS_BOARD_ROWS) {
        g_error("Index out of bounds on checkers_board_get_slots(). Index: %zu", row);
    }
#endif
    return slotsPerRow[row];
}

extern gboolean checkers_board_is_game_active(CheckersBoard *self) {
    gboolean isActiveGame;
    g_object_get(self, "game-active", &isActiveGame, NULL);
    return isActiveGame;
}

extern void checkers_board_set_game_active(CheckersBoard *self, gboolean gameActive) {
    g_object_set(self, "game-active", gameActive, NULL); 
}

extern CheckersTeam checkers_board_get_current_turn_team(CheckersBoard *self) {
    CheckersTeam hold; 
    g_object_get(self, "current-turn-team", &hold, NULL);
    return hold;
}

extern CheckersSlot *checkers_board_get_selected_slot(CheckersBoard *self) {
    CheckersSlot *hold; 
    g_object_get(self, "selected-slot", &hold, NULL);
    return hold;
}

extern void checkers_board_unselect_slot(CheckersBoard *self) {
    g_assert(self->selectedSlot != NULL && "Called checkers::board::unselect_slot() when a slot wasnt selected");
    for (size_t i = 0; i < N_DIRECTIONS && self->highlightedSlots[i] != NULL ; i++) {
        CheckersSlot *highlightedSlot = self->highlightedSlots[i];
        checkers_slot_set_highlighted(highlightedSlot, false);
        self->highlightedSlots[i] = NULL; 
    }
    checkers_slot_set_selected(self->selectedSlot, false);
    checkers_board_set_selected_slot(self, NULL);
}

extern void checkers_board_mark_slot_selected(CheckersBoard *self, CheckersSlot *slot) {
    g_assert(slot != NULL);
    if (self->selectedSlot != NULL)
        checkers_board_unselect_slot(self);
    /* Order is important. gtk::widget::grab_focus must be called before setting CheckersBoard::selectedSlot, otherwise Checkers::BoardButton::track_focus will assume focus was just lost */
    checkers_slot_set_selected(slot, true);
    checkers_board_set_selected_slot(self, slot);
    checkers_board_find_and_populate_moves(self, slot);
}

extern void checkers_board_move_selected_slot(CheckersBoard *self, CheckersSlot *destination) {
    g_assert(checkers_slot_is_highlighted(destination) && "Attempted to move selection to an unhighlighted slot");
    g_assert(self->selectedSlot != NULL && "Called checkers::board::move_selected_slot when no slot was selected");
    CheckersSlotState newState = checkers_slot_get_state(self->selectedSlot);
    checkers_slot_set_state(destination, newState);
    CheckersTeam team = checkers_slot_get_team(self->selectedSlot);
    checkers_slot_set_state(self->selectedSlot, (CheckersSlotState)(team | CHECKERS_SLOT_UNOCCUPIED));
    checkers_board_unselect_slot(self);
    CheckersTeam nextTurnTeam = checkers_team_compute_next_team(self->currentTurnTeam, self->players);
    gboolean currentTeamWon = checkers_board_check_win_condition(self, self->currentTurnTeam);
    if (currentTeamWon) 
        return checkers_board_on_game_over(self);
    else
        self->currentTurnTeam = nextTurnTeam;
}