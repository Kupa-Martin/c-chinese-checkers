#include "../includes.h"

enum SignalId {
    SIGNAL_PLAYERS_SELECTED,
    N_SIGNALS
};

// Type definitions
struct _CheckersPlayerSelect {
    GtkWindow parent_instance;
};

struct _CheckersPlayerSelectClass {
    GtkBoxClass parent_class;

};
// End type definitions

// Forward declarations
G_DEFINE_TYPE(CheckersPlayerSelect, checkers_player_select, GTK_TYPE_BOX);

static void checkers_player_select_init(CheckersPlayerSelect *);
static void checkers_player_select_class_init(CheckersPlayerSelectClass *);
static void checkers_player_select_dispose(GObject *);
static void checkers_player_select_finalize(GObject *);
static inline void handle_checkers_player_select_clicked_button1(GtkButton *, gpointer);
static inline void handle_checkers_player_select_clicked_button2(GtkButton *, gpointer);
static inline void handle_checkers_player_select_clicked_button3(GtkButton *, gpointer);
// End forward declarations

// Global variables
static guint signals[N_SIGNALS];
// End global variables

static void checkers_player_select_init(CheckersPlayerSelect *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/CheckersPlayerSelect.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    
    g_object_unref(cssProvider);
    return;
}

static void checkers_player_select_class_init(CheckersPlayerSelectClass *klass) {
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);
    GtkWidgetClass *widgetClass = GTK_WIDGET_CLASS(klass);

    objectClass->dispose = checkers_player_select_dispose;
    objectClass->finalize = checkers_player_select_finalize;
    
    signals[SIGNAL_PLAYERS_SELECTED] = g_signal_new(
                                            "players-selected",
                                            G_TYPE_FROM_CLASS(klass),
                                            G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
                                            0,
                                            NULL,
                                            NULL,
                                            NULL,
                                            G_TYPE_NONE,
                                            1,
                                            CHECKERS_TYPE_BOARD_PLAYERS
                                        );
    
    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/CheckersPlayerSelect.ui");
    gtk_widget_class_bind_template_callback(widgetClass, handle_checkers_player_select_clicked_button1);
    gtk_widget_class_bind_template_callback(widgetClass, handle_checkers_player_select_clicked_button2);
    gtk_widget_class_bind_template_callback(widgetClass, handle_checkers_player_select_clicked_button3);
    return;
}

static void checkers_player_select_dispose(GObject *self) {
    gtk_widget_dispose_template	(GTK_WIDGET(self), CHECKERS_TYPE_PLAYER_SELECT);
    G_OBJECT_CLASS(checkers_player_select_parent_class)->dispose(self);
    return;
}

static void checkers_player_select_finalize(GObject *self) {
    G_OBJECT_CLASS(checkers_player_select_parent_class)->finalize(self);
    return;
}

static inline void handle_checkers_player_select_clicked_button1(GtkButton *button1, gpointer data) {
    return g_signal_emit(
                gtk_widget_get_parent(GTK_WIDGET(button1)),
                signals[SIGNAL_PLAYERS_SELECTED],
                0,
                CHECKERS_BOARD_2_PLAYERS,
                NULL
            ); 
}

static inline void handle_checkers_player_select_clicked_button2(GtkButton *button2, gpointer data) {
    return g_signal_emit(
                gtk_widget_get_parent(GTK_WIDGET(button2)),
                signals[SIGNAL_PLAYERS_SELECTED],
                0,
                CHECKERS_BOARD_4_PLAYERS,
                NULL
            ); 
}

static inline void handle_checkers_player_select_clicked_button3(GtkButton *button3, gpointer data) {
    return g_signal_emit(
                gtk_widget_get_parent(GTK_WIDGET(button3)),
                signals[SIGNAL_PLAYERS_SELECTED],
                0,
                CHECKERS_BOARD_6_PLAYERS,
                NULL
            );         
}

extern GtkWidget *checkers_player_select_new(void) {
    return g_object_new(CHECKERS_TYPE_PLAYER_SELECT, NULL);
}
