# Code-Profiller-Tool
Small tool with some example c files that can be ran and profiled semi-automatically.
To try out various example codes, use run.py, example codes will require PAPI to run.

The instrumented code to get performance data from is available inside the folder, CodeExamples.

The original functional code for each example with no instrumentation added is available inside the folder, RawCodeExamples.
To print these codes in a one-line format for LLM input, use codeliner.py.

CodeExampleDetails.ods contains a small description of the problem created by each code, and how it was fixed, 
along with performance counter data taken while executing the code in my machine (Intel Skylake).

formattedInput.ods contains the code from the examples formatted with relevant inputs for the LLM training.

Interfacing_Unfinished contains code examples that are still a work in progress, along with other things.
