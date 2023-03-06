#include "../includes.h"

enum {
    PROPERTY_INITIAL_STATE,
    N_SLOTS = 121
}

// Type definitions

struct _BoardWindow {
    GtkWindow parent_instance;

    BoardWindowInitialState initialState;
    BoardButton *slots[N_SLOTS];
};

struct _BoardWindowClass {
    GtkWindowClass parent_class;
};

// End type definitions

// Forward declaration
G_DEFINE_TYPE(BoardWindow, board_window, GTK_TYPE_WINDOW);

static void board_window_init(BoardWindow *);
static void board_window_class_init(BoardWindowClass *);
static void board_window_dispose(GObject *);
static void board_window_finalize(GObject *);
static void board_window_get_property(GObject *, gint, const GValue *, GParamSpec *);
static void board_window_set_property(GObject *, gint, GValue *, GParamSpec *);
// End forward declaration

static void board_window_init(BoardWindow *self) {
    self->initialState = BOARD_WINDOW_EMPTY_BOARD;
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/BoardWindow.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    g_object_unref(cssProvider);

    enum {
        ETYS = BOARD_BUTTON_EMPTY_SLOT,
        REDS = BOARD_BUTTON_RED_SLOT,
        REDB = BOARD_BUTTON_RED_BALL,
        BLUS = BOARD_BUTTON_BLUE_SLOT,
        BLUB = BOARD_BUTTON_BLUE_BALL,
        GRES = BOARD_BUTTON_GREEN_SLOT,
        GREB = BOARD_BUTTON_GREEN_BALL,
        BLKS = BOARD_BUTTON_BLACK_SLOT,
        BLKB = BOARD_BUTTON_BLACK_BALL,
        YLWS = BOARD_BUTTON_YELLOW_SLOT,
        YLWB = BOARD_BUTTON_YELLOW_BALL,
        WHTS = BOARD_BUTTON_WHITE_SLOT,
        WHTB = BOARD_BUTTON_WHITE_BALL
    };
    
    switch (self->initialState) {
        case BOARD_WINDOW_EMPTY_BOARD: {
            BoardButtonSource sources[] = {
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
            };
            for (size_t i= 0; i < N_SLOTS; i++) board_button_set_source(self->slots[i], sources[i]);
            break;
        }
    }
    return;
}

static void board_window_class_init(BoardWindowClass *klass){
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->set_property = board_window_set_property;
    objectClass->get_property = board_window_get_property;
    objectClass->dispose = board_window_dispose;
    objectClass->finalize = board_window_finalize;

    g_object_class_install_property(objectClass, PROPERTY_INITIAL_STATE, 
                                    g_param_spec_enum(
                                            "initial-state", 
                                            "initial-state", 
                                            "The initial state on the board", 
                                            BOARD_TYPE_WINDOW_INITIAL_STATE, 
                                            BOARD_WINDOW_EMPTY_BOARD, 
                                            G_PARAM_READWRITE
                                        )
                                    );

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/BoardWindow.ui");

    for (size_t i= 0; i < N_SLOTS; i++) {
        const char objectId[] = ""; // todo
        gtk_widget_class_bind_template_child_full(widgetClass, /* posible fuck up --> */ objectId, FALSE, G_STRUCT_OFFSET(BoardWindow, slots)+((glong)(i * sizeof(BoardButton *))));
    }
    return;
}