/******************************************************************************/
/*                                                                            */
/*  GroupError.cpp A class for dealing with Group errors                      */
/*                                                                            */
/*  Copyright 2020 Dennis Paul Fleming                                        */
/*                                                                            */
/*  This software is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation, either version 3 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.     */
/*                                                                            */
/*                                                                            */
/* Version History                                                            */
/*  1.0: Initial version                                                      */
/*       June 11 2020:                                                        */
/*                                                                            */
/******************************************************************************/

#include <stdint.h>

#include "GroupError.h"

const std::string group_element_error_code[] =
  {
    "GrpErr_OK",                       // ==  0
    "GrpErr_ElementValueOutOfRange",   // ==  1
    "GrpErr_ElementValueDuplicated",   // ==  2
    "GrpErr_ElementValueMissing",      // ==  3
    "GrpErr_ElementValueNull",         // ==  4
    "GrpErr_IncompatibleGroupElement", // ==  5
    "GrpErr_ElementOverflow",          // ==  6
    "GrpErr_UndefinedGroupOrder",      // ==  7
    "GrpErr_MissingElement",           // ==  8
    "GrpErr_OrderMismatch",            // ==  9
    "GrpErr_GroupOrderCannotBeReset",  // == 10
    "GrpErr_MissingIdentity",          // == 11
    "GrpErr_IndexOutOfBounds",         // == 12
    "GrpErr_BadRotationDirection",     // == 13
    "GrpErr_UnimplimentedFunction",    // == 14
    "GrpErr_UninitializedElement",     // == 15
    "GrpErr_UnknownError"              // == 16
  };

// TODO: Need to consider if this is needed
const uint32_t max_order = 8;

GroupError::GroupError(GrpErr_t error)
{
  set_error_code(error);
}

const std::string &
GroupError::get_error_msg(void)
{
  if ((error_ >= GrpErr_MaxError) || (error_ < 0))
    {
      return group_element_error_code[GrpErr_UnknownError];
    }
  return group_element_error_code[error_];
}

GrpErr_t
GroupError::get_error_code(void)
{
  return error_;
}

void
GroupError::set_error_code(GrpErr_t error)
{
  if ((error < GrpErr_OK) || (error >= GrpErr_MaxError))
    {
      error_ = GrpErr_UnknownError;
    }
  else
    {
      error_ = error;
    }
}

// Nonclass
const std::string &
get_error_msg(GrpErr_t error)
{
  return group_element_error_code[error];
}
