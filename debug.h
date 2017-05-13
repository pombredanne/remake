/* Debugging macros and interface.
Copyright (C) 1999-2014 Free Software Foundation, Inc.
This file is part of GNU Make.

GNU Make is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

GNU Make is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef DEBUG_H
#define DEBUG_H

#include "types.h"

/**
  \brief debug masks which control tracing output.

  Imagine the below enums values as define'd values rather than
  distinct values of an enum. The enum is more (gdb) debugger friendly
  in showing what's set (e.g. DB_NONE, DB_ALL, or a single value
  and in entering expressons (e.g print db_level & DB_BASIC).
*/
typedef enum {
  DB_NONE           = 0x000, /**< Mask when none of the below are set. */
  DB_BASIC          = 0x001, /**< targets which need to be made and status;
				 also set when tracing or debugging */
  DB_VERBOSE        = 0x002, /**< A more verbose trace */
  DB_JOBS           = 0x004, /**< Prints messages giving details on the
				  invocation of specific subcommands.
				  Can be set via --debug=jobs
			     */
  DB_IMPLICIT       = 0x008, /**< Prints messages describing the implicit
				  rule searches for each target. Can be
				  set via --debug=implicit
			     */
  DB_TRACE          = 0x010, /**< tracing */
  DB_SHELL          = 0x020, /**< add +x to SHELL invocations */
  DB_MAKEFILES      = 0x100,
  DB_READ_MAKEFILES = 0x200, /**< Reading makefiles */
  DB_CALL           = 0x400, /**< GNU Make function call and returns */
  DB_UPDATE_GOAL    = 0x800, /**< GNU Make function call and returns */
  DB_ALL            = 0xfff  /**< Bitmask for all of the above. */
} debug_level_mask_t;

typedef enum {
  DEBUGGER_ON_ERROR  = 0x1,   /**< Enter debugger on any error */
  DEBUGGER_ON_FATAL  = 0x2,   /**< Enter debugger on a fatal error */
  DEBUGGER_ON_SIG    = 0x4    /**< Enter debugger on getting a signal */
} debug_enter_debugger_t;

typedef enum {
  DEBUGGER_QUIT_RC   = 77,    /**< debugger issued a "quit" command. */
} debug_dummy_t;

/** These variables are trickery to force the above enum symbol values to
    be recorded in debug symbol tables. It is used to allow one refer
    to above enumeration values in a debugger and debugger
    expressions */
extern debug_level_mask_t debug_dummy_level_mask;
extern debug_enter_debugger_t debug_dummy_enter_debugger_mask;

/** bitmask of debug_level_mask values. */
extern int db_level;

/*!
  If 0 (or false) we are not in the debugger command read loop.
  If 1 (or true) we are in the debugger command read loop.
  If DEBUGGER_QUIT_RC we've requested to quit.
 */
extern int in_debugger;

#include <setjmp.h>
extern jmp_buf debugger_loop;

/** \brief The structure used to hold the list of strings given
    in command switches of a type that takes string arguments.  */
struct stringlist
{
  const char **list;	/**< Nil-terminated list of strings.  */
  unsigned int idx;	/**< Index into above.  */
  unsigned int max;	/**< Number of pointers allocated.  */
};

extern int debug_flag;

/*! If 1, we don't give additional error reporting information. */
extern int no_extended_errors;

/*! If 1, we show variable definitions */
extern int show_variable_definitions;

/*! If true, enter the debugger before updating goal target. */
extern bool b_debugger_goal;

/*! If true, enter the debugger before reading any makefiles. */
extern bool b_debugger_preread;

/*! If nonzero, we are debugging after each "step" for that many times.
  When we have a value 1, then we actually run the debugger read loop.
  Otherwise we decrement the step count.

*/
extern unsigned int i_debugger_stepping;

/*! If nonzero, we are debugging after each "next" for that many times.
  When we have a value 1, then we actually run the debugger read loop.
  Otherwise we decrement the step count.

*/
extern unsigned int i_debugger_nexting;

/*! If nonzero, enter the debugger if we hit a fatal error.
*/
extern unsigned int debugger_on_error;

/*! If nonzero, we have requested some sort of debugging.
*/
extern unsigned int debugger_enabled;

extern stringlist_t *db_flags;

#define ISDB(_l)    ((_l)&db_level)

/*! Debugged print */
#define DBPRINT(_x)           \
   printf _x; fflush (stdout)

/*! Debugged print indented a number of spaces given by "_depth" */
#define DBPRINTS(_x, _depth) \
   print_spaces (_depth);    \
   DBPRINT(_x)

/*! Debugged print if debug mask is set indented a number of spaces
    implied by global variable "depth"
*/
#define DBS(_l,_x)           \
  do {                       \
    if(ISDB(_l)) {           \
       DBPRINTS(_x, depth);  \
    }                        \
  } while(0)

/*! Debugged print if debug mask is set indented a number of spaces
    given by "_depth"
*/
#define DBSD(_l,_x,_depth)   \
  do {                       \
    if(ISDB(_l)) {           \
      DBPRINTS(_x, _depth);   \
    }                        \
  } while(0)

extern int db_level;

#define ISDB(_l)    ((_l)&db_level)

#define DBF(_l,_x)  do{ if(ISDB(_l)) {print_spaces (depth); \
                                      printf (_x, file->name); \
                                      fflush (stdout);} }while(0)

#define DB(_l,_x)   do{ if(ISDB(_l)) {printf _x; fflush (stdout);} }while(0)


#endif /*DEBUG_H*/
