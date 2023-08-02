#include "../includes.h"
#include <math.h>
#ifdef DEBUG
#include <errno.h>
#endif

// the first part (coords excluded) of the id property of `CheckersBoardButton`s widgets in ../../ui/markup/CheckersBoard.ui
#define BOARD_BUTTONS_ID "checkersboardbutton"
#define BOARD_BUTTONS_ROWS 10

enum {
    N_SLOTS = 121 /* The number of `BoardButton`s in the board */
};

enum PropertyId {
    PROPERTY_PLAYERS = 1,
    PROPERTY_SLOT_RADIUS
};

static gint columns[] = {
    1,2,3,4,5,6,7,8,9,10
};
G_STATIC_ASSERT(sizeof columns / sizeof *columns == BOARD_BUTTONS_ROWS);

// Type definitions

struct XY {
    size_t x, y;
};

enum {
    // First 3 letters indicate color, last letter indicates wether it's a ball (B) or a slot (S)
    ETYS = CHECKERS_BOARD_BUTTON_EMPTY_SLOT,
    REDS = CHECKERS_BOARD_BUTTON_RED_SLOT,
    REDB = CHECKERS_BOARD_BUTTON_RED_BALL,
    BLUS = CHECKERS_BOARD_BUTTON_BLUE_SLOT,
    BLUB = CHECKERS_BOARD_BUTTON_BLUE_BALL,
    GRES = CHECKERS_BOARD_BUTTON_GREEN_SLOT,
    GREB = CHECKERS_BOARD_BUTTON_GREEN_BALL,
    BLKS = CHECKERS_BOARD_BUTTON_BLACK_SLOT,
    BLKB = CHECKERS_BOARD_BUTTON_BLACK_BALL,
    YLWS = CHECKERS_BOARD_BUTTON_YELLOW_SLOT,
    YLWB = CHECKERS_BOARD_BUTTON_YELLOW_BALL,
    WHTS = CHECKERS_BOARD_BUTTON_WHITE_SLOT,
    WHTB = CHECKERS_BOARD_BUTTON_WHITE_BALL
};

static const CheckersBoardButtonSource initialBoardDistributionData[CHECKERS_BOARD_N_PLAYER_VALUES][N_SLOTS] = {
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

struct _CheckersBoard {
    GtkBox parent_instance;

    // Properties
    CheckersBoardPlayers players;
    gint slotRadius;

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
static inline struct XY toXY(size_t); // private conversion function
// End forward declaration

static void checkers_board_init(CheckersBoard *self) {
    self->players = CHECKERS_BOARD_EMPTY_BOARD;
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/CheckersBoard.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    g_object_unref(cssProvider);
    return;
}

#ifdef TRUE
typedef struct MyParseData {
    int rows;
    int previousRow;
    int currentRow;
    int columns[BOARD_BUTTONS_ROWS];
} MyParseData;
void markupParse_startElement(GMarkupParseContext *context,
                              const gchar         *element_name,
                              const gchar        **attribute_names,
                              const gchar        **attribute_values,
                              gpointer             user_data,
                              GError             **error) 
{
    // Only interested in <object> tag
    if (g_strcmp0("object", element_name) != 0)
        return;

    while (*attribute_names != NULL) {
        if (g_strcmp0("class", *attribute_names) == 0) {
            if (g_strcmp0("CheckersBoardButton", *attribute_values) != 0) {
                // Only interested in tags whose class attribute equals "CheckersBoardButton"
                break;
            }
        } else if (g_strcmp0("id", *attribute_names) == 0) {
            // Check if id starts with BOARD_BUTTONS_ID
            enum { BUFFER_SIZE = sizeof(BOARD_BUTTONS_ID) };
            char buffer[BUFFER_SIZE];
            g_strlcpy(buffer, *attribute_values, BUFFER_SIZE);
            if (g_strcmp0(BOARD_BUTTONS_ID, buffer) == 0) {
                // Check if id's size is as expected
                bool hasExpectedLength = strlen(*attribute_values) != sizeof(BOARD_BUTTONS_ID) - 1 + 5; 
                if () {

                    g_assert();
                }
                // retrieve rows
                char numBuffer[3];
                gchar* endPtr;
                g_strlcpy(numBuffer, *attribute_values + (BUFFER_SIZE - 1), 3);
                errno = 0;
                gint64 row = g_ascii_strtoll(numBuffer, &endPtr, 10);
                if (row == 0 && errno != 0) {
                    if (numBuffer == endPtr) {
                        //longjmp failed assert. Invalid row value
                    } else {
                        //longjmp failed assert. Something else happened
                    }
                }
                g_strlcpy(numBuffer, *attribute_values + (BUFFER_SIZE - 1) + 3, 3);
                gint64 column = g_ascii_strtoll(numBuffer, &endPtr, 10);
                if (column == 0 && errno != 0) {
                    if (numBuffer == endPtr) {
                        //longjmp failed assert. Invalid column value
                    } else {
                        //longjmp failed assert. Something else happened
                    }
                }
                
                if (row < 0 || row >= BOARD_BUTTONS_ROWS) {
                    // BOARD_BUTTONS_ROWS doesnt match rows in .ui file
                }
                if (column < 0 || column >= columns[row]) {
                    // The columns array doesnt match the columns in the .ui file
                }

                MyParseData *data = user_data;
                data->columns[row]++;
            } else {
                //longjmp failed assert. CheckersBoardButton with invalid id
            }

            // Once we find the id attribute, there is no need to keep iterating
            break;
        }
        attribute_names++;
        attribute_values++;
    }
    
}

#endif

static void checkers_board_class_init(CheckersBoardClass *klass){
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

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/CheckersBoard.ui");

#ifdef TRUE
    //TODO: Check whether the board's id match the string BOARD_BUTTONS_ID and if it has the rows CHECKERS_BOARD_ROWS specifies
    struct MyParseData userData = {.rows = 0, .previousRow = -1, .columns = {0}};
    GBytes *uiFile = g_resources_lookup_data("/com/fullaccess/ChineseCheckers/ui/markup/CheckersBoard.ui", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
    gsize fileSize;
    const guint8 *data = g_bytes_get_data(uiFile, &fileSize);

    // Count the number of child elements
    GMarkupParser parser = {
        .start_element = markupParse_startElement
    };

    // Parse the template XML data
    GMarkupParseContext *context = g_markup_parse_context_new(&parser, G_MARKUP_TREAT_CDATA_AS_TEXT, &userData, NULL);
    g_markup_parse_context_parse(context, (const gchar *)data, fileSize, NULL);

    g_markup_parse_context_unref(context);
#endif

    for (size_t i= 0; i < N_SLOTS; i++) {
        struct XY temp = toXY(i); // I have to convert the sequencial numbering of the buttons to their position expressed as rows, column.
        enum {
            STRING_SIZE = sizeof(BOARD_BUTTONS_ID "%02zu-%02zu")
        };
        char objectId[STRING_SIZE]; // It doesnt need to persist, gtk_widget_class_bind_template_child_full will call stddup() on it

        snprintf(objectId, STRING_SIZE, BOARD_BUTTONS_ID "%02zu-%02zu", temp.x, temp.y);
        gtk_widget_class_bind_template_child_full(widgetClass, objectId, FALSE, G_STRUCT_OFFSET(CheckersBoard, slots)+((glong)(i * sizeof(CheckersBoardButton *))));
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
            break;
        }
        case PROPERTY_SLOT_RADIUS: {
            self->slotRadius = g_value_get_int(value);
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
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
    return round(boardButtonDiameter * 0.15470053);
    //                   2r          * (sqrt(4/3)-1)
}

static inline struct XY toXY(size_t num) {
    if (num == 0) return (struct XY){0, 0};
    else if (num < 3) return (struct XY){1, num - 1};
    else if (num < 6) return (struct XY){2, num - 3};
    else if (num < 10) return (struct XY){3, num - 6};
    else if (num < 23) return (struct XY){4, num - 10};
    else if (num < 35) return (struct XY){5, num - 23};
    else if (num < 46) return (struct XY){6, num - 35};
    else if (num < 56) return (struct XY){7, num - 46};
    else if (num < 65) return (struct XY){8, num - 56};
    else if (num < 75) return (struct XY){9, num - 65};
    else if (num < 86) return (struct XY){10, num - 75};
    else if (num < 98) return (struct XY){11, num - 86};
    else if (num < 111) return (struct XY){12, num - 98};
    else if (num < 115) return (struct XY){13, num - 111};
    else if (num < 118) return (struct XY){14, num - 115};
    else if (num < 120) return (struct XY){15, num - 118};
    else if (num == 120) return (struct XY){16, num - 120};
    else g_abort(); // this should never happen
}

extern GtkWidget *checkers_board_new(void) {
    return g_object_new(CHECKERS_TYPE_BOARD, NULL);
}

