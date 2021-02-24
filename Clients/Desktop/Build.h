#ifndef _BUILD_CLIENT_H
#define _BUILD_CLIENT_H

#include <gtk/gtk.h>
#include "Garbage.h"
#include "List.h"
#include "String.h"

#ifdef __cplusplus
extern "C" {
#endif

String* buildVote(List* groups);

#ifdef __cplusplus
}
#endif

#endif