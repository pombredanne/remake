/*
Copyright (C) 2011, 2015 R. Bernstein <rocky@gnu.org>
This file is part of GNU Make (remake variant).

GNU Make is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Make is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Make; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */
/* Continue running program. */
static debug_return_t
dbg_cmd_continue (char *psz_args)
{
  if (psz_args && *psz_args) {
    char *psz_target = get_word(&psz_args);
    file_t *p_target = NULL;
    brkpt_mask_t i_brkpt_mask;

    /** FIXME: DRY with code in break.h **/
    if (p_stack && p_stack->p_target) {
      char *psz_expanded_target =
        variable_expand_set(psz_target, p_stack->p_target->variables);
      if (*psz_expanded_target) {
        p_target = lookup_file(psz_expanded_target);
      }
    } else
      p_target = lookup_file(psz_target);

    if (!p_target) {
      printf("Can't find target %s; breakpoint not set.\n", psz_target);
	return debug_cmd_error;
    }

    /* FIXME: Combine with code in continue. */
    psz_args = get_word(&psz_args);
    if (!(psz_args && *psz_args))
      i_brkpt_mask = BRK_ALL;
    else {
      char *psz_break_type;
      i_brkpt_mask = get_brkpt_option(psz_args);
      while ((psz_break_type = get_word(&psz_args))) {
        if (!(psz_break_type && *psz_break_type)) break;
        i_brkpt_mask |= get_brkpt_option(psz_break_type) ;
      }
    }

    if (!add_breakpoint(p_target, i_brkpt_mask|BRK_TEMP))
      return debug_cmd_error;
  } else  {
    db_level = 0;
  }

  i_debugger_stepping = 0;
  i_debugger_nexting  = 0;
  define_variable_in_set("MAKEFLAGS", sizeof("MAKEFLAGS")-1,
                         "", o_debugger, 0, NULL, NULL);
  return continue_execution;
};

static void
dbg_cmd_continue_init(unsigned int c)
{
  short_command[c].func = &dbg_cmd_continue;
  short_command[c].use  = _("continue [TARGET [all|run|prereq|end]*]");
  short_command[c].doc  =
    _("Continue executing debugged Makefile until another breakpoint or\n"
"stopping point. If a target is given and valid we set a temporary\n"
"breakpoint at that target before continuing.\n"
"\n"
"When a target name is given, breakpoint properties can be given after\n"
"the target name\n"
"\n"
"See also \"break\" and \"finish\".\n"
);
}

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * indent-tabs-mode: nil
 * End:
 */
