import glob
from hack_assembler import assemble  # Import the assemble function

def batch_assemble():
    asm_files = glob.glob("*.asm")  # Get all .asm files in the current directory
    for asm_file in asm_files:
        hack_file = asm_file.replace(".asm", ".hack")
        assemble(asm_file, hack_file)  # Compile each .asm to .hack
        print(f"Compiled {asm_file} to {hack_file}")

# Run the batch assembler
if __name__ == "__main__":
    batch_assemble()