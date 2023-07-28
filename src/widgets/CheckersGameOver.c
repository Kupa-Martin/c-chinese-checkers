#include "../includes.h"

struct _CheckersGameOver {
    GtkBox parent_instance;
};

struct _CheckersGameOverClass {
    GtkBoxClass parent_class;
};


G_DEFINE_TYPE(CheckersGameOver, checkers_game_over, GTK_TYPE_BOX);

static void checkers_game_over_init(CheckersGameOver *);
static void checkers_game_over_class_init(CheckersGameOverClass *);
static void checkers_game_over_dispose(GObject *);
static void checkers_game_over_finalize(GObject *);

static void checkers_game_over_init(CheckersGameOver *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/CheckersGameOver.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    g_object_unref(cssProvider);
    return;
}

static void checkers_game_over_class_init(CheckersGameOverClass *klass) {
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->dispose = checkers_game_over_dispose;
    objectClass->finalize = checkers_game_over_finalize;

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/CheckersGameOver.ui");
    return;
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

extern GtkWidget *checkers_game_over_new(void) {
    return g_object_new(CHECKERS_TYPE_GAME_OVER, NULL);
}