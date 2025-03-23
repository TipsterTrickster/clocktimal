pinsets.py creates finds all of the valid pinsets and corresponding rows. Stores these to two separate binary files.

pinsets.bin, every 14 integers corresponds to a pinset.

rows.bin every 14 integers correponds to a matrix row, every 14 matrix rows corresponds to a pinset at index / 14 in pinsets.bin


pinsets_compact.py creates pinset_mappings.bin where every 14 integers corresponds to the index of the row in unique_rows.bin




TODO:
PARALLELLLLELL
command line args
batch solving from file
statistics, a lot of random scrambles, and one scramble
Optimize pinset generation
Optimize pinset storage
Research 12 move pinset coverage (all unique 12 movers? remove dupes?)
other clock puzzles


Command line args ideas
-m optimal movecount
-t optimal tickcount
-s optimal simulcount
-st optimal simultick
-i inputfile
-SC scramble input
-ST state input