import sys

def convert_fon_to_h(input_file, output_file, array_name="font_8x8"):
    try:
        with open(input_file, "rb") as f:
            data = f.read()

        with open(output_file, "w") as f:
            f.write(f"/* Font Data: 8x8 Fixed Width */\n")
            f.write(f"const unsigned char {array_name}[] = {{\n")
            
            for i, byte in enumerate(data):
                # Format byte as hex with padding
                f.write(f" 0x{byte:02x},")
                
                # After 8 bytes (one full 8x8 glyph), add a newline
                if (i + 1) % 8 == 0:
                    f.write("\n")
            
            f.write("};\n")
        print(f"Successfully converted {input_file} to {output_file}")
    except FileNotFoundError:
        print("Error: Input file not found.")

if __name__ == "__main__":
    # Usage: python script.py input.bin font.h
    if len(sys.argv) > 2:
        convert_fon_to_h(sys.argv[1], sys.argv[2])
    else:
        print("Usage: python script.py <input_file> <output_file>")
