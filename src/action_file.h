#ifndef _action_file_h
#define _action_file_h

#include <inttypes.h>
#include <stdbool.h>

typedef struct sky_action_file sky_action_file;

#include "bstring.h"
#include "file.h"
#include "types.h"
#include "table.h"
#include "action.h"

//==============================================================================
//
// Overview
//
//==============================================================================

// The actions file is a MessagePack formatted file that lists what actions are
// stored in an associated table. Each table has one actions file. Currently
// actions only support a numeric ID and a name but additional fields may be
// allowed in the future.


//==============================================================================
//
// Typedefs
//
//==============================================================================

struct sky_action_file {
    sky_table *table;
    sky_action **actions;
    int64_t action_count;
};


//==============================================================================
//
// Functions
//
//==============================================================================

//======================================
// Lifecycle
//======================================

sky_action_file *sky_action_file_create(sky_table *table);

void sky_action_file_free(sky_action_file *action_file);


//======================================
// Persistence
//======================================

int sky_action_file_get_path(sky_action_file *action_file, bstring *path);

int sky_action_file_load(sky_action_file *action_file);

int sky_action_file_unload(sky_action_file *action_file);

int sky_action_file_save(sky_action_file *action_file);


//======================================
// Action Management
//======================================

int sky_action_file_find_action_by_name(sky_action_file *action_file,
    bstring name, sky_action **ret);

int sky_action_file_add_action(sky_action_file *action_file, sky_action *ret);

#endif
