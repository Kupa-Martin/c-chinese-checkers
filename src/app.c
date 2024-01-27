#include "checkers.h"
#include <gtk/gtk.h>

void handle_gtk_application_activate_app(GtkApplication *app, gpointer userData);

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.fullaccess.ChineseCheckers", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(handle_gtk_application_activate_app), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

void handle_gtk_application_activate_app(GtkApplication *app, gpointer userData) {
    g_type_ensure(MAIN_MENU_TYPE_WINDOW);
    g_type_ensure(CHECKERS_TYPE_SLOT);
    g_type_ensure(CHECKERS_TYPE_WINDOW);
    g_type_ensure(CHECKERS_TYPE_GAME_OVER);
    g_type_ensure(CHECKERS_TYPE_BOARD);
    GtkBuilder *builder = gtk_builder_new_from_resource("/com/fullaccess/ChineseCheckers/resources/markup/app.ui");
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/resources/styles/global.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(window)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_window_set_application(window, app);

    gtk_window_present(window);
    g_object_unref(builder);
    g_object_unref(cssProvider);
    return;
}
