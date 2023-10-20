import os
import subprocess

MAX_FILES = 100
NUM_RUNS = 10

PAPI_EVENTS = {
    "L1 Misses": "PAPI_L1_DCM",
    "Instructions Executed": "PAPI_TOT_INS"
}

# Additional library path and library name
library_path = "/home/zemor/papi-7.0.1/src"
library_name = "/home/zemor/papi-7.0.1/src/libpapi.a"  # Replace with the actual library name

def compile_and_run_with_timing(filename, num_loops, event, file_number):
    fileandpath = "./CodeExamples/" + filename
    compile_command = ["gcc", "-g", "-mavx", "-Wall", f"-I{library_path}", fileandpath, library_name, "-o", "temp", "-lm"]

    print("Tests Running (0-10):", end=' ', flush=True)
    if subprocess.run(compile_command).returncode != 0:
        print("Compilation failed.")
        return

    totalaverage1 = 0
    totalaverage2 = 0
    if event >= len(PAPI_EVENTS):
        code_name = "none"
    else:
        # Get the event name and code name based on the user's input
        event_name = list(PAPI_EVENTS.keys())[event]
        code_name = PAPI_EVENTS[event_name]


    
    for i in range(NUM_RUNS):

        execute_command = ["./temp", str(num_loops), code_name]
        result = subprocess.run(execute_command, stdout=subprocess.PIPE)

        output = result.stdout.decode('utf-8').strip().split('\n')

        output_values = [int(value) for value in output]

        #print("EVENT NUMBER", event)

        # Split the list into two separate lists
        if (event > -1 and event <= len(PAPI_EVENTS)):
            output_integers1 = output_values[::2]  # Get every even-indexed element
            output_integers2 = output_values[1::2]  # Get every odd-indexed element
            #print(output_integers1)
            #print(output_integers2)

            average1 = sum(output_integers1) / len(output_integers1)
            average2 = sum(output_integers2) / len(output_integers2)
            #print(f"Average Cycles: {average1}")
            #print(f"Average2 {event_name}: {average2}")
            totalaverage2 += average2

        elif (event == -1):
            average1 = sum(output_values) / len(output_values) 
            #print(f"Average Cycles: {average1}")
        totalaverage1 += average1
        if (i<9):
            print((i+1), end=', ', flush=True)
    print("10. Done")
    print("-----------------------------------")
    print(f"Test {filename} (File Nº{file_number}) with {num_loops} loops/array size:")
    print(f"Average Clock Cycles:", totalaverage1 / NUM_RUNS)
    if (event in [0, 1, 2]):
        print(f"Average {event_name}:", totalaverage2 / NUM_RUNS)
    # Clean up the temporary executable
    print("-----------------------------------")
    subprocess.run(["rm", "temp"])

def main():
    filenames = []
    num_files = 0

    # Automatically scan and list C code files in the current directory
    for entry in os.listdir("./CodeExamples"):
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
                    print("Enter the number of loops/array size for the selected file: ")
                    num_loops = int(input())
                    if (num_loops > 0):
                        print("Select the event to monitor during execution (clock cycles are always monitored, press 0 for clock cycles only): ")
                        print("0. TSC Clock Cycles")
                        for i, event in enumerate(PAPI_EVENTS, start=1):
                            print(f"{i}. {event}")
                        event = int(input()) - 1
                        if -1 <= event < len(PAPI_EVENTS):
                            compile_and_run_with_timing(filenames[file_number - 1], num_loops, event, file_number)
                        else:
                            print("Invalid event number. Please enter a valid option")
                    else:
                        print("Invalid Loop count. Please enter a positive integer.")
                elif (file_number != 0):
                    print("Invalid file number. Please enter a valid option.")
        elif choice == 3:
            exit(0)
        else:
            print("Invalid choice. Please enter a valid option.")

if __name__ == "__main__":
    main()
