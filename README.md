# Group
A class for investigating finite groups
This is part of my investigation in to finite groups.
It is also a way to explore C++ and experiment on its capabilities.
There are two aspects to a group:
1. The order of each group element.
2. The order of the group itself.

I've organized group elements as permutations of numbers (Sn). 
Groups themselves have order by virtue of the number of elements in them.

# Dealing with the Source
I've included a makefile hierarchy taken from https://github.com/marbl/canu/blob/master/src/Makefile
in an effort to get away from recursive makefiles.  The tradeoff is that the makefile itself is a bit impenetrable.

## Compiling
At the top directory type make.  This has only been tested on Linux.
## Test Programs

1. BasicGroup: This runs various tests on two group elements given in the command line.
2. EvenOdd: Given an element order this will print out all of the elements, their inverses and whether they are even or odd permutations.
3. GenerateGroup: Given an element this will produce the cyclic group formed by that element.
4. CreateGroupTable: Several ways to create a group based on the command line including:
  * The Permutation Group Sn where n is the command line argument.
  * The Alternating Group An (even elements of Sn) where n is the command line argument.
  * The Dihedral Group consisting of reflections and rotations of a regular polygon where n gives the number of vertices.
  * The Rotation Group consisting of all rotations of a regular polygon where n gives the number of vertices.
  
  
