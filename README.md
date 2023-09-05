1. **Parse Command-Line Arguments**
    - Create a function to parse command-line arguments.
    - Extract the number of processes to create and the sub-directory name containing text files.
    - Ensure error handling for incorrect or missing arguments.

2. **Retrieve List of Files**
    - Implement code to retrieve a list of file names in the specified sub-directory.
    - Store these file names in an array for further processing.

3. **Set Up Ring Communication Structure**
    - Use provided functions like `add_new_node` and `make_trivial_ring` to establish the ring communication structure.
    - Create a ring of processes, including the main process.

4. **Divide Work Among Processes**
    - Divide the list of files among the processes, ensuring each process has a subset of files to analyze.
    - Determine how to distribute the work evenly among processes.

5. **Character Frequency Analysis**
    - Implement the `procFileCounts` function to count character frequencies in a given file.
    - Ensure that the analysis is case-insensitive by converting uppercase letters to lowercase.
    - Update the frequency counts in an array for each character.

6. **Implement Communication**
    - Set up communication mechanisms between processes in the ring structure.
    - Define how processes will share and aggregate their character frequency counts.
    - Ensure synchronization and data exchange between processes are correctly implemented.

7. **Calculate Maximum Character Count**
    - Calculate the maximum character count among all processes.
    - This value will be used to scale the star visualization.

8. **Generate Star Visualization**
    - Implement a function to create a star visualization for character frequencies.
    - Use the formula `(char_count / max_count) * BARLENGTH` for visualization, where `char_count` is the count for a character, `max_count` is the count for the most frequent character, and `BARLENGTH` is the maximum size of the chart.

9. **Display Results**
    - Output the final character frequencies and the star visualization.
    - Ensure that the results are presented clearly, with frequencies for each character and the visualization.

10. **Error Handling and Cleanup**
    - Implement error handling throughout the program to handle edge cases, such as file not found or process creation failures.
    - Ensure that resources are properly released, including file handles and allocated memory.

11. **Testing and Debugging**
    - Thoroughly test your program with different inputs, including a variety of text files and different numbers of processes.
    - Debug any issues that arise during testing to ensure correctness and reliability.

12. **Documentation**
    - Document your code thoroughly, including comments and explanations of the program's logic.
    - Provide clear instructions on how to compile and run your program.

14. **Optional Enhancements**
    - Consider adding additional features or optimizations to your program, such as parallelism or performance improvements, if time permits and it aligns with your project goals.
