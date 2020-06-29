/******************************************************************************/
/*                                                                            */
/*  EvenOdd.cpp: Test to see if a given permuation is even or odd             */
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

#include <utility>  // for sawp
#include <GroupElement.h>

void permute(std::string & a, int32_t l, int32_t r);



int
main(int32_t argc, char *argv[])
{
  if (argc < 2)
    {
      std::cout << "You need to supply the Group order [2-8]\n";
      return 0;
    }

  uint32_t group_order = atoi(argv[1]);
  GroupElement identity_element;
  std::string str;

  fprintf(stderr, "Setting group order to %u\n", group_order);
  identity_element.set_order(group_order);
  str = identity_element.get_element();

  std::cout << "      Element   Inverse Even or Odd\n";
  permute(str, 0, group_order - 1);

  return 1;
}

void permute(std::string & a, int l, int r)
{
  // Base case
  if (l == r)
    {
      GroupElement element;

      element.set_element(a);

      std::string e = element.get_inverse();

      std::cout << "\t" << element << "    " << element.get_inverse() << "     "
                << (element.is_odd() ? "Odd" : "Even") << "\n";
    }
  else
    {
      // Permutations made
      for (int32_t i = l; i <= r; i++)
        {

          // Swapping done
          std::swap(a[l], a[i]);

          // Recursion called
          permute(a, l+1, r);

          //backtrack
          std::swap(a[l], a[i]);
        }
    }
}
