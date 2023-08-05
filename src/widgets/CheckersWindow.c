#include "../includes.h"

enum PropertyId {
    PROPERTY_INITIAL_STATE = 1
};

// Type definitions
struct _CheckersWindow {
    GtkWindow parent_instance;

    // Properties
    CheckersBoardPlayers initialState; /* property drilling, also this is a game property */

    /* Future game object */

    // Child widgets
    GtkBox *sidebar;
    CheckersBoard *board;
};

struct _CheckersWindowClass {
    GtkWindowClass parent_class;
};

// End type definitions

// Forward declaration
G_DEFINE_TYPE(CheckersWindow, checkers_window, GTK_TYPE_WINDOW);

static void checkers_window_init(CheckersWindow *);
static void checkers_window_class_init(CheckersWindowClass *);
static void checkers_window_dispose(GObject *);
static void checkers_window_finalize(GObject *);
static void checkers_window_get_property(GObject *, guint, GValue *, GParamSpec *);
static void checkers_window_set_property(GObject *, guint, const GValue *, GParamSpec *);
static void handle_checkers_player_select_playersSelected_playerSelect1(CheckersPlayerSelect *, CheckersBoardPlayers, gpointer);
// End forward declaration

static void checkers_window_init(CheckersWindow *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/CheckersWindow.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	GtkAlertDialog *testDialog = gtk_alert_dialog_new("test dialog");
	gtk_alert_dialog_set_modal(testDialog, true);
	gtk_alert_dialog_show(testDialog, GTK_WINDOW(self));
    
    g_object_unref(cssProvider);
    return;
}

static void checkers_window_class_init(CheckersWindowClass *klass){
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->set_property = checkers_window_set_property;
    objectClass->get_property = checkers_window_get_property;
    objectClass->dispose = checkers_window_dispose;
    objectClass->finalize = checkers_window_finalize;

    g_object_class_install_property(objectClass, PROPERTY_INITIAL_STATE, 
                                g_param_spec_enum(
                                        "players", 
                                        "players", 
                                        "Value to be passed on to CheckersBoard.players", 
                                        CHECKERS_TYPE_BOARD_PLAYERS, 
                                        CHECKERS_BOARD_EMPTY_BOARD, 
                                        G_PARAM_READWRITE
                                    )
                                );

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/CheckersWindow.ui");
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, sidebar);
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, board);
    // gtk_widget_class_bind_template_callback(widgetClass, handle_checkers_player_select_playersSelected_playerSelect1);
    return;
}

static void checkers_window_dispose(GObject *object) {
    gtk_widget_dispose_template(GTK_WIDGET(object), CHECKERS_TYPE_WINDOW);
    G_OBJECT_CLASS(checkers_window_parent_class)->dispose(object);
    return;
}

static void checkers_window_finalize(GObject *object) {
    G_OBJECT_CLASS(checkers_window_parent_class)->finalize(object);
    return;
}

static void checkers_window_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec) {
    CheckersWindow *self = CHECKERS_WINDOW(object);

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

static void checkers_window_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    CheckersWindow *self = CHECKERS_WINDOW(object);
    
    switch (propertyId) {
        case PROPERTY_INITIAL_STATE: {
            CheckersBoardPlayers initialState = g_value_get_enum(value);
            self->initialState = initialState;
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void handle_checkers_player_select_playersSelected_playerSelect1(CheckersPlayerSelect *playerSelect1, CheckersBoardPlayers players, gpointer args) {
    g_message("players: %d\n", players);
    
}


extern GtkWidget *checkers_window_new(void) {
    return g_object_new(CHECKERS_TYPE_WINDOW, NULL);
}
