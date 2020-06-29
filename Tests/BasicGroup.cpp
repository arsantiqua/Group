/******************************************************************************/
/*                                                                            */
/*  BasicGroup.cpp: Tests for the basic functions of group elements           */
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
/*       June 22 2020:                                                        */
/******************************************************************************/

#include <stdint.h>

#include <GroupElement.h>

GroupElement identity;
GroupElement a_element;
GroupElement b_element;
GroupElement c_element;


void
print_elements(void)
{
  std::cout << "\t\tI = <" << identity << "><"
            << identity.get_inverse() << ">\n";
  std::cout << "\t\tA = <"   << a_element << "><"
            << a_element.get_inverse() << ">\n";
  std::cout << "\t\tB = <"   << b_element << "><"
            << b_element.get_inverse() << ">\n";
  std::cout << "\t\tC = <"   << c_element << "><"
            << c_element.get_inverse() << ">\n";
}

void
usage(char * cmd)
{
  std::cout << cmd << "-a <element> -b <element> -d\n";
  std::cout << "       -a <element> where element is a string of numbers\n";
  std::cout << "       -b <element> where element is a string of numbers\n";
  std::cout << "       -d           Print debugging information\n";
  std::cout << "NOTE: <element> must contain a number corresponding to each index\n";
  std::cout << "      where the index starts with 0\n";
  std::cout << "      The numbers need (should) not be consecuting eg. 4651230\n";
  std::cout << "NOTE: The order for b must be equal to the order of a\n";
  std::cout << "      I.e. if a has 6 numbers then b must also\n";

}

int
main(int32_t argc, char *argv[])
{
  bool debug = false;
  char *cmd = argv[0];
  uint32_t group_order = 0;

  if (argc < 2)
    {
      usage(argv[0]);
      return 0;
    }

  std::string a;
  std::string b;
  std::string c;

  while (--argc)
    {
      argv++;
      switch (argv[0][1])
        {
        case 'a':
          a = argv[1];
          //A = argv[1];
          if (group_order == 0)
            {
              group_order = a.size();
            }
          else if (group_order != a.size())
            {
              std::cerr << "ERROR: Trying to use elements with a different order "
                        << " A = " << a.size()
                        << " B = " << group_order << "\n";
              return 0;
            }
          argv++;
          argc--;
          break;
        case 'b':
          b = argv[1];
          //B = argv[1];
          if (group_order == 0)
            {
              group_order = b.size();
            }
          else if (group_order != b.size())
            {
              std::cerr << "ERROR: Trying to use elements with a different order "
                        << " A = " << group_order
                        << " B = " << b.size() << "\n";
              return 0;
            }
          argv++;
          argc--;
          break;
        case 'd':
          debug = true;
          break;
        default:
          std::cerr << "Uknown flag: " << argv[0]
                    << "\n";
          usage(cmd);
          return 0;
        }
    }

  // Do some quick checks on the input

  if (a.size() != b.size())
    {
      std::cerr << "ERROR: Group element sizes are not equal: "  << a
                << " " << b
                << "\n";
      return 0;
    }

  // Some more checks
  try
    {
      identity.set_order(group_order);
    }
  catch (GroupError &e)
    {
      std::cout << "Error: Creating identity <"
                << e.get_error_code() << "> "
                << e.get_error_msg() << ">\n";
      return 0;
    }
  try
    {
      a_element.set_element(a);
    }
  catch (GroupError &e)
    {
      std::cout << "Error: element a <"
                << e.get_error_code() << "> "
                << e.get_error_msg() << ">\n";
      return 0;
    }
  try
    {
      b_element.set_element(b);
    }
  catch (GroupError &e)
    {
      std::cout << "Error: element b <"
                << e.get_error_code() << "> "
                << e.get_error_msg() << ">\n";
      return 0;
    }
  try
    {
      c_element.set_order(group_order);
    }
  catch (GroupError &e)
    {
      std::cout << "Error: Setting the order <"
                << e.get_error_code() << "> "
                << e.get_error_msg() << ">\n";
      return 0;
    }

  std::cout << "elements: ";
  std::cout << "identity I <" << identity << ">";
  std::cout << "; ";
  std::cout << "A <" << a_element << ">";
  std::cout << "; ";
  std::cout << "B <" << b_element << ">\n";
  if (debug) print_elements();

  std::cout << " 1: Does the assignment work? C <- I ----------------------------------- ";
  c_element = identity;

  if (c_element != identity)
    {
      std::cout << "FAIL: c == <" << c_element
                << ">\n";
    }
  else
    {
      std::cout << "PASS\n";
    }

  if (debug) print_elements();

  std::cout << " 2: Does general assignment work? C <- A ------------------------------- ";
  c_element = a_element;
  if (c_element != a_element)
    {
      std::cout << "FAIL\n";
    }
  else
    {
      std::cout << "PASS\n";
    }
  if (debug) print_elements();

  std::cout << " 3: Does the I multiply with itself? C <- I * I ------------------------ ";
  c_element = identity * identity;

  if (c_element != identity)
    {
      std::cout << "FAIL\n";
    }
  else
    {
      std::cout << "PASS\n";
    }

  if (debug) print_elements();

  std::cout << " 4: Does the left identity element change other elements? C <- I * A --- ";

  c_element = identity * a_element;
  if (c_element != a_element)
    {
      std::cerr << "FAIL\n";
    }
  else
    {
      std::cerr << "PASS\n";
    }

  if (debug) print_elements();

  std::cout << " 5: Does the right identity element change other elements? C <- A * I -- ";

  c_element = a_element * identity;
  if (c_element != a_element)
    {
      std::cerr << "FAIL\n";
    }
  else
    {
      std::cerr << "PASS\n";
    }

  if (debug) print_elements();

  std::cout << " 6: Does the left inverse work? A(-1) * A ------------------------------ ";

  c_element.set_element(a_element.get_inverse());

  c_element *= a_element;
  if (c_element != identity)
    {
      std::cerr << "FAIL\n";
    }
  else
    {
      std::cerr << "PASS\n";
    }
  if (debug) print_elements();

  std::cout << " 7: Does the right inverse work? B * B(-1) ----------------------------- ";

  c_element.set_element(b_element.get_inverse());

  c_element = b_element * c_element;
  if (c_element != identity)
    {
      std::cerr << "FAIL\n";
    }
  else
    {
      std::cerr << "PASS\n";
    }
  if (debug) print_elements();

  std::string bad_element;
  bad_element += group_order + '0';

  for (uint32_t idx = 1; idx < group_order; idx++)
    {
      bad_element += idx + '0';
    }
  std::cout << " 8: Does it check for malformed elements? C <- <"
            << bad_element << ">------------------- ";
  try
    {
      c_element.set_element(bad_element);
      std::cout << "FAIL\n";
    }
  catch (GroupError &e)
    {
      //std::cout << "PASS <"  << e << ": " << group_element_error_code[e] << ">\n";
      std::cout << "PASS <"  << e.get_error_code() << ": " << e.get_error_msg() << ">\n";
    }

  bad_element.clear();
  // Deliberately setting an element value with too few items
  for (uint32_t idx = 0; idx < group_order - 1; idx++)
    {
      bad_element += idx + '0';
    }
  std::cout << " 9: Multiplying A with too small an element? C <- <"
            << bad_element << ">----------------- ";

  try
    {
      c_element.set_element(bad_element);
      c_element *= a_element;
      std::cout << "FAIL\n";
    }
  catch (GroupError &e)
    {
      std::cout << "PASS <"  << e.get_error_code() << ": " << e.get_error_msg() << ">\n";
    }

  bad_element.clear();
  for (uint32_t idx = 0; idx < group_order + 1; idx++)
    {
      bad_element += idx + '0';
    }
  std::cout << "10: Multiplying A with too big an element? C <- <"
            << bad_element << ">----------------- ";

  try
    {
      c_element.set_element(bad_element);
      c_element *= a_element;
      std::cout << "FAIL\n";
    }
  catch (GroupError &e)
    {
      std::cout << "PASS <" << e.get_error_code() << ": " << e.get_error_msg() << ">\n";
    }

  bad_element.clear();
  bad_element += '0';
  for (uint32_t idx = 0; idx < group_order - 1; idx++)
    {
      bad_element += idx + '0';
    }
  std::cout << "11: Check for duplicated value? C <- <"
            << bad_element << ">----------------------------- ";

  try
    {
      c_element.set_element(bad_element);
      c_element *= a_element;
      std::cout << "FAIL\n";
    }
  catch (GroupError &e)
    {
      std::cout << "PASS <" << e.get_error_code() << ": " << e.get_error_msg() << ">\n";
    }
}
