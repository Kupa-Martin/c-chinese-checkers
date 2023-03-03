#include "../includes.h"

enum {
    PROPERTY_SOURCE = 1
};

// Type definitions
struct _BoardButton {
    GtkButton parent_instance;

    BoardButtonSource source;
    GtkImage *image;
};

struct _BoardButtonClass {
    GtkButtonClass parent_class;
};
// End type definitions

// Forward declarations
G_DEFINE_TYPE(BoardButton, board_button, GTK_TYPE_BUTTON);
static void board_button_dispose(GObject *);
static void board_button_finalize(GObject *);
static void board_button_get_property(GObject *, guint, GValue *, GParamSpec *);
static void board_button_set_property(GObject *, guint, const GValue *, GParamSpec *);
static void handle_board_button_clicked(BoardButton *, gpointer);
// End forward declarations

static void board_button_init(BoardButton *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    self->source = BOARD_BUTTON_EMPTY_SLOT;
    gtk_image_set_from_resource(self->image, "/com/fullaccess/ChineseCheckers/ui/assets/empty_slot.png");

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/ui/styles/BoardButton.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    
    g_object_unref(cssProvider);
    return;
}

static void board_button_class_init(BoardButtonClass *klass) {
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->set_property = board_button_set_property;
    objectClass->get_property = board_button_get_property;
    objectClass->dispose = board_button_dispose;
    objectClass->finalize = board_button_finalize;

    g_object_class_install_property(objectClass, PROPERTY_SOURCE, 
                                    g_param_spec_enum(
                                            "source", 
                                            "source", 
                                            "The source of the button's image", 
                                            BOARD_TYPE_BUTTON_SOURCE, 
                                            BOARD_BUTTON_EMPTY_SLOT, 
                                            G_PARAM_READWRITE
                                        )
                                    );

    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/ui/markup/BoardButton.ui");
    gtk_widget_class_bind_template_child(widgetClass, BoardButton, image);
    gtk_widget_class_bind_template_callback(widgetClass, handle_board_button_clicked);
    return;
}

static void board_button_dispose(GObject *object) {
    gtk_widget_dispose_template(GTK_WIDGET(object), BOARD_TYPE_BUTTON);
    G_OBJECT_CLASS(board_button_parent_class)->dispose(object);
    return;
}

static void board_button_finalize(GObject *object) {
    G_OBJECT_CLASS(board_button_parent_class)->finalize(object);
    return;
}

static void board_button_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec) {
    BoardButton *self = BOARD_BUTTON(object);

    switch (propertyId) {
        case PROPERTY_SOURCE: {
            g_value_set_enum(value, self->source);
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void board_button_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    BoardButton *self = BOARD_BUTTON(object);
    
    switch (propertyId) {
        case PROPERTY_SOURCE: {
            const char const *resources[] = {
                "/com/fullaccess/ChineseCheckers/ui/assets/empty_slot.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/red_empty_slot.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/red_ball.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/blue_empty_slot.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/blue_ball.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/green_empty_slot.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/green_ball.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/black_empty_slot.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/black_ball.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/yellow_empty_slot.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/yellow_ball.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/white_empty_slot.png",
                "/com/fullaccess/ChineseCheckers/ui/assets/white_ball.png"
            };
            BoardButtonSource source = g_value_get_enum(value);
            gtk_image_set_from_resource(self->image, resources[source]);
            self->source = source;
            break;
        }
        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
            break;
        }
    }
}

static void handle_board_button_clicked(BoardButton *self, gpointer data) {
    board_button_set_source(self, (board_button_get_source(self)+1)%BOARD_BUTTON_N_SOURCES);
    return;
}

extern GtkWidget *board_button_new(void) {
    return g_object_new(BOARD_TYPE_BUTTON, NULL);
}

extern BoardButtonSource board_button_get_source(BoardButton *self) {
    BoardButtonSource hold;
    g_object_get(self, "source", &hold, NULL);
    return hold;
}

extern void board_button_set_source(BoardButton *self, BoardButtonSource value) {
    g_object_set(self, "source", value, NULL);
    return;
}
