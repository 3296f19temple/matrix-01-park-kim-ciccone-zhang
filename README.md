


## Parallel Computing

### Architecture



### Algorithms

1. Single Instruction Multiple Data (SIMD)
    * 

2. Symmetric Multiprocessing
    * 

3. Distributed Memory
    * 

## Project Life Cycle

Parallel Computing

Architecture

The foundational core of this program is spread across multiple files to create one cohesive parallel computing environment. The program is dependent on a source of matrices from which its makes computations of, writing its results to a designated output file. For these matrices, we defined a unique method to automatically generate an NxN matrix.

The program is then executed with these files being passed as arguments to the command. The MPI implementation is accessed via the mmult_mpi_omp.c file and all other algorithms are accessed via the mmult_omp_timing.c file.

The program will run these computations based on the desired algorithm and write results to the local machine. From there, the data is extrapolated across a range of matrices and plotted using GNUPlot to delineate the trends between computation time and thread count.

Algorithms

Single Instruction Multiple Data (SIMD)

This algorithm involves condensing complex code into one single instruction. Multiple memory addresses are loaded into the RAM to allow for quick access and retrieval of variables. The program will then take this instruction and perform the same operation simultaneously across multiple data. Found in mmult.c.
Symmetric Multiprocessing

In this implementation, a program utilizes a shared memory across multiple processors. This allows for concurrent programming but leads to the infamous problem of race conditions. This algorithm is coded using OpenMP in mmult_omp_timing.c.
Distributed Memory

This algorithm is arguably the most powerful of the bunch, utilizing an entire network of high-speed computational machines. This is made possible using MPI technology linking multiple independent computers to form one cluster. This cluster was accessed via Temple's chinook server using MPI commands found in mmult_mpi_omp.c
Project Life Cycle

For this assignment, we tracked our progress on a project board created on Trello. We used this board by first creating a list of all the small features we had to implement. The team members then individually picked which of these they wanted to implement and moved them into the in-progress column. Eventually, once the feature was fully coded and pushed onto the master branch the to-do was marked as completed.

The usual flow of code development was definitely altered during this project. As only a powerful machine made for cluster networks can implement MPI, the code could not be simply ran on a personal computer. As such, some adjustments to the project development were needed. The native UI of an IDE is much more preferable to the vim editor on Unix, so code was developed locally on a personal machine first. The finished code was then pushed to the remote repository and the changes subsequently pulled from the Wolfgand machines to test.

This was a very time consuming process -- as a simple typo would require correction on the local repository, pushing to the remote repository, and once again pulling the changes. Despite this, a firm foundational knowledge of C allowed for a minimal amount of these encounters and did not hinder the overall functionality of the project.

We planned on a very test-driven process but not full TDD. We scripted our functions and then would test immediately after, accounting for a myriad of variables -- including but not limited to: speed, memory consumption, and matrix validity. Additionally, we especially made certain to test any references to files and whether they return null upon opening, as this would be very damaging to the program if it were to occur.

For our testing, we generated square matricies of varying sizes to text files, and modified the code to accept these as input. This way, we could ensure the same calculations were used in gathering the data. The compare_matrices method was used across all the implementations in mmult_omp_timing to ensure the calculations were coming to the same results. To ensure the SIMD implementation provided was using vectorized data correctly, a test was written to print out the memory addresses being accessed.

Graphs were generated from the collected data using GNUPLOT.

The proportion of implementation work and testing work was approximately a 50 / 50 proportion. Many implementations were provided, though we did modify them some. Only skeleton code was provided for the MPI implementation, so this constituted the most work with regard to coding the calculation methods. Several methods and classes were written and used to assist with testing.