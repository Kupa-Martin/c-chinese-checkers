#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))
#define STRINGIFY(x) #x
#define LENGTH(str) (sizeof(str) - 1)
#define COUNT_DIGITS(n) (LENGTH(STRINGIFY(n)))

#include <gtk/gtk.h>
#include "enum_types.h"
#include "widgets/CheckersBoardButton.h"
#include "widgets/CheckersPlayerSelect.h"
#include "widgets/MainMenuWindow.h"
#include "widgets/CheckersWindow.h"
#include "widgets/CheckersGameOver.h"
#include "widgets/CheckersBoard.h"

#endif