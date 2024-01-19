#include "CheckersBoardButton.h"
#include "CheckersBoard.h"
#include "../macro_utils.h"
#include <stddef.h>

enum PropertyId {
    PROPERTY_SOURCE = 1,
    PROPERTY_RADIUS
};

// Type definitions
struct _CheckersBoardButton {
    GtkButton parent_instance;

    // Properties
    CheckersBoardButtonSource source;
    gint radius;

    // Child widgets
    GtkImage *image;
};

struct _CheckersBoardButtonClass {
    GtkButtonClass parent_class;
};
// End type definitions

// Forward declarations
G_DEFINE_TYPE(CheckersBoardButton, checkers_board_button, GTK_TYPE_BUTTON);
static void checkers_board_button_init(CheckersBoardButton *);
static void checkers_board_button_class_init(CheckersBoardButtonClass *);
static void checkers_board_button_dispose(GObject *);
static void checkers_board_button_finalize(GObject *);
static void checkers_board_button_get_property(GObject *, guint, GValue *, GParamSpec *);
static void checkers_board_button_set_property(GObject *, guint, const GValue *, GParamSpec *);
static void checkers_board_button_handle_clicked_self(CheckersBoardButton *, gpointer);
static gchararray checkers_board_button_source_to_resource(CheckersBoardButton *, CheckersBoardButtonSource);
// End forward declarations

static void checkers_board_button_init(CheckersBoardButton *self) {
    self->source = CHECKERS_BOARD_BUTTON_EMPTY_SLOT;
    self->radius = 32;
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/resources/styles/CheckersBoardButton.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    
    g_object_unref(cssProvider);
    return;
}

static void checkers_board_button_class_init(CheckersBoardButtonClass *klass) {
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->set_property = checkers_board_button_set_property;
    objectClass->get_property = checkers_board_button_get_property;
    objectClass->dispose = checkers_board_button_dispose;
    objectClass->finalize = checkers_board_button_finalize;

    g_object_class_install_property(objectClass, PROPERTY_SOURCE, 
                                    g_param_spec_enum(
                                            "source", 
                                            "source", 
                                            "The source of the button's image", 
                                            CHECKERS_TYPE_BOARD_BUTTON_SOURCE, 
                                            CHECKERS_BOARD_BUTTON_EMPTY_SLOT, 
                                            G_PARAM_READWRITE
                                        )
                                    );
    g_object_class_install_property(objectClass, PROPERTY_RADIUS, 
                                    g_param_spec_int(
                                            "radius",
                                            "radius",
                                            "The radius of the button",
                                            0,
                                            G_MAXINT,
                                            32,
                                            G_PARAM_READWRITE
                                        )
                                    );

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/resources/markup/CheckersBoardButton.ui");
    gtk_widget_class_bind_template_child(widgetClass, CheckersBoardButton, image);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_board_button_handle_clicked_self);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_board_button_source_to_resource);
    return;
}

static void checkers_board_button_dispose(GObject *object) {
    gtk_widget_dispose_template(GTK_WIDGET(object), CHECKERS_TYPE_BOARD_BUTTON);
    G_OBJECT_CLASS(checkers_board_button_parent_class)->dispose(object);
    return;
}

static void checkers_board_button_finalize(GObject *object) {
    G_OBJECT_CLASS(checkers_board_button_parent_class)->finalize(object);
    return;
}

static void checkers_board_button_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec) {
    CheckersBoardButton *self = CHECKERS_BOARD_BUTTON(object);

    switch (propertyId) {
        case PROPERTY_SOURCE: {
            g_value_set_enum(value, self->source);
            break;
        }
        case PROPERTY_RADIUS: {
            g_value_set_int(value, self->radius);
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void checkers_board_button_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    CheckersBoardButton *self = CHECKERS_BOARD_BUTTON(object);
    
    switch (propertyId) {
        case PROPERTY_SOURCE: {
            CheckersBoardButtonSource source = g_value_get_enum(value);
            self->source = source;
            break;
        }
        case PROPERTY_RADIUS: {
            gint radius = g_value_get_int(value);
            self->radius = radius;
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void checkers_board_button_handle_clicked_self(CheckersBoardButton *self, gpointer data) {
    GtkWidget *w = gtk_widget_get_ancestor(GTK_WIDGET(self), CHECKERS_TYPE_BOARD);
    CheckersBoard *owner = CHECKERS_BOARD(w);
    g_assert(owner != NULL);
    if (!checkers_board_is_game_active(owner)) 
        return;
    gtk_widget_grab_focus(GTK_WIDGET(self));
    checkers_board_button_set_source(self, (checkers_board_button_get_source(self)+1)%CHECKERS_BOARD_BUTTON_N_SOURCES);
    return;
}

static gchararray checkers_board_button_source_to_resource(CheckersBoardButton *self, CheckersBoardButtonSource source) {
    static const char *resources[] = {
        "/com/fullaccess/ChineseCheckers/resources/assets/empty_slot.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/red_empty_slot.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/red_marble.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/blue_empty_slot.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/blue_marble.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/green_empty_slot.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/green_marble.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/black_empty_slot.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/black_marble.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/yellow_empty_slot.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/yellow_marble.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/white_empty_slot.png",
        "/com/fullaccess/ChineseCheckers/resources/assets/white_marble.png"
    };
    g_assert(source < ARRAY_SIZE(resources));
    // Dont return a string literal, it crashes the app.
    return g_strdup_printf(resources[source]);
}

extern GtkWidget *checkers_board_button_new(void) {
    return g_object_new(CHECKERS_TYPE_BOARD_BUTTON, NULL);
}

extern CheckersBoardButtonSource checkers_board_button_get_source(CheckersBoardButton *self) {
    CheckersBoardButtonSource hold;
    g_object_get(self, "source", &hold, NULL);
    return hold;
}

extern void checkers_board_button_set_source(CheckersBoardButton *self, CheckersBoardButtonSource value) {
    g_object_set(self, "source", value, NULL);
    return;
}

