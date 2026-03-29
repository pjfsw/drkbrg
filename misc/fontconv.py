import sys

def convert_bdf_to_h(input_bdf, output_h, array_name="font_8x8"):
    # Initialize a blank array for 256 characters, 8 bytes each
    font_data = [0] * 2048

    try:
        with open(input_bdf, "r") as f:
            lines = f.readlines()
    except FileNotFoundError:
        print("Error: Input BDF file not found.")
        return

    current_encoding = -1
    in_bitmap = False
    row_idx = 0

    for line in lines:
        line = line.strip()
        
        if line.startswith("ENCODING"):
            parts = line.split()
            if len(parts) > 1:
                current_encoding = int(parts[1])
        elif line == "BITMAP":
            in_bitmap = True
            row_idx = 0
        elif line == "ENDCHAR":
            in_bitmap = False
        elif in_bitmap:
            # Parse only if it's a standard ASCII character and within the 8-row limit
            if 0 <= current_encoding < 256 and row_idx < 8:
                # BDF bitmap data is hex. Take the first 2 chars for 8-pixel width
                font_data[current_encoding * 8 + row_idx] = int(line[0:2], 16)
                row_idx += 1

    # Generate the C header file
    with open(output_h, "w") as f:
        f.write(f"const unsigned char {array_name}[2048] = {{\n")
        for i in range(256):
            f.write(f"    /* ASCII {i} */\n    ")
            for j in range(8):
                f.write(f"0x{font_data[i * 8 + j]:02X}, ")
            f.write("\n")
        f.write("};\n")
    
    print(f"Successfully converted {input_bdf} to {output_h}")

if __name__ == "__main__":
    if len(sys.argv) > 2:
        convert_bdf_to_h(sys.argv[1], sys.argv[2])
    else:
        print("Usage: python bdf_converter.py <input.bdf> <output.h>")
