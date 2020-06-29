/******************************************************************************/
/*                                                                            */
/*  GenerateGroup.cpp: generate the group through successive multiplications  */
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

#include <iostream>

#include <GroupElement.h>

void
usage(char *cmd)
{
  std::cerr << "USAGE: " << cmd << " 01[2[3[4...]]]\n";
}

int32_t
main(int32_t argc, char *argv[])
{
  if (argc < 2)
    {
      usage(argv[0]);
    }

  GroupElement GP;
  GroupElement Accumulator;
  GroupElement I;
  GP.set_element(argv[1]);
  Accumulator.set_element(argv[1]);
  I.set_order(strlen(argv[1]));

  std::cout << GP << "\n";

  while (Accumulator != I)
    {
      Accumulator *= GP;
      std::cout << Accumulator << "\n";
    }
  return 0;
}
