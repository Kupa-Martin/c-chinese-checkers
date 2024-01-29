#include "CheckersWindow.h"
#include "CheckersBoard.h"
#include "CheckersGameOver.h"
#include <assert.h>




// Type definitions
struct _CheckersWindow {
    GtkWindow parent_instance;

    /* Future game object */

    // Child widgets
    GtkBox *sidebar;
    CheckersBoard *board;
    CheckersGameOver *over;
    GtkToggleButton *select2;
    GtkToggleButton *select4;
    GtkToggleButton *select6;
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
static CheckersPlayers checkers_window_change_board_players(CheckersWindow *, gboolean, gboolean, gboolean);
static void checkers_window_handle_button1_clicked(GtkButton *, gpointer);
static gboolean checkers_window_closure_player_select(CheckersWindow *, gboolean, gboolean);
static gchararray checkers_window_resolve_button1_label(CheckersWindow *self, gboolean isGameActive);
static void checkers_window_handle_game_over(CheckersBoard *board, CheckersTeam winner, gpointer data);
static gboolean checkers_window_control_button1_sensitivity(CheckersWindow *self, gboolean select2Active, gboolean select4Active, gboolean select6Active);
// End forward declaration

static void checkers_window_init(CheckersWindow *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/resources/styles/CheckersWindow.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
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

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/resources/markup/CheckersWindow.ui");
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, sidebar);
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, board);
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, over);
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, select2);
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, select4);
    gtk_widget_class_bind_template_child(widgetClass, CheckersWindow, select6);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_window_change_board_players);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_window_handle_button1_clicked);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_window_closure_player_select);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_window_handle_game_over);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_window_resolve_button1_label);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_window_control_button1_sensitivity);
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
    // CheckersWindow *self = CHECKERS_WINDOW(object);

    switch (propertyId) {
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void checkers_window_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    // CheckersWindow *self = CHECKERS_WINDOW(object);
    
    switch (propertyId) {

        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static CheckersPlayers checkers_window_change_board_players(CheckersWindow *self, gboolean players2, gboolean players4, gboolean players6) {
    assert(!(players2 && players4) || !(players2 && players6) || !(players4 && players6));
    if (players2) 
        return CHECKERS_2_PLAYERS;
    if (players4) 
        return CHECKERS_4_PLAYERS;
    if (players6)
        return CHECKERS_6_PLAYERS;
    return CHECKERS_NO_PLAYERS;
}

static void checkers_window_handle_button1_clicked(GtkButton *self, gpointer data) {
    GtkRoot *r = gtk_widget_get_root(GTK_WIDGET(self));
    CheckersWindow *window = CHECKERS_WINDOW(r);
    CheckersSlot *selectedSlot = checkers_board_get_selected_slot(window->board);
    if (selectedSlot != NULL)
        checkers_board_unselect_slot(window->board);
    checkers_board_set_game_active(window->board, !checkers_board_is_game_active(window->board));
}

static gboolean checkers_window_closure_player_select(CheckersWindow *self, gboolean gameActive, gboolean gameOverVisible) {
    return !gameActive && !gameOverVisible;
}

static gchararray checkers_window_resolve_button1_label(CheckersWindow *self, gboolean isGameActive) {
    return g_strdup(isGameActive ? "End" : "Start");
}

static void checkers_window_handle_game_over(CheckersBoard *board, CheckersTeam winner, gpointer data) {
    g_assert(board != NULL && winner >= 0 && winner < CHECKERS_NO_TEAM);
    GtkWidget *w = gtk_widget_get_ancestor(GTK_WIDGET(board), CHECKERS_TYPE_WINDOW);
    CheckersWindow *window = CHECKERS_WINDOW(w);
    gtk_toggle_button_set_active(window->select2, false);
    gtk_toggle_button_set_active(window->select4, false);
    gtk_toggle_button_set_active(window->select6, false);
    checkers_game_over_set_winner(window->over, winner);
    gtk_widget_set_visible(GTK_WIDGET(window->over), true);
}

static gboolean checkers_window_control_button1_sensitivity(CheckersWindow *self, gboolean select2Active, gboolean select4Active, gboolean select6Active) {
    return select2Active || select4Active || select6Active;
}

extern GtkWidget *checkers_window_new(void) {
    return g_object_new(CHECKERS_TYPE_WINDOW, NULL);
}
