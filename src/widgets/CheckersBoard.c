#include "../includes.h"
#include <math.h>
#ifdef DEBUG
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#endif

// the first part (coords excluded) of the id property of `CheckersBoardButton`s widgets in ../../ui/markup/CheckersBoard.ui
#define BOARD_BUTTONS_ID "checkersboardbutton"


enum {
    N_SLOTS = 121 /* The number of `BoardButton`s in the board */
};

enum PropertyId {
    PROPERTY_PLAYERS = 1,
    PROPERTY_SLOT_RADIUS
};

static const int columns[] = { 1, 2, 3, 4, 13, 12, 11, 10, 9, 10, 11, 12, 13, 4, 3, 2, 1 };
enum { CHECKERS_BOARD_ROWS = ARRAY_SIZE(columns) };

// Type definitions

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

G_STATIC_ASSERT(ARRAY_SIZE(initialBoardDistributionData) == CHECKERS_BOARD_N_PLAYER_VALUES);

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
#ifdef DEBUG
static void markupParse_startElement(GMarkupParseContext *, const gchar *, const gchar **, const gchar **, gpointer, GError **); 
#endif
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


/**
 * This DEBUG block validates that the constants defined in this .c file are
 * consistent with the .ui file. It parses the .ui file and count the rows and
 * columns it finds
*/
#ifdef DEBUG
typedef struct MyParseData {
    int totalButtons;
    int rows;
    int columns[CHECKERS_BOARD_ROWS];
} MyParseData;
void markupParse_startElement(GMarkupParseContext *context,
                              const gchar         *element_name,
                              const gchar        **attribute_names,
                              const gchar        **attribute_values,
                              gpointer             user_data,
                              GError             **error) 
{
    bool onObjectTag = g_strcmp0("object", element_name) == 0;
    if (!onObjectTag)
        return; // Only interested in <object> tag

    while (*attribute_names != NULL) {
        bool onClassAttribute = g_strcmp0("class", *attribute_names) == 0;
        bool onIdAttribute = g_strcmp0("id", *attribute_names) == 0;
        if (onClassAttribute) {
            bool isBoardButton = g_strcmp0("CheckersBoardButton", *attribute_values) == 0;
            if (!isBoardButton) 
                return;
        }
        if (!onIdAttribute) {
            //Nothing to do, only interested in the id attribute
            attribute_names++;
            attribute_values++;
            continue;
        }
        // Check if id starts with BOARD_BUTTONS_ID
        enum { ID_START_BUFFER_SIZE = sizeof(BOARD_BUTTONS_ID) };
        char idStartBuffer[ID_START_BUFFER_SIZE];
        g_strlcpy(idStartBuffer, *attribute_values, ID_START_BUFFER_SIZE);
        bool idStartsWithExpectedValue = g_strcmp0(BOARD_BUTTONS_ID, idStartBuffer) == 0;
        if (idStartsWithExpectedValue) {
            // This BoardButton is now considered to be a slot in the CheckersBoard. It's id attribute must meet certain requirements
#define CURRENT_LINE_MESSAGE "Parsing file CheckersBoard.ui line: %d char: %d"
            int lineNumber;
            int charNumber;
            g_markup_parse_context_get_position(context, &lineNumber, &charNumber);

            // Check if id's size is as expected
            enum { EXPECTED_LENGTH = LENGTH(BOARD_BUTTONS_ID "00-00") };
		    bool hasExpectedLength = strlen(*attribute_values) == EXPECTED_LENGTH; 
            if (!hasExpectedLength) 
                g_error("Id's length is not as expected (expected %d). " CURRENT_LINE_MESSAGE, EXPECTED_LENGTH, lineNumber, charNumber);
            // retrieve the 2 digit numbers in the id
            enum { NUM_BUFFER_SIZE = sizeof("00") };
            char numBuffer[NUM_BUFFER_SIZE];
            char *endPtr;
            g_strlcpy(numBuffer, *attribute_values + LENGTH(BOARD_BUTTONS_ID), NUM_BUFFER_SIZE);
            errno = 0;
            int64_t row = g_ascii_strtoll(numBuffer, &endPtr, 10);
            bool error = row == 0 && errno != 0;
            if (error) {
                bool couldNotConvert = numBuffer == endPtr;
                if (couldNotConvert) {
		            g_error("The id's row value is not a number. " CURRENT_LINE_MESSAGE, lineNumber, charNumber);
                } else {
		            g_error("Could not retrieve row from CheckersBoardButton's id; g_ascii_strtoll() returned an unknown error number in errno: %d. " CURRENT_LINE_MESSAGE, errno, lineNumber, charNumber);
                }
            }
            g_strlcpy(numBuffer, *attribute_values + LENGTH(BOARD_BUTTONS_ID "00-"), NUM_BUFFER_SIZE);
            errno = 0;
            int64_t column = g_ascii_strtoll(numBuffer, &endPtr, 10);
            error = column == 0 && errno != 0;
            if (error) {
                bool couldNotConvert = numBuffer == endPtr;
                if (couldNotConvert) {
                    g_error("The id's column value is not a number. " CURRENT_LINE_MESSAGE, lineNumber, charNumber);
                } else {
                    g_error("Could not retrieve column from CheckersBoardButton's id; g_ascii_strtoll() returned an unknown error number in errno: %d. " CURRENT_LINE_MESSAGE, errno, lineNumber, charNumber);
                }
            }
            if (row < 0 || row >= CHECKERS_BOARD_ROWS)
		        g_error("The value of the CHECKERS_BOARD_ROWS macro in CheckersBoard.c doesnt match the rows in CheckersBoard.ui" CURRENT_LINE_MESSAGE, lineNumber, charNumber);
            
            if (column < 0 || column >= columns[row]) 
		        g_error("The amount of columns in the %" PRId64 "th row (i.e global variable columns[%" PRId64 "] in CheckersBoard.c) doesnt match the columns in CheckersBoard.ui" CURRENT_LINE_MESSAGE, row+1, row, lineNumber, charNumber);
#undef CURRENT_LINE_MESSAGE
            // Keep count of rows and columns. class_init() will compare the result with those provided at compile-time
            MyParseData *data = user_data;
		    if (data->columns[row] == 0)
		        data->rows++;
            data->columns[row]++;
            data->totalButtons++;
        }
        // Once we found and validated the id attribute, there is no need to keep iterating
        break;
    }
}
#endif

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

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/CheckersBoard.ui");
/**
 * This DEBUG block validates that the constants defined in this .c file are consistent with the .ui file. It checks the values in the columns[] array as well as the CHECKERS_BOARD_ROWS constant by comparing them with those gathered by the markupParse_startElement() function
*/
#ifdef DEBUG
    GBytes *uiFile = g_resources_lookup_data("/com/fullaccess/ChineseCheckers/ui/markup/CheckersBoard.ui", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
    gsize fileSize;
    const gchar *text = g_bytes_get_data(uiFile, &fileSize);

    GMarkupParser parser = {
        .start_element = markupParse_startElement,
	    .end_element = NULL,
	    .text = NULL,
	    .passthrough = NULL,
	    .error = NULL
    };

    // Parse CheckersBoard.ui
    MyParseData userData = {0};
    GMarkupParseContext *context = g_markup_parse_context_new(&parser, G_MARKUP_TREAT_CDATA_AS_TEXT, &userData, NULL);
    g_markup_parse_context_parse(context, text, fileSize, NULL);

    if (userData.totalButtons != N_SLOTS)
        g_error("The amount of buttons in CheckersBoard.ui (%d) doesnt match its corresponding constant N_SLOTS (%d) in CheckersBoard.c", userData.totalButtons, N_SLOTS);

    if (userData.rows != CHECKERS_BOARD_ROWS) 
	    g_error("The amount of rows in CheckersBoard.ui (%d) doesnt its corresponding constant CHECKERS_BOARD_ROWS (%d) in CheckersBoard.c", userData.rows, CHECKERS_BOARD_ROWS);

    for (size_t i = 0; i < CHECKERS_BOARD_ROWS; i++) {
        if (columns[i] != userData.columns[i]) 
	        g_error("The columns (%d) in CheckersBoard.ui's %zuth row dont match those in CheckersBoard.c's (coluns[%zu] = %d)", userData.columns[i], i+1, i,  columns[i]);
    }
    g_markup_parse_context_unref(context);
#endif

    for (size_t i= 0; i < N_SLOTS; i++) {
        // I have to convert the sequencial numbering of the buttons to their position expressed as rows, column.
        struct {int row, column;} temp = {0}; 
        while (temp.column <= i) 
            temp.column += columns[temp.row++];
        temp.column = i - (temp.column - columns[--temp.row]);

        enum { OBJECT_ID_BUFFER_SIZE = sizeof(BOARD_BUTTONS_ID "%02d-%02d") };
        char objectIdBuffer[OBJECT_ID_BUFFER_SIZE]; // It doesnt need to persist, gtk_widget_class_bind_template_child_full will call stddup() on it

        snprintf(objectIdBuffer, OBJECT_ID_BUFFER_SIZE, BOARD_BUTTONS_ID "%02d-%02d", temp.row, temp.column);
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

extern GtkWidget *checkers_board_new(void) {
    return g_object_new(CHECKERS_TYPE_BOARD, NULL);
}

