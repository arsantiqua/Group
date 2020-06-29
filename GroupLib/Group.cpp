/******************************************************************************/
/*                                                                            */
/*  Group.cpp: A class for controlling access to a group                      */
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
/******************************************************************************/

#include <stdint.h>
#include <string>
#include <algorithm>   // std::rotate

#include "GroupError.h"
#include "GroupElement.h"
#include "Group.h"

const std::string group_error_code[] =
  {
    "No Error",
    "Uknown Error"
  };


Group::Group()
{
  group_order = 0;
}

Group::~Group()
{
}

void
Group::erase(void)
{
  elements.clear();
}

void
Group::clear(void)
{
  elements.clear();
  identity.clear();
  group_order = 0;
}

uint32_t
Group::get_order(void)
{
  return elements.size();
}

GroupElement
Group::get_identity(void) const
{
  return identity;
}


std::string
Group::get_identity_value(void)
{
  return identity.get_element();
}


GrpErr_t
Group::add_element(GroupElement &e)
{
  GrpErr_t rVal = GrpErr_OK;

  elements.insert(e);

  return rVal;
}

GrpErr_t
Group::delete_element(uint32_t index)
{
  if (index > size())
    {
      return GrpErr_IndexOutOfBounds;
    }

  std::set<GroupElement>::iterator iter;
  uint32_t local_index = 0;

  for (iter = elements.begin(); iter != elements.end(); ++iter, local_index++)
    {
      if (index == local_index)
        {
          elements.erase(iter);
          return GrpErr_OK;
        }
    }
  return GrpErr_IndexOutOfBounds;
}

GrpErr_t
Group::generate(GroupElement &e)
{
  GrpErr_t rVal = GrpErr_OK;
  GroupElement new_element = e;
  GroupElement ident;

  ident.set_element(e.create_identity());

  clear();
  add_element(ident);
  //  add_element(e);

  while (new_element != ident)
    {
      add_element(new_element);
      new_element *= e;
    }

  return rVal;
}

// Create the permutation group of the give order
// if the order is 0 then use the preset order of the group
// else the group is set by order
GrpErr_t
Group::generate_Sn(uint32_t order)
{
  GrpErr_t rVal = GrpErr_OK;
  GroupElement element;

  erase();

  element.set_order(order);

  // Default element is identity
  std::string value = element.get_element();

  permute(value, 0, order - 1);
  return rVal;
}

void
Group::permute(std::string a, int32_t l, int32_t r)
{
  if (a.size() == 0)
    {
      GroupError e;
      e.set_error_code(GrpErr_ElementValueNull);
      throw e;
    }
    // Base case
    if (l == r)
      {
        GroupElement element;

        element.set_element(a);
        this->add_element(element);
      }
    else
      {
        // Permutations made
        for (int32_t i = l; i <= r; i++)
          {

            // Swapping done
            std::swap(a[l], a[i]);

            // Recursion called
            permute(a, l + 1, r);

            //backtrack
            std::swap(a[l], a[i]);
        }
    }
}

GrpErr_t
Group::generate_An(uint32_t order)
{
  generate_Sn(order);

  std::set<GroupElement>::iterator itr;

  for (itr = elements.begin(); itr != elements.end(); ++itr)
    {
      if (itr->is_odd())
        {
          elements.erase(itr);
        }
    }
  return GrpErr_OK;
}

// Generate the dihedral group with the given vertices
// This includes polygonal rotations and polygonal reflections
GrpErr_t
Group::generate_Dn(uint32_t vertices)
{
  GrpErr_t rVal = GrpErr_OK;
  generate_Cn(vertices);

  std::vector<GroupElement> reflections;
  std::set<GroupElement>::iterator itr;
  GroupError error;

  // Set up an element where we can get a proper identity
  GroupElement e;
  e.set_order(vertices);
  add_element(e);

  GroupElement reflection;

  if ((vertices % 2) != 0) // i.e. odd # of vertices
    {
      // For each vertex (idx) in the polygon
      // wrap around clockwise (right) and counterclockwise (left)
      for (int32_t idx = 0; idx < static_cast<int32_t>(vertices); idx++)
        {
          std::string reflection_value;
          int32_t right = (idx + 1) % vertices;
          int32_t left = (idx + vertices - 1) % vertices;

          // get the identity value
          reflection_value = e.get_element();

          for (uint32_t idx2 = 0; idx2 < vertices/2; idx2++)
            {
              std::swap(reflection_value[right], reflection_value[left]);
              right++; right %= vertices;
              left += vertices; left-- ; left %= vertices;
            }
          reflection.set_element(reflection_value);
          add_element(reflection);
        }
    }
  else
    {
      // Since even polygons reflect on vertex pairs and edge pairs
      // do the vertices (half of them)
      for (int32_t idx = 0; idx < static_cast<int32_t>(vertices)/2; idx++)
        {
          std::string reflection_value;
          int32_t right = (idx + 1) % vertices;
          int32_t left = (idx + vertices - 1) % vertices;

          // get the identity value
          reflection_value = e.get_element();

          // Add the reflection on vertex
          for (uint32_t idx2 = 0; idx2 < vertices/4; idx2++)
            {
              std::swap(reflection_value[right], reflection_value[left]);
              right++; right %= vertices;
              left += vertices; left-- ; left %= vertices;
            }
          reflection.set_element(reflection_value);
          add_element(reflection);

          // get the identity value
          reflection_value = e.get_element();


          right = idx % vertices;
          left = (idx + vertices - 1) % vertices;
          // Add the reflection on the edge
          for (uint32_t idx2 = 0; idx2 <= vertices/4; idx2++)
            {
              std::swap(reflection_value[right], reflection_value[left]);
              right++; right %= vertices;
              left += vertices; left-- ; left %= vertices;
            }
          reflection.set_element(reflection_value);
          add_element(reflection);
        }
    }

  return rVal;
}

GrpErr_t
Group::generate_Cn(uint32_t vertices)
{
  GrpErr_t rVal = GrpErr_OK;
  GroupElement element;

  if (vertices == 0)
    {
      GroupError e(GrpErr_UndefinedGroupOrder);
      throw e;
    }

  erase();
  clear();


  element.set_order(vertices);

  // Default element is identity
  std::string value = element.get_element();

  GroupElement new_element;
  new_element.set_element(value);
  do
    {
      add_element(new_element);
      std::rotate(value.begin(),
                  value.begin() + 1,
                  value.end());

      new_element.set_element(value);

    } while (element != new_element);

  return rVal;
}

GrpErr_t
Group::generate_group_from_file(std::iostream in)
{
  GrpErr_t rVal = GrpErr_OK;
  GroupError e;
  e.set_error_code(GrpErr_UnimplimentedFunction);
  throw e;
  return rVal;
}




GrpErr_t
Group::is_consistent(void)
{
  GrpErr_t rVal = GrpErr_OK;
  std::set<GroupElement>::iterator iter;
  iter = elements.begin();
  const GroupElement ident = get_identity();

  if (*iter != ident)
    {
      rVal = GrpErr_MissingIdentity;
    }

  return rVal;
}


bool
Group::get_element(uint32_t element_index, GroupElement &e)
{
  std::set<GroupElement>::iterator iter;
  uint32_t local_index = 0;

  for (iter = elements.begin(); iter != elements.end(); ++iter, local_index++)
    {
      if (element_index == local_index)
        {
          e = *iter;
          return true;
        }
    }
  return false;
}

uint32_t
Group::size(void)
{
  return elements.size();
}
