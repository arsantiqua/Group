/******************************************************************************/
/*                                                                            */
/*  Group.h Definitions for Group descriptions and interactions               */
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

#ifndef GROUP_H__
#define  GROUP_H__

#include <stdint.h>

#include <set>

// The various means by which we may flesh out a group
typedef enum
  {
    GrpGen_Unknown,   // No symmetry
    GrpGen_Cyclic,    // Cyclic from a specified generator
    GrpGen_Sn,        // Permuations on n objects
    GrpGen_An,        // The alternating (even) permutation on n objects
    GrpGen_Dn,        // Polygonal symmetries on n vertices (rotation & reflection)
    GrpGen_Cn,        // Rotational Symmetries on n vertices
    GrpGen_FromFile,  // Read from a file
    GrpGen_MaxGrpGen
  } GrpGen_t;


class Group
{
 public:
  Group();
  ~Group();

  friend std::ostream &operator <<(std::ostream &output, Group &G)
  {
    std::set<GroupElement>::iterator itr;

    for (itr = G.elements.begin(); itr != G.elements.end(); ++itr)
      {
        output << "<" << *itr << ">";
      }
    return output;
  }
  // Remove the elements from the group
  void erase(void);
  // Clear all information from the group
  void clear(void);
  uint32_t get_order(void);
  GroupElement get_identity(void) const;
  std::string get_identity_value(void);
  GrpErr_t add_element(GroupElement &e);
  GrpErr_t delete_element(uint32_t index);

  // Various Group generation methods
  // Generate a cyclic group based on the element e
  GrpErr_t generate(GroupElement &e);

  // Generate the full permutation group
  void permute(std::string a, int32_t l, int32_t r);
  GrpErr_t generate_Sn(uint32_t order);

  // Generate the even subgroup of Sn
  GrpErr_t generate_An(uint32_t order);

  // Generate the Dihedral group
  GrpErr_t generate_Dn(uint32_t vertices);

  // Generate the polygon rotational group
  GrpErr_t generate_Cn(uint32_t vertices);

  GrpErr_t generate_group_from_file(std::iostream in);

  // Internal consistency check
  GrpErr_t is_consistent();

  // Element is returned in the reference
  bool get_element(uint32_t element_index, GroupElement &e);

  // The number of elements currently in the group
  // The group need not represent a full group
  // I.e. elements may be missing if building up the group
  // one element at a time
  uint32_t size(void);

 private:
  uint32_t group_order;

  GroupElement identity;

  std::set<GroupElement> elements;
};

// The various generator functions


#endif // GROUP_H__
