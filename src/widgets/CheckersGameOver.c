#include "CheckersGameOver.h"

enum PropertyId {
    WINNER_PROPERTY = 1,
    N_PROPERTIES
};

struct _CheckersGameOver {
    GtkBox parent_instance;

    CheckersTeam winner;
};

struct _CheckersGameOverClass {
    GtkBoxClass parent_class;
};

G_DEFINE_TYPE(CheckersGameOver, checkers_game_over, GTK_TYPE_BOX);

static GParamSpec *properties[N_PROPERTIES];

static void checkers_game_over_init(CheckersGameOver *);
static void checkers_game_over_class_init(CheckersGameOverClass *);
static void checkers_game_over_dispose(GObject *);
static void checkers_game_over_finalize(GObject *);
static void checkers_game_over_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec);
static void checkers_game_over_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec);
static gchararray checkers_game_over_resolve_label(CheckersGameOver *self);
static void checkers_game_over_handle_button1_clicked(GtkButton *button1, gpointer data);

static void checkers_game_over_init(CheckersGameOver *self) {
    self->winner = CHECKERS_NO_TEAM;
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/resources/styles/CheckersGameOver.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    g_object_unref(cssProvider);
    return;
}

static void checkers_game_over_class_init(CheckersGameOverClass *klass) {
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->dispose = checkers_game_over_dispose;
    objectClass->finalize = checkers_game_over_finalize;
    objectClass->set_property = checkers_game_over_set_property;
    objectClass->get_property = checkers_game_over_get_property;
    
    properties[WINNER_PROPERTY] = g_param_spec_enum(
        "winner",
        "winner",
        "The winner of the game",
        CHECKERS_TYPE_TEAM,
        CHECKERS_NO_TEAM,
        G_PARAM_READWRITE
    );
    g_object_class_install_properties(objectClass, N_PROPERTIES, properties);
    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/resources/markup/CheckersGameOver.ui");
    gtk_widget_class_bind_template_callback(widgetClass, checkers_game_over_resolve_label);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_game_over_handle_button1_clicked);
    return;
}

static void checkers_game_over_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec) {
    CheckersGameOver *self = CHECKERS_GAME_OVER(object);

    switch (propertyId) {
        case WINNER_PROPERTY: {
            g_value_set_enum(value, (gint)self->winner);
            return;
        } 
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static void checkers_game_over_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    CheckersGameOver *self = CHECKERS_GAME_OVER(object);

    switch (propertyId) {
        case WINNER_PROPERTY: {
            CheckersTeam winner = (CheckersTeam)g_value_get_enum(value);
            self->winner = winner;
            return;
        } 
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static void checkers_game_over_dispose(GObject *object) {
    gtk_widget_dispose_template(GTK_WIDGET(object), CHECKERS_TYPE_GAME_OVER);
    G_OBJECT_CLASS(checkers_game_over_parent_class)->dispose(object);
    return;
}

static void checkers_game_over_finalize(GObject *object) {
    G_OBJECT_CLASS(checkers_game_over_parent_class)->finalize(object);
    return;
}

static gchararray checkers_game_over_resolve_label(CheckersGameOver *self) {
#define TEAM_HASH(team) ((team) >> 1) 
    static const char *const messages[] = {
        [TEAM_HASH(CHECKERS_RED_TEAM)] = "Red",
        [TEAM_HASH(CHECKERS_BLACK_TEAM)] = "Black",
        [TEAM_HASH(CHECKERS_BLUE_TEAM)] = "Blue",
        [TEAM_HASH(CHECKERS_GREEN_TEAM)] = "Green",
        [TEAM_HASH(CHECKERS_WHITE_TEAM)] = "White",
        [TEAM_HASH(CHECKERS_YELLOW_TEAM)] = "Yellow",
        [TEAM_HASH(CHECKERS_NO_TEAM)] = "No one"
    };
    return g_strdup_printf("%s won!", messages[TEAM_HASH(self->winner)]);
#undef TEAM_HASH
}

static void checkers_game_over_handle_button1_clicked(GtkButton *button1, gpointer data) {
    GtkWidget *w = gtk_widget_get_ancestor(GTK_WIDGET(button1), CHECKERS_TYPE_GAME_OVER);
    CheckersGameOver *self = CHECKERS_GAME_OVER(w);
    g_assert(self != NULL);
    gtk_widget_set_visible(GTK_WIDGET(self), false);
}

extern GtkWidget *checkers_game_over_new(void) {
    return g_object_new(CHECKERS_TYPE_GAME_OVER, NULL);
}

extern CheckersTeam checkers_game_over_get_winner(CheckersGameOver *self) {
    CheckersTeam hold; 
    g_object_get(self, "winner", &hold, NULL);
    return hold;
}

extern void checkers_game_over_set_winner(CheckersGameOver *self, CheckersTeam winner) {
    g_object_set(self, "winner", winner, NULL);
}