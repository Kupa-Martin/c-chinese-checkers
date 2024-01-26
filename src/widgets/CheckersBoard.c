#include "CheckersBoard.h"
#include "CheckersBoardButton.h"
#include "CheckersWindow.h"
#include "../macro_utils.h"
#include <math.h>

// the first part (coords excluded) of the id property of `CheckersBoardButton`s widgets in ../../resources/markup/CheckersBoard.ui
#define BOARD_BUTTONS_ID "checkersboardbutton"

enum PropertyId {
    PROPERTY_PLAYERS = 1,
    PROPERTY_SLOT_RADIUS,
    PROPERTY_GAME_ACTIVE,
    PROPERTY_CURRENT_TURN_TEAM,
    PROPERTY_SELECTED_SLOT
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

// Type definitions
// First 3 letters indicate color, last letter indicates wether it's a ball (B) or a slot (S)
#define ETYS CHECKERS_BOARD_BUTTON_EMPTY_SLOT
#define REDS CHECKERS_BOARD_BUTTON_RED_SLOT
#define REDM CHECKERS_BOARD_BUTTON_RED_MARBLE
#define BLUS CHECKERS_BOARD_BUTTON_BLUE_SLOT
#define BLUM CHECKERS_BOARD_BUTTON_BLACK_MARBLE
#define GRES CHECKERS_BOARD_BUTTON_GREEN_SLOT
#define GREM CHECKERS_BOARD_BUTTON_GREEN_MARBLE
#define BLKS CHECKERS_BOARD_BUTTON_BLACK_SLOT
#define BLKM CHECKERS_BOARD_BUTTON_BLACK_MARBLE
#define YLWS CHECKERS_BOARD_BUTTON_YELLOW_SLOT
#define YLWM CHECKERS_BOARD_BUTTON_YELLOW_MARBLE
#define WHTS CHECKERS_BOARD_BUTTON_WHITE_SLOT
#define WHTM CHECKERS_BOARD_BUTTON_WHITE_MARBLE

static const CheckersBoardButtonState initialBoardState[][N_SLOTS] = {
    [CHECKERS_BOARD_EMPTY_BOARD] =
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
    [CHECKERS_BOARD_2_PLAYERS] = 
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
    [CHECKERS_BOARD_4_PLAYERS] =
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
    [CHECKERS_BOARD_6_PLAYERS] = 
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
G_STATIC_ASSERT(ARRAY_SIZE(initialBoardState) == CHECKERS_BOARD_N_PLAYERS);
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
    CheckersBoardPlayers players;
    gint slotRadius;
    gboolean gameActive;
    CheckersBoardButtonTeam currentTurnTeam;
    CheckersBoardButton *selectedSlot;
    CheckersBoardButton *highlightedSlots[N_DIRECTIONS];
    CheckersBoardButton *slotsPerTeam[CHECKERS_BOARD_BUTTON_N_TEAMS-1][SLOTS_PER_TEAM];
    // Child widgets
    GtkBox *rows[CHECKERS_BOARD_ROWS];
    CheckersBoardButton *slots[N_SLOTS];
};

struct _CheckersBoardClass {
    GtkBoxClass parent_class;
};

// End type definitions

// Forward declaration
G_DEFINE_TYPE(CheckersBoard, checkers_board, GTK_TYPE_BOX);

static void checkers_board_init(CheckersBoard *);
static void checkers_board_class_init(CheckersBoardClass *);
static void checkers_board_dispose(GObject *);
static void checkers_board_finalize(GObject *);
static void checkers_board_get_property(GObject *, guint, GValue *, GParamSpec *);
static void checkers_board_set_property(GObject *, guint, const GValue *, GParamSpec *);
static gint checkers_board_closure_compute_spacing(CheckersBoard *, gint);
static gint checkers_board_find_destination_index(CheckersBoard *self, CheckersBoardButton *moveFrom, CheckersBoardDirection direction);
static void checkers_board_find_and_populate_moves(CheckersBoard *self, CheckersBoardButton *moveFrom);
static CheckersBoardButton *checkers_board_find_move(CheckersBoard *self, CheckersBoardButton *moveFrom, CheckersBoardDirection direction);
static void checkers_board_lost_focus(GtkEventControllerFocus *self, gpointer);
// End forward declaration

static void checkers_board_init(CheckersBoard *self) {
    self->players = CHECKERS_BOARD_EMPTY_BOARD;
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
    
    size_t slotsPerTeamCounter[CHECKERS_BOARD_BUTTON_N_TEAMS-1] = {0};
    // Should be reference counted but im too dumb to find it in the source code
    GtkExpression *selfExpr = gtk_constant_expression_new(CHECKERS_TYPE_BOARD, self);
    GtkExpression *slotRadiusExpr = gtk_property_expression_new(CHECKERS_TYPE_BOARD, selfExpr, "slot-radius");
    for (guint row= 0; row < CHECKERS_BOARD_ROWS; row++) {
        guint precedingSlotsSum = slotSum[row];
        GtkBox *rowContainer = self->rows[row];
        for (guint i= 0; i < slotsPerRow[row]; i++) {
            guint slotIndex = precedingSlotsSum + i;
            CheckersBoardButtonState state = initialBoardState[self->players][slotIndex];
            CheckersBoardButtonTeam team = CHECKERS_BOARD_BUTTON_TEAM_FROM_STATE(state);  
            /** The column of the slot if the board were a 17 by 13 bidimensional array */
            guint column = i + ((13 - slotsPerRow[row]) >> 1);
            CheckersBoardButton *slot = CHECKERS_BOARD_BUTTON(g_object_new(CHECKERS_TYPE_BOARD_BUTTON, "team", team, "state", state, "row", row, "column", column, NULL));
            if (team != CHECKERS_BOARD_BUTTON_NEUTRAL)
                self->slotsPerTeam[TEAM_HASH(team)][slotsPerTeamCounter[TEAM_HASH(team)]++] = slot; 
            self->slots[slotIndex] = slot;
            gtk_expression_bind(slotRadiusExpr, slot, "radius", NULL);
            enum { BUFFER_SIZE = ARRAY_SIZE(BOARD_BUTTONS_ID "00-00") };
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, BOARD_BUTTONS_ID "%02u-%02u", row, i);
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

    g_object_class_install_property(objectClass, PROPERTY_PLAYERS, 
                                    g_param_spec_enum(
                                            "players", 
                                            "players", 
                                            "How many players are in the current game", 
                                            CHECKERS_TYPE_BOARD_PLAYERS, 
                                            CHECKERS_BOARD_EMPTY_BOARD, 
                                            G_PARAM_READWRITE
                                        )
                                    );
    g_object_class_install_property(objectClass, PROPERTY_CURRENT_TURN_TEAM, 
                                    g_param_spec_enum(
                                            "current-turn-team", 
                                            "current-turn-team", 
                                            "The team to which the curent turn belongs to", 
                                            CHECKERS_TYPE_BOARD_BUTTON_TEAM, 
                                            CHECKERS_BOARD_BUTTON_NEUTRAL, 
                                            G_PARAM_READABLE
                                        )
                                    );
    g_object_class_install_property(objectClass, PROPERTY_SLOT_RADIUS, 
                                    g_param_spec_int(
                                            "slot-radius",
                                            "slot-radius",
                                            "The radius of the board's buttons",
                                            0,
                                            G_MAXINT,
                                            20,
                                            G_PARAM_READWRITE
                                        )
                                    );
    g_object_class_install_property(objectClass, PROPERTY_GAME_ACTIVE, 
                                    g_param_spec_boolean(
                                            "game-active",
                                            "game-active",
                                            "Whether a game is ongoing",
                                            false,
                                            G_PARAM_READWRITE
                                        )
                                    );
    g_object_class_install_property(objectClass, PROPERTY_SELECTED_SLOT, 
                                    g_param_spec_object(
                                            "selected-slot",
                                            "selected-slot",
                                            "The slot currently selected",
                                            CHECKERS_TYPE_BOARD_BUTTON,
                                            G_PARAM_READWRITE
                                        )
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
        case PROPERTY_PLAYERS: {
            g_value_set_enum(value, (gint)self->players);
            return;
        }
        case PROPERTY_SLOT_RADIUS: {
            g_value_set_int(value, self->slotRadius);
            return;
        }
        case PROPERTY_GAME_ACTIVE: {
            g_value_set_boolean(value, self->gameActive);
            return;
        }
        case PROPERTY_CURRENT_TURN_TEAM: {
            g_value_set_enum(value, (gint)self->currentTurnTeam); 
            return;
        }
        case PROPERTY_SELECTED_SLOT: {
            g_value_set_object(value, self->selectedSlot);
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static void checkers_board_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    CheckersBoard *self = CHECKERS_BOARD(object);
    
    switch (propertyId) {
        case PROPERTY_PLAYERS: {
            self->players = (CheckersBoardPlayers)g_value_get_enum(value);
            const CheckersBoardButtonState *data = initialBoardState[self->players];
            for (size_t i= 0; i < N_SLOTS; i++) 
                checkers_board_button_set_state(self->slots[i], data[i]);
            return;
        }
        case PROPERTY_SLOT_RADIUS: {
            self->slotRadius = g_value_get_int(value);
            return;
        }
        case PROPERTY_GAME_ACTIVE: {
            bool gameActive = g_value_get_boolean(value);
            g_assert((gameActive != self->gameActive) && "Setting Checkers::Board::gameActive to its own value");
            self->gameActive = gameActive;
            self->currentTurnTeam = gameActive ? CHECKERS_BOARD_BUTTON_RED_TEAM : CHECKERS_BOARD_BUTTON_NEUTRAL;
            for (size_t i= 0; i < N_SLOTS; i++) 
                gtk_widget_set_can_focus(GTK_WIDGET(self->slots[i]), gameActive);
            return;
        }
        case PROPERTY_CURRENT_TURN_TEAM: {
            self->currentTurnTeam = (CheckersBoardButtonTeam)g_value_get_enum(value);
            return;
        }
        case PROPERTY_SELECTED_SLOT: {
            self->selectedSlot = (CheckersBoardButton *)g_value_get_object(value);
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static gint checkers_board_closure_compute_spacing(CheckersBoard *self, gint boardButtonDiameter) {
    /**
     * Consider:
     *                  CheckersBoardButton 
     *                          /|\
     *                         / | \
     *                        /  |  \  2(r+c)
     *                       / 2r|   \
     *                      /    |    \
     * CheckersBoardButton ------------- CheckersBoardButton
     *                     (r+c)   (r+c)
     * 
     * Where 
     *  2c is the spacing between buttons in a row (what we need to compute)
     *  r is the radius of the buttons (i.e CheckersBoardButton::radius, passed as boardButtonDiameter)
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
static gint checkers_board_find_destination_index(CheckersBoard *self, CheckersBoardButton *moveFrom, CheckersBoardDirection direction) { 
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
    guint row = checkers_board_button_get_row(moveFrom);
    guint column = checkers_board_button_get_column(moveFrom);
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

static CheckersBoardButton *checkers_board_find_move(CheckersBoard *self, CheckersBoardButton *moveFrom, CheckersBoardDirection direction) {
    g_assert(moveFrom != NULL);
    g_assert(direction < N_DIRECTIONS);
    bool jumpingOver = false;
    tryAgain:;
    gint destIdx = checkers_board_find_destination_index(self, moveFrom, direction);
    bool outOfBounds = destIdx < 0;
    if (outOfBounds)  
        return NULL; 
    guint destinationIndex = (guint)destIdx;
    CheckersBoardButton *destinationSlot = self->slots[destinationIndex];
    g_assert(destinationSlot != NULL);
    CheckersBoardButtonState destinationState = checkers_board_button_get_state(destinationSlot);
    bool isAvailable = CHECKERS_BOARD_BUTTON_OCCUPANCY_FROM_STATE(destinationState) == CHECKERS_BOARD_BUTTON_UNOCCUPIED;
    if (isAvailable) 
        return destinationSlot;
    if (jumpingOver)
        return NULL; /* Can only jump over one marble */
    /* attempt to jump over the occupied slot */
    jumpingOver = true;
    moveFrom = destinationSlot;
    goto tryAgain;
}

static void checkers_board_find_and_populate_moves(CheckersBoard *self, CheckersBoardButton *moveFrom) {
    g_assert(moveFrom != NULL);
    guint moveCounter = 0; 
    for (CheckersBoardDirection direction= 0; direction < N_DIRECTIONS; direction++) {
        CheckersBoardButton *destinationSlot = checkers_board_find_move(self, moveFrom, direction);
        bool noMovesAvailable = destinationSlot == NULL;
        if (noMovesAvailable) 
            continue;
        checkers_board_button_set_highlighted(destinationSlot, true);
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

extern CheckersBoardButtonTeam checkers_board_get_current_turn_team(CheckersBoard *self) {
    CheckersBoardButtonTeam hold; 
    g_object_get(self, "current-turn-team", &hold, NULL);
    return hold;
}

extern CheckersBoardButton *checkers_board_get_selected_slot(CheckersBoard *self) {
    return self->selectedSlot;
}

extern void checkers_board_unselect_slot(CheckersBoard *self) {
    g_assert(self->selectedSlot != NULL && "Called checkers::board::unselect_slot() when a slot wasnt selected");
    for (size_t i = 0; i < N_DIRECTIONS && self->highlightedSlots[i] != NULL ; i++) {
        CheckersBoardButton *highlightedSlot = self->highlightedSlots[i];
        checkers_board_button_set_highlighted(highlightedSlot, false);
        self->highlightedSlots[i] = NULL; 
    }
    checkers_board_button_set_selected(self->selectedSlot, false);
    self->selectedSlot = NULL;
}

extern void checkers_board_mark_slot_selected(CheckersBoard *self, CheckersBoardButton *slot) {
    g_assert(slot != NULL);
    if (self->selectedSlot != NULL)
        checkers_board_unselect_slot(self);
    /* Order is important. gtk::widget::grab_focus must be called before setting CheckersBoard::selectedSlot, otherwise Checkers::BoardButton::track_focus will assume focus was just lost */
    checkers_board_button_set_selected(slot, true);
    self->selectedSlot = slot;
    checkers_board_find_and_populate_moves(self, slot);
}

extern void checkers_board_move_selected_slot(CheckersBoard *self, CheckersBoardButton *destination) {
    g_assert(checkers_board_button_is_highlighted(destination) && "Attempted to move selection to an unhighlighted slot");
    g_assert(self->selectedSlot != NULL && "Called checkers::board::move_selected_slot when no slot was selected");
    CheckersBoardButtonState newState = checkers_board_button_get_state(self->selectedSlot);
    checkers_board_button_set_state(destination, newState);
    CheckersBoardButtonTeam team = checkers_board_button_get_team(self->selectedSlot);
    checkers_board_button_set_state(self->selectedSlot, (CheckersBoardButtonState)(team | CHECKERS_BOARD_BUTTON_UNOCCUPIED));
    checkers_board_unselect_slot(self);
}