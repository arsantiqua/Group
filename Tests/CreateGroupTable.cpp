/******************************************************************************/
/*                                                                            */
/*  CreateGroupTable.cpp: creates the group multiplication table              */
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
#include <string.h>

#include <getopt.h>

#include <iostream>

#include <GroupElement.h>
#include <Group.h>

void
usage(char *cmd)
{
  std::cerr << "USAGE: " << cmd << " [OPTIONS]\n";
  std::cerr << "       -a, --alternating <order>       -- Generate the Alternating group An (Even subgroup of Sn).\n";
  std::cerr << "       -c, --cyclic 012[3[4...]]]      -- Generate the group from cycling on the given element.\n";
  std::cerr << "       -d, --dihedral <vertices>       -- Generate dihedral group for a polygon Dn (rotations and reflections).\n";
  std::cerr << "       -f, --file <filename>           -- Read the group from the given file\n";
  std::cerr << "       -r, --rotation <vertices>       -- Generate polygonal rotation.\n";
  std::cerr << "       -s, --permutation <group_order> -- Generate the symmetric group Sn.\n";
}

int32_t
main(int32_t argc, char *argv[])
{
  try
    {
      std::string group_seed = "";
      Group group1;
      GroupElement element1;
      int32_t opt;
      GrpGen_t gen_type = GrpGen_Unknown;
      uint32_t vertices = 0;

      while (1)
        {
          int32_t option_index = 0;

          static struct option long_options[] =
            {
              {"alternating", required_argument, 0,  'a'},
              {"cyclic",      required_argument, 0,  'c'},
              {"dihedral",    required_argument, 0,  'd'},
              {"file"  ,      required_argument, 0,  'f'},
              {"rotation",    required_argument, 0,  'r'},
              {"permutation", required_argument, 0,  's'},
              {"Help",        no_argument,       0,  'H'},
              {"help",        no_argument,       0,  'h'},
              {"HELP",        no_argument,       0,  '?'},
              {0,             0,                 0,   0 }
            };

          opt = getopt_long(argc, argv, "a:c:d:f:r:s:hH?",
                            long_options, &option_index);
          if (opt == -1)
            {
              break;
            }
          switch (opt)
            {
            case 'a':  // The Alternating subgroup of Sn
              gen_type = GrpGen_An;
              vertices = atoi(optarg);
              break;

            case 'c':  // Cycle generated from seed
              gen_type = GrpGen_Cyclic;
              group_seed = optarg;
              break;

            case 'd': // Rotations and Reflections
              std::cout << "Currently the focus\n";
              gen_type = GrpGen_Dn;
              vertices = atoi(optarg);
              break;

            case 'f':
              std::cout << "Currently unavailable\n";
              return 0;
              break;

            case 'r':  // Rotations
              gen_type = GrpGen_Cn;
              vertices = atoi(optarg);
              break;

            case 's':
              gen_type = GrpGen_Sn;
              vertices = atoi(optarg);
              break;

            default:
              std::cerr << "Error: Unknown command line argument [" << opt << "]\n";
              // no break; just fall into the help
            case 'h':
            case 'H':
            case '?':
              usage(argv[0]);
              return 0;
              break;
            }
        }

      switch (gen_type)
        {
        case GrpGen_Cyclic:
          // Generate the group from the seed provided
          if (group_seed != "")
            {
              element1.set_element(group_seed);

              group1.generate(element1);
              std::cout << "Cyclic G: " << group1 << "\n";
            }
          break;
        case GrpGen_Sn:
          group1.generate_Sn(vertices);
          break;
        case GrpGen_Dn:
          group1.generate_Dn(vertices);
          break;
        case GrpGen_Cn:
          group1.generate_Cn(vertices);
          break;
        case GrpGen_An:
          group1.generate_An(vertices);
          break;
        case GrpGen_FromFile:
          std::cerr << "Unavailable generator type: " << gen_type << "\n";
          break;
        default:
          std::cerr << "Unknown generator type: " << gen_type << "\n";
          break;
        }

      // Move the elements into a vector for easier manipulation
      uint32_t group_size = group1.size();
      std::vector<GroupElement> e_list;

      for (uint32_t idx = 0; idx < group_size; idx++)
        {
          GroupElement e;

          if (!group1.get_element(idx, e))
            {
              std::cerr << "Error: Cannot get the element at position " << idx << "\n";
            }
          else
            {
              e_list.push_back(e);
            }
        }

      // print out the header
      for (uint32_t idx2 = 0; idx2 < e_list[0].get_order(); idx2++)
        {
          std::cout << " ";
        }

      for (uint32_t idx1 = 0; idx1 < e_list.size(); idx1++)
        {
          std::cout << " | " << e_list[idx1].get_element();
        }
      std::cout << "\n";

      for (uint32_t idx1 = 0; idx1 < e_list.size(); idx1++)
        {
          std::cout << e_list[idx1].get_element();
          for (uint32_t idx2 = 0; idx2 < e_list.size(); idx2++)
            {
              GroupElement out;
              out = e_list[idx1] * e_list[idx2];
              std::cout << " | " << out;
            }
          std::cout << "\n";
        }
    }
  catch (GroupError &e)
    {
          std::cerr << "Error creating a group generated from "
                    << e.get_error_code()
                    << ": " << e.get_error_msg()
                    << "\n";
    }
  return 0;
}
