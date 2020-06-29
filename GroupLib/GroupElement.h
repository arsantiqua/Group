/******************************************************************************/
/*                                                                            */
/*  Group.h Definitions for Group Elements as permutations                    */
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

#ifndef GROUP_ELEMENT_H__
#define GROUP_ELEMENT_H__


#include <iostream>
#include <vector>
#include <bitset>

#include "GroupError.h"

typedef enum
  {
    GrpElDirection_right,
    GrpElDirection_left,
  } GrpElDirection_t;

class GroupElement
{
 public:

  GroupElement();
  ~GroupElement();

 public:
  GroupElement operator =(const GroupElement& rhs);
  bool operator ==(const GroupElement& rhs) const;
  bool operator !=(const GroupElement& rhs) const;
  bool operator <(const GroupElement& rhs) const;
  bool operator >(const GroupElement& rhs) const;
  GroupElement operator *=(const GroupElement &rhs);
  // operator * defined outside of the class

  friend std::ostream &operator <<(std::ostream &output, const GroupElement &G)
  {
    output << G.get_element();

    return output;
  }


  void set_order(uint32_t order);

  // Creates an identity value comensurate to the element's order
  std::string create_identity(void);
  std::string get_element(void) const;
  void clear(void);

  // If the order has been set then there will be a check on the value
  void set_element(const std::string  element_str);
  bool check_element(const GroupElement &group_element);
  GrpErr_t check_element_values(const std::string &element_values);

  uint32_t get_order(void) const;
  std::string get_inverse(void);
  bool is_odd(void) const;

  const std::string &get_error_msg(GrpErr_t error);

 private:
  std::string value;
  std::string inverse;

  void create_inverse(void);

 private:
  std::bitset<8> group_order_map;
  int32_t eveness;
};

bool check_element_value(std::string element_value, uint32_t order);
bool is_even(GroupElement &G);

inline GroupElement
operator *(GroupElement lhs, const GroupElement& rhs)
{
  lhs *= rhs;
  return lhs;
}


#endif // GROUP_ELEMENT_H__
