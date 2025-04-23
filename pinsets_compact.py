import struct

NUM_ELEMENTS = 14
pinset_format = "14i"  # Format for reading 14 integers
row_format = "14i"  # Format for reading 14 integers


unique_rows = []
row_to_index = {}

def get_or_add_row_index(row):
    """Get index of a row if it exists, otherwise add it."""
    if row not in row_to_index:
        index = len(unique_rows)
        unique_rows.append(row)
        row_to_index[row] = index
    return row_to_index[row]


# Reading rows from binary file
pinset_to_row_indices = []
with open("rows.bin", "rb") as f2:
    all_rows = []

    i = 0
    while True:

        row_indices = []
        b = False

        for j in range(NUM_ELEMENTS):

            data = f2.read(struct.calcsize(row_format))  # Read the size of one row
            if not data:
                b = True
                break  # End of file
            row = struct.unpack(row_format, data)  # Unpack binary data into a tuple

            row_indices.append(get_or_add_row_index(row))

        if b == True:
            break

        pinset_to_row_indices.append(row_indices)

        i += 1
        if (not (i % 100000)):
            print(i)


print("Unique Row Count")
print(len(unique_rows))

print("example indexes")
print(pinset_to_row_indices[20000])

print("corresponding matrix")
for i in range(14):
    print(unique_rows[pinset_to_row_indices[20000][i]])


# Write unique rows to binary file
with open("unique_rows.bin", "wb") as f:
    for row in unique_rows:
        f.write(struct.pack(row_format, *row))

# Write pinset-row index mappings to binary file
with open("pinset_mappings.bin", "wb") as f:
    for row_indices in pinset_to_row_indices:
        f.write(struct.pack("14i", *row_indices))

print("Unique rows and pinset mappings successfully written to binary files.")

# Reading pinsets from binary file
with open("pinsets.bin", "rb") as f:
    all_pinsets = []
    while True:
        data = f.read(struct.calcsize(pinset_format))  # Read the size of one pinset
        if not data:
            break  # End of file
        pinset = struct.unpack(pinset_format, data)  # Unpack binary data into a tuple
        all_pinsets.append(pinset)

print("corresponding pinset")
print(all_pinsets[20000])