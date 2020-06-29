/******************************************************************************/
/*                                                                            */
/*  GroupError.h Definitions for error descriptions                           */
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
/*       June 10 2020:                                                        */
/*                                                                            */
/******************************************************************************/

#ifndef GROUP_ERROR_H__
#define  GROUP_ERROR_H__

#include <string>


typedef enum
  {
    GrpErr_OK                       =  0,
    GrpErr_ElementValueOutOfRange   =  1,  // a value is above the group order
    GrpErr_ElementValueDuplicated   =  2,  // Duplicated values in the element
    GrpErr_ElementValueMissing      =  3,  // The element is missing some values
    GrpErr_ElementValueNull         =  4,  // The element value is an empty string
    GrpErr_IncompatibleGroupElement =  5,  // An attempt to add a member that is incompatible with the current group
    GrpErr_ElementOverflow          =  6,  //
    GrpErr_UndefinedGroupOrder      =  7,  // Likely an uninitialized element in a computation
    GrpErr_MissingElement           =  8,  // Consistency check failure
    GrpErr_OrderMismatch            =  9,  //
    GrpErr_GroupOrderCannotBeReset  = 10,  // Changing a group's order when elements are already present
    GrpErr_MissingIdentity          = 11,  // Consistency check failure
    GrpErr_IndexOutOfBounds         = 12,  //
    GrpErr_BadRotationDirection     = 13,
    GrpErr_UnimplimentedFunction    = 14,
    GrpErr_UninitializedElement     = 14,
    GrpErr_MaxError                 = 16,
    GrpErr_UnknownError             =  GrpErr_MaxError
  } GrpErr_t;

extern const uint32_t max_order;

class GroupError
{
 public:
  GroupError(){};
  GroupError(GrpErr_t code);
  ~GroupError(){};

  const std::string &get_error_msg(void);
  GrpErr_t get_error_code(void);
  void set_error_code(GrpErr_t erro);

 private:
  GrpErr_t error_;
};


const std::string &get_error_msg(GrpErr_t error);


#endif // GROUP_ERROR_H__
