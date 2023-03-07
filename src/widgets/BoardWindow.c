#include "../includes.h"

enum {
    N_SLOTS = 121
};

enum /* Property id's */ {
    PROPERTY_INITIAL_STATE = 1
};

// Type definitions

struct XY {
    size_t x;
    size_t y;
};

struct _BoardWindow {
    GtkWindow parent_instance;

    BoardWindowInitialState initialState;
    GtkLabel *label;
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
static void board_window_get_property(GObject *, guint, GValue *, GParamSpec *);
static void board_window_set_property(GObject *, guint, const GValue *, GParamSpec *);
static void board_window_constructed(GObject *);
static inline struct XY toXY(size_t); // private conversion function
// End forward declaration

static void board_window_init(BoardWindow *self) {
    self->initialState = BOARD_WINDOW_EMPTY_BOARD;
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/BoardWindow.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    g_object_unref(cssProvider);
    return;
}

static void board_window_class_init(BoardWindowClass *klass){
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->set_property = board_window_set_property;
    objectClass->get_property = board_window_get_property;
    objectClass->constructed = board_window_constructed;
    objectClass->dispose = board_window_dispose;
    objectClass->finalize = board_window_finalize;

    g_object_class_install_property(objectClass, PROPERTY_INITIAL_STATE, 
                                    g_param_spec_enum(
                                            "initial-state", 
                                            "initial-state", 
                                            "The initial state on the board", 
                                            BOARD_TYPE_WINDOW_INITIAL_STATE, 
                                            BOARD_WINDOW_EMPTY_BOARD, 
                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
                                        )
                                    );

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/BoardWindow.ui");
    gtk_widget_class_bind_template_child(widgetClass, BoardWindow, label);
    for (size_t i= 0; i < N_SLOTS; i++) {
        struct XY temp = toXY(i);
        char objectId[17]; // should it persist?
        snprintf(objectId, 17, "boardbutton%02zu-%02zu", temp.x, temp.y);
        gtk_widget_class_bind_template_child_full(widgetClass, /* posible fuck up --> */ objectId, FALSE, G_STRUCT_OFFSET(BoardWindow, slots)+((glong)(i * sizeof(BoardButton *))));
    }
    return;
}

static void board_window_dispose(GObject *object) {
    gtk_widget_dispose_template(GTK_WIDGET(object), BOARD_TYPE_WINDOW);
    G_OBJECT_CLASS(board_window_parent_class)->dispose(object);
    return;
}

static void board_window_finalize(GObject *object) {
    G_OBJECT_CLASS(board_window_parent_class)->finalize(object);
    return;
}

static void board_window_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec) {
    BoardWindow *self = BOARD_WINDOW(object);

    switch (propertyId) {
        case PROPERTY_INITIAL_STATE: {
            g_value_set_enum(value, self->initialState);
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void board_window_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    BoardWindow *self = BOARD_WINDOW(object);
    
    switch (propertyId) {
        case PROPERTY_INITIAL_STATE: {
            BoardWindowInitialState initialState = g_value_get_enum(value);
            self->initialState = initialState;
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void board_window_constructed(GObject *object) {    
    BoardWindow *self = BOARD_WINDOW(object);
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
    BoardButtonSource *sources;
    switch (self->initialState) {
        case BOARD_WINDOW_EMPTY_BOARD: 
            sources = (BoardButtonSource[N_SLOTS]){
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
            goto init_board_buttons;
        
        case BOARD_WINDOW_2_PLAYERS: 
            sources = (BoardButtonSource[N_SLOTS]){
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
            };
            goto init_board_buttons;
        
        case BOARD_WINDOW_4_PLAYERS: 
            sources = (BoardButtonSource[N_SLOTS]){
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
            };
            goto init_board_buttons;
        
        case BOARD_WINDOW_6_PLAYERS: 
            sources = (BoardButtonSource[N_SLOTS]){
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
            };
            goto init_board_buttons;

        init_board_buttons: 
        for (size_t i= 0; i < N_SLOTS; i++) board_button_set_source(self->slots[i], sources[i]);
    }
    G_OBJECT_CLASS(board_window_parent_class)->constructed(object);
}

#define BETWEEN(num, min, max) (((num) >= (min)) && ((num) <= (max)))
static inline struct XY toXY(size_t num) {
    if (num == 0) return (struct XY){0, num - 0};
    else if (BETWEEN(num, 1, 2)) return (struct XY){1, num - 1};
    else if (BETWEEN(num, 3, 5)) return (struct XY){2, num - 3};
    else if (BETWEEN(num, 6, 9)) return (struct XY){3, num - 6};
    else if (BETWEEN(num, 10, 22)) return (struct XY){4, num - 10};
    else if (BETWEEN(num, 23, 34)) return (struct XY){5, num - 23};
    else if (BETWEEN(num, 35, 45)) return (struct XY){6, num - 35};
    else if (BETWEEN(num, 46, 55)) return (struct XY){7, num - 46};
    else if (BETWEEN(num, 56, 64)) return (struct XY){8, num - 56};
    else if (BETWEEN(num, 65, 74)) return (struct XY){9, num - 65};
    else if (BETWEEN(num, 75, 85)) return (struct XY){10, num - 75};
    else if (BETWEEN(num, 86, 97)) return (struct XY){11, num - 86};
    else if (BETWEEN(num, 98, 110)) return (struct XY){12, num - 98};
    else if (BETWEEN(num, 111, 114)) return (struct XY){13, num - 111};
    else if (BETWEEN(num, 115, 117)) return (struct XY){14, num - 115};
    else if (BETWEEN(num, 118, 119)) return (struct XY){15, num - 118};
    else if (num == 120) return (struct XY){16, num - 120};
    else g_abort(); // this should never happen
}
#undef BETWEEN

extern GtkWidget *board_window_new(void) {
    return g_object_new(BOARD_TYPE_WINDOW, NULL);
}