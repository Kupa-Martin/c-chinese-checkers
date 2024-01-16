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
    PROPERTY_GAME_ACTIVE
};

#define FOR_SLOTS_PER_ROW(DO) \
    DO(1) DO(2) DO(3) DO(4) DO(13) DO(12) DO(11) DO(10) DO(9) DO(10) DO(11) DO(12) DO(13) DO(4) DO(3) DO(2) DO(1)

#define COMMA(x) x,
static const guint slotsPerRow[] = { FOR_SLOTS_PER_ROW(COMMA) };
#undef COMMA
enum { CHECKERS_BOARD_ROWS = ARRAY_SIZE(slotsPerRow) };
#define SUM(x) +x
enum { N_SLOTS = 0 + FOR_SLOTS_PER_ROW(SUM)  /* The number of `BoardButton`s in the board */ };
#undef SUM

// Type definitions

// First 3 letters indicate color, last letter indicates wether it's a ball (B) or a slot (S)
#define ETYS CHECKERS_BOARD_BUTTON_EMPTY_SLOT
#define REDS CHECKERS_BOARD_BUTTON_RED_SLOT
#define REDB CHECKERS_BOARD_BUTTON_RED_BALL
#define BLUS CHECKERS_BOARD_BUTTON_BLUE_SLOT
#define BLUB CHECKERS_BOARD_BUTTON_BLUE_BALL
#define GRES CHECKERS_BOARD_BUTTON_GREEN_SLOT
#define GREB CHECKERS_BOARD_BUTTON_GREEN_BALL
#define BLKS CHECKERS_BOARD_BUTTON_BLACK_SLOT
#define BLKB CHECKERS_BOARD_BUTTON_BLACK_BALL
#define YLWS CHECKERS_BOARD_BUTTON_YELLOW_SLOT
#define YLWB CHECKERS_BOARD_BUTTON_YELLOW_BALL
#define WHTS CHECKERS_BOARD_BUTTON_WHITE_SLOT
#define WHTB CHECKERS_BOARD_BUTTON_WHITE_BALL

static const CheckersBoardButtonSource initialBoardDistributionData[][N_SLOTS] = {
    // CHECKERS_BOARD_EMPTY_BOARD
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
    // CHECKERS_BOARD_2_PLAYERS 
    {
                                                        BLKB,
                                                    BLKB,    BLKB,
                                                BLKB,    BLKB,    BLKB,
                                            BLKB,    BLKB,    BLKB,   BLKB,
        YLWS,   YLWS,   YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,   GRES,   GRES,
            YLWS,   YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,   GRES,
                YLWS,   YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,   GRES,
                    YLWS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GRES,
                        ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,
                    WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,
                WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,
            WHTS,   WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,   BLUS,
        WHTS,   WHTS,   WHTS,   WHTS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUS,   BLUS,   BLUS,   BLUS,
                                            REDB,    REDB,    REDB,   REDB,
                                                REDB,    REDB,    REDB,
                                                    REDB,    REDB,
                                                        REDB
    },
    // CHECKERS_BOARD_4_PLAYERS
    {
                                                        BLKS,
                                                    BLKS,    BLKS,
                                                BLKS,    BLKS,    BLKS,
                                            BLKS,    BLKS,    BLKS,   BLKS,
        YLWB,   YLWB,   YLWB,   YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,   GREB,   GREB,   GREB,
            YLWB,   YLWB,   YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,   GREB,   GREB,
                YLWB,   YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,   GREB,
                    YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,
                        ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,
                    WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,
                WHTB,   WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,   BLUB,
            WHTB,   WHTS,   WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,   BLUB,   BLUB,
        WHTB,   WHTB,   WHTB,   WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,   BLUB,   BLUB,   BLUB,
                                            REDS,    REDS,    REDS,   REDS,
                                                REDS,    REDS,    REDS,
                                                    REDS,    REDS,
                                                        REDS
    },
    // CHECKERS_BOARD_6_PLAYERS 
    {
                                                        BLKB,
                                                    BLKB,    BLKB,
                                                BLKB,    BLKB,    BLKB,
                                            BLKB,    BLKB,    BLKB,   BLKB,
        YLWB,   YLWB,   YLWB,   YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,   GREB,   GREB,   GREB,
            YLWB,   YLWB,   YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,   GREB,   GREB,
                YLWB,   YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,   GREB,
                    YLWB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   GREB,
                        ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,
                    WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,
                WHTB,   WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,   BLUB,
            WHTB,   WHTS,   WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,   BLUB,   BLUB,
        WHTB,   WHTB,   WHTB,   WHTB,   ETYS,   ETYS,   ETYS,   ETYS,   ETYS,   BLUB,   BLUB,   BLUB,   BLUB,
                                            REDB,    REDB,    REDB,   REDB,
                                                REDB,    REDB,    REDB,
                                                    REDB,    REDB,
                                                        REDB
    }
};
#undef ETYS
#undef REDS
#undef REDB
#undef BLUS
#undef BLUB
#undef GRES
#undef GREB
#undef BLKS
#undef BLKB
#undef YLWS
#undef YLWB
#undef WHTS
#undef WHTB

G_STATIC_ASSERT(ARRAY_SIZE(initialBoardDistributionData) == CHECKERS_BOARD_N_PLAYER_VALUES);

struct _CheckersBoard {
    GtkBox parent_instance;

    // Properties
    CheckersBoardPlayers players;
    gint slotRadius;
    gboolean gameActive;

    // Child widgets
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
static void checkers_board_constructed(GObject *);
static gint checkers_board_closure_computeSpacingForEquilateralTriangle(CheckersBoard *, gint);
// End forward declaration

static void checkers_board_init(CheckersBoard *self) {
    self->players = CHECKERS_BOARD_EMPTY_BOARD;
    self->gameActive = false;
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/resources/styles/CheckersBoard.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    g_object_unref(cssProvider);	
    return;
}

static void checkers_board_class_init(CheckersBoardClass *klass) {
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->set_property = checkers_board_set_property;
    objectClass->get_property = checkers_board_get_property;
    objectClass->constructed = checkers_board_constructed;
    objectClass->dispose = checkers_board_dispose;
    objectClass->finalize = checkers_board_finalize;

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

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/resources/markup/CheckersBoard.ui");

    for (guint i= 0; i < N_SLOTS; i++) {
        // I have to convert the sequencial numbering of the buttons to their position expressed as rows, column.
        struct {guint row, column;} temp = {0}; 
        while (temp.column <= i) 
            temp.column += slotsPerRow[temp.row++];
        temp.column = i - (temp.column - slotsPerRow[--temp.row]);

        enum { OBJECT_ID_BUFFER_SIZE = sizeof(BOARD_BUTTONS_ID "%02u-%02u") };
        char objectIdBuffer[OBJECT_ID_BUFFER_SIZE]; // It doesnt need to persist, gtk_widget_class_bind_template_child_full will call strdup() on it

        snprintf(objectIdBuffer, OBJECT_ID_BUFFER_SIZE, BOARD_BUTTONS_ID "%02u-%02u", temp.row, temp.column);
        gtk_widget_class_bind_template_child_full(widgetClass, objectIdBuffer, FALSE, G_STRUCT_OFFSET(CheckersBoard, slots)+((glong)(i * sizeof(CheckersBoardButton *))));
    }
    gtk_widget_class_bind_template_callback(widgetClass, checkers_board_closure_computeSpacingForEquilateralTriangle);
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
            g_value_set_enum(value, self->players);
            break;
        }
        case PROPERTY_SLOT_RADIUS: {
            g_value_set_int(value, self->slotRadius);
            break;
        }
        case PROPERTY_GAME_ACTIVE: {
            g_value_set_boolean(value, self->gameActive);
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void checkers_board_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    CheckersBoard *self = CHECKERS_BOARD(object);
    
    switch (propertyId) {
        case PROPERTY_PLAYERS: {
            self->players = g_value_get_enum(value);
            for (size_t i= 0; i < N_SLOTS; i++) 
                checkers_board_button_set_source(self->slots[i], initialBoardDistributionData[self->players][i]);
            return;
        }
        case PROPERTY_SLOT_RADIUS: {
            self->slotRadius = g_value_get_int(value);
            return;
        }
        case PROPERTY_GAME_ACTIVE: {
            self->gameActive = g_value_get_boolean(value);
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static void checkers_board_constructed(GObject *object) {    
    CheckersBoard *self = CHECKERS_BOARD(object);
    for (size_t i= 0; i < N_SLOTS; i++) 
        checkers_board_button_set_source(self->slots[i], initialBoardDistributionData[self->players][i]);
    G_OBJECT_CLASS(checkers_board_parent_class)->constructed(object);
}

static gint checkers_board_closure_computeSpacingForEquilateralTriangle(CheckersBoard *self, gint boardButtonDiameter) {
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

