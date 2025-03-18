pinsets.py creates finds all of the valid pinsets and corresponding rows. Stores these to two separate binary files.

pinsets.bin, every 14 integers corresponds to a pinset.

rows.bin every 14 integers correponds to a matrix row, every 14 matrix rows corresponds to a pinset at index / 14 in pinsets.bin


pinsets_compact.py creates pinset_mappings.bin where every 14 integers corresponds to the index of the row in unique_rows.bin