#include "CheckersSlot.h"
#include "CheckersBoard.h"
#include "macro_utils.h"
#include <stddef.h>

#define HIGHLIGHTED_CSS_CLASS "highlighted"
#define SELECTED_CSS_CLASS "selected"

enum PropertyId {
    PROPERTY_STATE = 1,
    PROPERTY_TEAM,
    PROPERTY_RADIUS,
    PROPERTY_ROW,
    PROPERTY_COLUMN,
    PROPERTY_HIGHLIGHTED,
    PROPERTY_SELECTED
};

// Type definitions
struct _CheckersSlot {
    GtkButton parent_instance;

    // Properties
    CheckersSlotState state;
    CheckersTeam team;
    gint radius;
    guint row;
    guint column;
    gboolean isHighlighted;
    gboolean isSelected;
    // Child widgets
    GtkImage *image;
};

struct _CheckersSlotClass {
    GtkButtonClass parent_class;
};
// End type definitions

// Forward declarations
G_DEFINE_TYPE(CheckersSlot, checkers_slot, GTK_TYPE_BUTTON);
static void checkers_slot_init(CheckersSlot *);
static void checkers_slot_class_init(CheckersSlotClass *);
static void checkers_slot_dispose(GObject *);
static void checkers_slot_finalize(GObject *);
static void checkers_slot_get_property(GObject *, guint, GValue *, GParamSpec *);
static void checkers_slot_set_property(GObject *, guint, const GValue *, GParamSpec *);
static void checkers_slot_handle_clicked_self(CheckersSlot *, gpointer);
static gchararray checkers_slot_resolve_resource(CheckersSlot *, CheckersSlotState);
// End forward declarations

static void checkers_slot_init(CheckersSlot *self) {
    self->state = CHECKERS_SLOT_EMPTY_SLOT;
    self->isHighlighted = false;
    self->isSelected = false;
    self->team = CHECKERS_NO_TEAM;
    self->radius = 32;
    self->row = 0;
    self->column = 0;
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/fullaccess/ChineseCheckers/resources/styles/CheckersSlot.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(GTK_WIDGET(self)), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    
    g_object_unref(cssProvider);
    return;
}

static void checkers_slot_class_init(CheckersSlotClass *klass) {
    GtkWidgetClass *widgetClass= GTK_WIDGET_CLASS(klass);
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);

    objectClass->set_property = checkers_slot_set_property;
    objectClass->get_property = checkers_slot_get_property;
    objectClass->dispose = checkers_slot_dispose;
    objectClass->finalize = checkers_slot_finalize;

    g_object_class_install_property(objectClass, PROPERTY_STATE, 
                                    g_param_spec_enum(
                                            "state", 
                                            "state", 
                                            "The state tracks which marbles (if any) are on this button", 
                                            CHECKERS_TYPE_SLOT_STATE, 
                                            CHECKERS_SLOT_EMPTY_SLOT, 
                                            G_PARAM_CONSTRUCT | G_PARAM_READWRITE
                                        )
                                    );
    g_object_class_install_property(objectClass, PROPERTY_TEAM, 
                                    g_param_spec_enum(
                                            "team", 
                                            "team", 
                                            "The team defines whether to consider the slot when determining game over", 
                                            CHECKERS_TYPE_TEAM, 
                                            CHECKERS_NO_TEAM, 
                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
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
    g_object_class_install_property(objectClass, PROPERTY_ROW, 
                                    g_param_spec_uint(
                                            "row",
                                            "row",
                                            "Used for easier navigation on the board",
                                            0,
                                            G_MAXUINT,
                                            0,
                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
                                        )
                                    );
    g_object_class_install_property(objectClass, PROPERTY_COLUMN, 
                                    g_param_spec_uint(
                                            "column",
                                            "column",
                                            "Used for easier navigation on the board",
                                            0,
                                            G_MAXUINT,
                                            0,
                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
                                        ) 
                                    );
    g_object_class_install_property(objectClass, PROPERTY_HIGHLIGHTED, 
                                    g_param_spec_boolean(
                                            "is-highlighted",
                                            "is-highlighted",
                                            "Whether the slot is highlighted as a valid move",
                                            false,
                                            G_PARAM_READWRITE 
                                        ) 
                                    );
    g_object_class_install_property(objectClass, PROPERTY_SELECTED, 
                                    g_param_spec_boolean(
                                            "is-selected",
                                            "is-selected",
                                            "Whether the slot is selected",
                                            false,
                                            G_PARAM_READWRITE 
                                        ) 
                                    );
    gtk_widget_class_set_template_from_resource(widgetClass, "/com/fullaccess/ChineseCheckers/resources/markup/CheckersSlot.ui");
    gtk_widget_class_bind_template_child(widgetClass, CheckersSlot, image);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_slot_handle_clicked_self);
    gtk_widget_class_bind_template_callback(widgetClass, checkers_slot_resolve_resource);
    return;
}

static void checkers_slot_dispose(GObject *object) {
    gtk_widget_dispose_template(GTK_WIDGET(object), CHECKERS_TYPE_SLOT);
    G_OBJECT_CLASS(checkers_slot_parent_class)->dispose(object);
    return;
}

static void checkers_slot_finalize(GObject *object) {
    G_OBJECT_CLASS(checkers_slot_parent_class)->finalize(object);
    return;
}

static void checkers_slot_get_property(GObject *object, guint propertyId, GValue *value, GParamSpec *paramSpec) {
    CheckersSlot *self = CHECKERS_SLOT(object);

    switch (propertyId) {
        case PROPERTY_STATE: {
            g_value_set_enum(value, (gint)self->state);
            return;
        }
        case PROPERTY_TEAM: {
            g_value_set_enum(value, (gint)self->team);
            return;
        }
        case PROPERTY_RADIUS: {
            g_value_set_int(value, (gint)self->radius);
            return;
        }
        case PROPERTY_ROW: {
            g_value_set_uint(value, self->row);
            return;
        }
        case PROPERTY_COLUMN: {
            g_value_set_uint(value, self->column);
            return;
        }
        case PROPERTY_HIGHLIGHTED: {
            g_value_set_boolean(value, self->isHighlighted);
            return;
        }
        case PROPERTY_SELECTED: {
            g_value_set_boolean(value, self->isSelected);
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static void checkers_slot_set_property(GObject *object, guint propertyId, const GValue *value, GParamSpec *paramSpec) {
    CheckersSlot *self = CHECKERS_SLOT(object);
    
    switch (propertyId) {
        case PROPERTY_STATE: {
            CheckersSlotState state = (CheckersSlotState)g_value_get_enum(value);
            self->state = state;
            return;
        }
        case PROPERTY_TEAM: {
            CheckersTeam team = (CheckersTeam)g_value_get_enum(value);
            self->team = team;
            return;
        }
        case PROPERTY_RADIUS: {
            gint radius = g_value_get_int(value);
            self->radius = radius;
            return;
        }
        case PROPERTY_ROW: {
            guint row = g_value_get_uint(value);
            self->row = row;
            return;
        }
        case PROPERTY_COLUMN: {
            guint column = g_value_get_uint(value);
            self->column = column;
            return;
        }
        case PROPERTY_HIGHLIGHTED: {
            gboolean isHighlighted = g_value_get_boolean(value);
            if (isHighlighted != self->isHighlighted) {
                if (isHighlighted)
                    gtk_widget_add_css_class(GTK_WIDGET(self), HIGHLIGHTED_CSS_CLASS);
                else
                    gtk_widget_remove_css_class(GTK_WIDGET(self), HIGHLIGHTED_CSS_CLASS);
            }
            self->isHighlighted = isHighlighted;
            return;
        }
        case PROPERTY_SELECTED: {
            gboolean isSelected = g_value_get_boolean(value);
            if (isSelected != self->isSelected) {
                if (isSelected)
                    gtk_widget_add_css_class(GTK_WIDGET(self), SELECTED_CSS_CLASS);
                else
                    gtk_widget_remove_css_class(GTK_WIDGET(self), SELECTED_CSS_CLASS);
            }
            self->isSelected = isSelected;
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propertyId, paramSpec);
}

static void checkers_slot_handle_clicked_self(CheckersSlot *self, gpointer data) {
    GtkWidget *w = gtk_widget_get_ancestor(GTK_WIDGET(self), CHECKERS_TYPE_BOARD);
    CheckersBoard *owner = CHECKERS_BOARD(w);
    g_assert(owner != NULL);
    if (!checkers_board_is_game_active(owner)) 
        return;
    gboolean selfHasMarble = CHECKERS_SLOT_OCCUPANCY_FROM_STATE(checkers_slot_get_state(self)) == CHECKERS_SLOT_OCCUPIED;
    g_assert(!(selfHasMarble && self->isHighlighted) && "A slot cannot be highlighted as a valid move and at the same time be holding a marble");
    if (selfHasMarble) {
        CheckersTeam turnTeam = checkers_board_get_current_turn_team(owner);
        CheckersTeam ownTeam = CHECKERS_SLOT_TEAM_FROM_STATE(checkers_slot_get_state(self));
        gboolean canMove = turnTeam == ownTeam;
        if (canMove)
            return checkers_board_mark_slot_selected(owner, self); 
    } else if (self->isHighlighted) {
        return checkers_board_move_selected_slot(owner, self); 
    }
    g_message("click ignored");
}

static gchararray checkers_slot_resolve_resource(CheckersSlot *self, CheckersSlotState state) {
    static const char *const stateToResource[CHECKERS_SLOT_N_STATES] = {
        [CHECKERS_SLOT_RED_SLOT] = "/com/fullaccess/ChineseCheckers/resources/assets/red_empty_slot.png",
        [CHECKERS_SLOT_RED_MARBLE] = "/com/fullaccess/ChineseCheckers/resources/assets/red_marble.png",
        [CHECKERS_SLOT_BLUE_SLOT] = "/com/fullaccess/ChineseCheckers/resources/assets/blue_empty_slot.png",
        [CHECKERS_SLOT_BLUE_MARBLE] = "/com/fullaccess/ChineseCheckers/resources/assets/blue_marble.png",
        [CHECKERS_SLOT_GREEN_SLOT] = "/com/fullaccess/ChineseCheckers/resources/assets/green_empty_slot.png",
        [CHECKERS_SLOT_GREEN_MARBLE] = "/com/fullaccess/ChineseCheckers/resources/assets/green_marble.png",
        [CHECKERS_SLOT_BLACK_SLOT] = "/com/fullaccess/ChineseCheckers/resources/assets/black_empty_slot.png",
        [CHECKERS_SLOT_BLACK_MARBLE] = "/com/fullaccess/ChineseCheckers/resources/assets/black_marble.png",
        [CHECKERS_SLOT_YELLOW_SLOT] = "/com/fullaccess/ChineseCheckers/resources/assets/yellow_empty_slot.png",
        [CHECKERS_SLOT_YELLOW_MARBLE] = "/com/fullaccess/ChineseCheckers/resources/assets/yellow_marble.png",
        [CHECKERS_SLOT_WHITE_SLOT] = "/com/fullaccess/ChineseCheckers/resources/assets/white_empty_slot.png",
        [CHECKERS_SLOT_WHITE_MARBLE] = "/com/fullaccess/ChineseCheckers/resources/assets/white_marble.png",
        [CHECKERS_SLOT_EMPTY_SLOT] = "/com/fullaccess/ChineseCheckers/resources/assets/empty_slot.png" 
    };
    g_assert(state < ARRAY_SIZE(stateToResource)); 
    return g_strdup_printf("%s", stateToResource[state]);
}

extern GtkWidget *checkers_slot_new(void) {
    return g_object_new(CHECKERS_TYPE_SLOT, NULL);
}

extern CheckersSlotState checkers_slot_get_state(CheckersSlot *self) {
    CheckersSlotState hold;
    g_object_get(self, "state", &hold, NULL);
    return hold;
}

extern void checkers_slot_set_state(CheckersSlot *self, CheckersSlotState value) {
    g_object_set(self, "state", value, NULL);
    return;
}

extern gboolean checkers_slot_is_highlighted(CheckersSlot *self) {
    gboolean hold;
    g_object_get(self, "is-highlighted", &hold, NULL);
    return hold;
}

extern void checkers_slot_set_highlighted(CheckersSlot *self, gboolean value) {
    g_object_set(self, "is-highlighted", value, NULL);
    return;
}

extern gboolean checkers_slot_is_selected(CheckersSlot *self) {
    gboolean hold;
    g_object_get(self, "is-selected", &hold, NULL);
    return hold;
}

extern void checkers_slot_set_selected(CheckersSlot *self, gboolean value) {
    g_object_set(self, "is-selected", value, NULL);
    return;
}
extern CheckersTeam checkers_slot_get_team(CheckersSlot *self) {
    CheckersTeam hold;
    g_object_get(self, "team", &hold, NULL);
    return hold;
}

extern guint checkers_slot_get_row(CheckersSlot *self) {
    guint hold;
    g_object_get(self, "row", &hold, NULL);
    return hold;
}

extern guint checkers_slot_get_column(CheckersSlot *self) {
    guint hold;
    g_object_get(self, "column", &hold, NULL);
    return hold;
}
