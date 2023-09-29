import os
import subprocess

MAX_FILES = 100
NUM_RUNS = 10


PAPI_EVENTS = [
    "L1 Misses",
    "Clock Cycles PAPI",
    "Instructions Executed",
    "Clock Cycles TSC"
]

# Additional library path and library name
library_path = "/home/zemor/papi-7.0.1/src"
library_name = "/home/zemor/papi-7.0.1/src/libpapi.a"  # Replace with the actual library name

def compile_and_run_with_timing(filename, num_loops, event):
    compile_command = ["gcc", "-g", "-Wall", f"-I{library_path}", filename, library_name, "-o", "temp", "-lm"]

    if subprocess.run(compile_command).returncode != 0:
        print("Compilation failed.")
        return

    totalaverage = 0
    for _ in range(NUM_RUNS):

        execute_command = ["./temp", str(num_loops), str(event)]
        result = subprocess.run(execute_command, stdout=subprocess.PIPE)

        output = result.stdout.decode('utf-8').strip().split('\n')

        output_integers = [int(value) for value in output]
        average = sum(output_integers) / len(output_integers)

        chosen_event = PAPI_EVENTS[event]
        # Print the average
        #print(f"Average {chosen_event}: {average}")

        totalaverage += average
        #print(output)
    print(f"\nTest {filename} with {num_loops} iterations")
    print(f"Real Average {chosen_event}:", totalaverage / NUM_RUNS)
    # Clean up the temporary executable
    subprocess.run(["rm", "temp"])

def main():
    filenames = []
    num_files = 0

    # Automatically scan and list C code files in the current directory
    for entry in os.listdir("."):
        if num_files < MAX_FILES and entry.endswith(".c") and entry != "run.c":
            filenames.append(entry)
            num_files += 1
    # Sort the filenames alphabetically
    filenames = sorted(filenames)

    while True:
        print("\nChoose an option:")
        print("1. List available C code files")
        print("2. Run C code")
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
                print("Enter the number corresponding to the C code file to run (or '0' to cancel): ")
                file_number = int(input())

                if 1 <= file_number <= num_files:
                    print("Enter the number of loops for the selected file: ")
                    num_loops = int(input())

                    print("Select the event to monitor during execution: ")
                    for i, event in enumerate(PAPI_EVENTS, start=1):
                        print(f"{i}. {event}")
                    event = int(input()) - 1

                    compile_and_run_with_timing(filenames[file_number - 1], num_loops, event)
                elif file_number != 0:
                    print("Invalid file number. Please enter a valid option.")
        elif choice == 3:
            exit(0)
        else:
            print("Invalid choice. Please enter a valid option.")

if __name__ == "__main__":
    main()
