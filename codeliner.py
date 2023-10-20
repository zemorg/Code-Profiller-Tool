import os

MAX_FILES = 100

def read_file(filename):
    try:
        with open(f"./RawCodeExamples/{filename}", 'r') as file:
            file_contents = file.read()
        return file_contents
    except FileNotFoundError:
        return None

def main():
    filenames = []
    num_files = 0

    # Automatically scan and list C code files in the current directory
    for entry in os.listdir("./RawCodeExamples"):
        if num_files < MAX_FILES and entry.endswith(".c") and entry != "run.c":
            filenames.append(entry)
            num_files += 1
    # Sort the filenames alphabetically
    filenames = sorted(filenames)

    while True:
        print("\nChoose an option:")
        print("1. List available C code files")
        print("2. Print C code in one line format")
        print("3. Quit")

        choice = int(input())
        if choice == 1:
            if num_files == 0:
                print("No C code files available in the directory.")
            else:
                print("-----------------------------------")
                print("Available C code files:")
                for i, filename in enumerate(filenames, start=1):
                    print(f"{i}. {filename}")
                print("-----------------------------------")
        elif choice == 2:
            if num_files == 0:
                print("No C code files available. Please add some files.")
            else:
                print("Enter the number corresponding to the C code file to print it's code (or '0' to cancel): ")
                file_number = int(input())
                if 1 <= file_number <= len(filenames):
                    selected_filename = filenames[file_number - 1]
                    file_contents = read_file(selected_filename)
                    if file_contents is not None:
                        print(f"Contents of '{selected_filename}':")
                        print("------------------------------------------------------------------------")
                        print(file_contents)
                        print("------------------------------------------------------------------------")
                        print(f"Contents of '{selected_filename}' (in one line format):")
                        single_line_code = file_contents.replace('\n', '\\n')
                        print(single_line_code)
                        print("------------------------------------------------------------------------")
                    else:
                        print(f"File '{selected_filename}' not found.")
                else:
                    print("Invalid file number. Please select a valid file.")
        elif choice == 3:
            exit(0)


if __name__ == "__main__":
    main()