# CSP-Sudoku-Solver

## Technical Background

### Files

This is a C++ solution to solving Sudoku puzzles given as text input. The program can be run in three different modes by passing a flag as a command line argument: `B`, `B+FC`, or `B+FC+H`. Each mode corresponds to its own combination used by the solver: 

- `B`: Basic backtracking search (no forward checking, random variable order and random value order)
- `B+FC`: Backtracking search with forward checking (random variable order and random value order)
- `B+FC+H`: Backtracking search with forward checking and 3 heuristics (Most Constrained Variable, Most Constraining Variable, Least Constraining Value) to order variables and values (remaining ties are broken in the order of the variables and values at random)

The program can be run on its own or using the Bash script `sudokuRuns`, which runs the program multiple times and collects statistics on the time taken and the number of nodes expanded for each run.

Additionally, some puzzles have also been included as text files in the `puzzles` folder. The puzzles are named according to their difficulty level, where `0`s represent empty cells.

### Running the C++ Code

You can directly execute the C++ code associated with the Sudoku solver on its own. If not already present, first use GCC to compile the C++ code with the -O3 optimization flag:

``g++ sudokuSolver.cpp -o sudokuSolver -O3``

Note that this compilation command may differ for your system. For instance, explicitly compiling with GCC version 13.1.0 on MacOS would look like ``g++-13 sudokuSolver.cpp -o sudokuSolver -O3``. 

This compilation will create an executable file named ``sudokuSolver``. To run the program, pass the desired mode as a command line argument and provide the input puzzle via standard input:

``./sudokuSolver [COMBINATION] < puzzle.txt``

For example, to run the program in mode B, which is basic backtracking, on the Hard puzzle use the following command:
    
``./sudokuSolver B < puzzles/hard.txt``

The input puzzle should be in the form of a 9x9 grid of numbers, where 0 represents an empty cell. The program will output the solved puzzle if a solution exists, along with the number of nodes expanded during the search process.

### Running the Bash Script

Assuming that the Sudoku solver has been compiled, the Bash script ``sudokuRuns`` can be used to run the Sudoku solver program multiple times and collect statistics on the time taken and the number of nodes expanded for each run. To use the script, first make sure that it is executable:

``chmod +x sudokuRuns``

Then run the script:

``./sudokuRuns``

The script will run the Sudoku solver program `runs` number of times on the input file and output the average and standard deviation of the time taken and the number of nodes expanded over all runs during the end. The aforementioned statistics will also be outputted using the runs so far if the script is interrupted before all runs are completed, in the case that the program is taking too long to run. The number of runs can be changed by modifying the runs variable in the script. The number of runs can be changed by modifying the `runs` variable on line 4. The combination and input file can be changed by modifying the input variable in the script on line 63. For example, to run the program in mode B on the input file hard.txt, change the line to:

``output=$(./sudokuSolver B < hard.txt)``

Here’s an example of what the output of the Bash script might look like after completing a set of runs:

```
Average Time: 0.013720
Standard Deviation Time: 0.005807
Average Nodes Expanded: 70.200000
Standard Deviation Nodes Expanded: 9.123596
```

## Theoretical Background

### Preface

A quick refresher on Sudoku: https://en.wikipedia.org/wiki/Sudoku

Solving Sudoku is a NP-Complete problem. This means that it is computationally difficult to solve and there is no known algorithm that can solve it in polynomial time. However, there are many techniques that can be used to solve Sudoku puzzles efficiently.

### Formalizing Sudoku as a Constraint Satisfaction Problem (CSP)

Sudoku can be modeled as a Constraint Satisfaction Problem (CSP). A CSP is defined as {V,D,C} where

- V = {V1, V2, ..., Vn} is a set of variables
- D = {D1, ..., Dn} is the set of domains, Di is the domain of possible values for variable Vi. 
- C = {C1, ..., Cm} is the set of constraints

Each constraint involves some subset of the variables and specifies the allowable combinations of values for that subset. A state is an assignment of values to some or all of the variables {Vi = xi, Vj = xj, ...}, an assignment is consistent if it does not violate any constraints, and asolution is a complete, consistent assignment.

So we can formalize Sudoku as a CSP as follows: 
- 81 variables, Vij, i = 1 to 9, j = 1 to 9
- Domain of each variable is {1, 2, 3, 4, 5, 6, 7, 8, 9}
- Constraints: 
    – All the values in a row must be different.
    – All the values in a column must be different.
    – All the values in a 3x3 sub-grid must be different.

### Solving Sudoku Faster with Forward Checking and CSP Heuristics

A traditional solution to solving Sudoku is backtracking. Backtracking is a depth-first search algorithm that can be used to solve Sudoku puzzles. It involves assigning numbers to empty cells one by one and checking if the same number is not present in the current row, column, or 3x3 subgrid. If the number is not present, it is assigned to the cell and the process is repeated recursively until a valid solution is obtained. If none of the numbers from 1 to 9 fit in a cell, the algorithm leaves the cell blank and goes back to the previous cell. This is certainly not fast, and is O(9^m) in the worst-case where there are unfilled cells. 

We can improve backtracking by adding forward checking. Forward checking is a technique that can be used to improve backtracking by reducing the search space. It involves keeping track of the remaining legal values for unassigned variables and updating them whenever a variable is assigned. This helps to detect infeasible solutions earlier and reduces the amount of backtracking required. In the context of Sudoku, forward checking would work by keeping track of the remaining legal values for each empty cell. Whenever a number is assigned to a cell, the algorithm would update the remaining legal values for all cells in the same row, column, and 3x3 subgrid by removing the assigned number from their list of legal values. If any cell has no remaining legal values, it means that the current assignment is infeasible and the algorithm needs to backtrack. 

We can also introduce some CSP heuristics: Most Constrained Variable (MCV), Most Constraining Variable (MCV), and Least Constraining Value (LCV) to further improve our effort in solving Sudoku puzzles. MCV selects the variable with the fewest remaining legal values, while MCV selects the variable that is involved in the largest number of constraints on other unassigned variables. LCV selects the value that rules out the fewest choices for neighboring variables. In the context of Sudoku, these heuristics can be used to select which empty cell to fill next and which number to assign to it. For example, MCV would select the empty cell with the fewest remaining legal values, while MCV would select the empty cell that is involved in the largest number of constraints on other empty cells. LCV would select the number that rules out the fewest choices for neighboring empty cells. These heuristics can help reduce the search space and improve efficiency.

### Observations 

Using the program defined above, each combination of solely backtracking, backtracking and forward checking, and backtracking and forward checking and the CSP heuristics was ran 50 times on various puzzle difficulties ranging from easy to evil. 

#### Time to Termination (seconds)

```
+--------+------------------------+----------------------+--------------------+
|        | B                      | B+FC                 | B+FC+H             |
+--------+------------------------+----------------------+--------------------+
| Easy   | 0.387055±0.253597      | 0.064769±0.029627    | 0.011792±0.003214  |
+--------+------------------------+----------------------+--------------------+
| Medium | 1.222995±0.661525      | 0.177574±0.104769    | 0.011972±0.001764  |
+--------+------------------------+----------------------+--------------------+
| Hard   | 172.799754±89.550537   | 5.727973±3.476126    | 0.013975±0.003251  |
+--------+------------------------+----------------------+--------------------+
| Evil   | 1240.256768±627.874612 | 44.627875±30.167156  | 0.013720±0.005807  |
+--------+------------------------+----------------------+--------------------+

```

#### Number of Nodes Expanded (seconds)

```
+--------+----------------------------+---------------------+-------------+
|        | B                          | B+FC                | B+FC+H      |
+--------+----------------------------+---------------------+-------------+
| Easy   | 483384.60±319584.62        | 1013.10±567.95      | 45.00±0.00  |
+--------+----------------------------+---------------------+-------------+
| Medium | 1482782.68±804771.51       | 3821.10±2515.02     | 62.88±12.20 |
+--------+----------------------------+---------------------+-------------+
| Hard   | 135724569.44±69065175.01   | 133018.64±80322.08  | 93.28±32.57 |
+--------+----------------------------+---------------------+-------------+
| Evil   | 1105733095.35±343386446.93 | 880677.72±555273.05 | 70.20±9.12  |
+--------+----------------------------+---------------------+-------------+
```

### Results 

Observing the data after several runs of the algorithm over various puzzle difficulties, we can see a clear trend of decreasing time and number of nodes expanded as we move from basic backtracking search to backtracking search with forward checking and finally to backtracking search with forward checking and heuristics. This trend is consistent across all difficulty levels (easy, medium, hard, and evil). The introduction of forward checking appears to have had a significant impact on the performance of the algorithm. The detection of conflicts earlier in the search process had greatly reduced the  number of nodes as a result. leading to faster solution times. The addition of heuristics also appears to have improved the performance of the algorithm even further. The incorporation of the Most Constrained Variable, Most Constraining Variable, and Least Constraining Value heuristics clearly helped to reduce the search space by guiding the search towards more promising areas of the search space and avoiding conflicts. As a result, we see even faster solution times and fewer nodes expanded when these heuristics are used.

The magnitude of the performance improvements achieved by the introduction of forward checking and 3 heuristics is quite significant. For example, for the Evil difficulty level, the average time taken to solve a puzzle decreased from an average of ~1240 seconds for basic backtracking search to ~45 seconds for backtracking search with forward checking and finally to just ~0.01 seconds for backtracking search with forward checking and heuristics. This represents a reduction in solution time of over 97% when forward checking is introduced and over 99% when heuristics are also used. Similarly, for the Evil difficulty level, the average number of nodes expanded decreased from 1105733095 for basic backtracking search to 880677 for backtracking search with forward checking and finally to just 70 for backtracking search with forward checking and heuristics. 

Overall, these results demonstrate the effectiveness of modeling Sudoku as a Constraint Satisfaction Problem (CSP) and using techniques such as forward checking and heuristics to improve the performance of backtracking search algorithms. By representing Sudoku as a CSP, we are able to leverage powerful techniques from the field of constraint satisfaction to significantly reduce the search space and improve the efficiency of the algorithm. The data shows that these techniques can lead to large improvements in solution times and number of nodes expanded, making it possible to solve even the most difficult Sudoku puzzles quickly and efficiently.
