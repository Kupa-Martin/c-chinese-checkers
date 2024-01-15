#include "MainMenuWindow.h"
#include "CheckersWindow.h"

// Type definitions
struct _MainMenuWindow {
    GtkWindow parent_instance;

    // Child widgets
    GtkButton *button1;
};

struct _MainMenuWindowClass {
    GtkWindowClass parent_class;
};
// End type definitions

// Forward declarations
G_DEFINE_TYPE(MainMenuWindow, main_menu_window, GTK_TYPE_WINDOW);

static void main_menu_window_init(MainMenuWindow *);
static void main_menu_window_class_init(MainMenuWindowClass *);
static void main_menu_window_dispose(GObject *);
static void main_menu_window_finalize(GObject *);
static void handle_main_menu_window_clicked_button1(GtkButton *, gpointer);
// End forward declarations

static void main_menu_window_init(MainMenuWindow *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/MainMenuWindow.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    
    g_object_unref(cssProvider);
    return;
}

static void main_menu_window_class_init(MainMenuWindowClass *klass) {
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);
    GtkWidgetClass *widgetClass = GTK_WIDGET_CLASS(klass);

    objectClass->dispose = main_menu_window_dispose;
    objectClass->finalize = main_menu_window_finalize;

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/MainMenuWindow.ui");

    gtk_widget_class_bind_template_child(widgetClass, MainMenuWindow, button1);
    gtk_widget_class_bind_template_callback(widgetClass, handle_main_menu_window_clicked_button1);
    return;
}

static void main_menu_window_dispose(GObject *self) {
    gtk_widget_dispose_template	(GTK_WIDGET(self), MAIN_MENU_TYPE_WINDOW);
    G_OBJECT_CLASS(main_menu_window_parent_class)->dispose(self);
    return;
}

static void main_menu_window_finalize(GObject *self) {
    G_OBJECT_CLASS(main_menu_window_parent_class)->finalize(self);
    return;
}

static void handle_main_menu_window_clicked_button1(GtkButton *button1, gpointer data) {
    GListModel *windowList = gtk_window_get_toplevels();
    GtkWindow *self = GTK_WINDOW(g_list_model_get_item(windowList, 0));
    GtkApplication *app = gtk_window_get_application(self);
    GtkWindow *gameWindow = GTK_WINDOW(g_object_new(CHECKERS_TYPE_WINDOW, NULL));
    gtk_window_set_application(gameWindow, app);
    gtk_window_close(self);
    gtk_window_present(gameWindow);
}

extern GtkWidget *main_menu_window_new(void) {
    return g_object_new(MAIN_MENU_TYPE_WINDOW, NULL);
}
