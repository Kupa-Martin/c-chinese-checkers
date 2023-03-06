#include "includes.h"

#define DEBUG

void handle_gtk_application_activate_app(GtkApplication *app, gpointer userData) {
    #ifdef DEBUG
    BoardButton *test= BOARD_BUTTON(board_button_new());
    #endif
    g_type_ensure(BOARD_TYPE_BUTTON);
    g_type_ensure(MAIN_MENU_TYPE_WINDOW);
    GtkBuilder *builder = gtk_builder_new_from_resource("/com/fullaccess/ChineseCheckers/ui/markup/app.ui");
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/global.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(window)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_window_set_application(window, app);

    gtk_widget_show(GTK_WIDGET(window));
    g_object_unref(builder);
    g_object_unref(cssProvider);
    return;
}

int main(int argc, char const *argv[]) {
    GtkApplication *app= gtk_application_new("com.fullaccess.ChineseCheckers", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(handle_gtk_application_activate_app), NULL);
    int status= g_application_run(G_APPLICATION(app), argc, (char **)argv);
    g_object_unref(app);
    return status;
}