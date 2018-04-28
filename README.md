# ParallelProgramming
This directory consists of a group of console applications used to demonstrate parallel programming techniques such Pthreads, MPI, and OpenMP.
 
# General Notes on Parallel Computing
-We use multi-core ICs now because the limit of transistor size and heat dissipation is quickly being reached. <br />
-We can parallelize serial programs to make them faster on new processors. <br />

## Parallel Programs
-Work is coordinated between cores. <br />
-Cores communicate with one another. <br />
-Loads the cores have to process are balanced (Load Balancing). <br />
-Cores are synchronized so they run on similar time scales. <br />
-Concurrent programs have multiple tasks in progress at any instant. <br />
-Parallel and distributed programs usually have tasks that execute simultaneously. <br />
-Three methods of parallel programming covered: MPI, Pthreads, and OpenMP. <br />

### MPI (Message-Passing Interface)
-distributed memory systems <br />
-library with type definitions, functions, etc. for C <br />
-provides methods for sending messages <br />

### Pthreads (POSIX threads)
-shared memory systems <br />
-library with type definitions, functions, etc. for C <br />
-provides methods for accessing shared-memory locations <br />

### OpenMP
-shared memory systems <br />
-library and C compiler modifications <br />
-provides methods for accessing shared-memory locations <br />

## Memory Systems
-Distributed - cores have private memories <br />
-Shared - cores have access to the same memory <br />

# General Notes on Parallel Software and Parallel Hardware Background Information
## Von Neumann Architecture
-Consists of main memory, CPU/Core, and an interconnection between the two. <br />
### Main memory
-It is a collection of locations. <br />
-Every location consists of an address. <br />
-Instructions and data are stored in locations. <br />

### Central Processing Unit
-Divided into a Control Unit and an Arithmetic and Logic Unit (ALU) <br />
-Data is stored in special fast-storage called Registers. <br />
-The separation is the Von Neumann Bottleneck - <br />
&nbsp; Execution times are traditionally much faster than fetch times. <br />

#### Control Unit
-Decides which instructions to execute. <br />
-The Control Unit has a register called the Program Counter - <br />
&nbsp; The Program Counter stores the address of the next instruction. <br />

#### ALU
-Resonsible for executing the actual instructions. <br />

#### Interconnection (Bus)
-Collection of parallel wires. <br />
-Data is fetched and read from memory. <br />
-Data is written and stored to memory. <br />

## Threading
-Programs execute commands on what is called a Time Slice. <br />
-When a process is wating for a resource it blocks. <br />
-When on thread blocks, another thread can execute. <br />
-Switching between threads can be faster in some OS than switching processes. <br />

## Caching
-Caches are collections of memory locations that are quickly accessible. <br />
-Takes advantage of both Spatial (physically nearby) and Temporal (accessed in the near future) Locality. <br />
-Have different levels (L1, L2, etc.) <br />
-L1 miss, L2 hit for L2 access in many cases. <br />

## Parallel Software
-Focus on Homogeneous Multiple Instruction, Multiple Data (MIMD) systems. <br />
-Programs will be Single Program, Multiple Data (SPMD). <br />
-Need to address Load Balance, Communication, and Synchronization. <br />

### Shared-Memory
-Will have Private and Shared Variables. <br />
-Problems will be Mutual Exclusion Lock (Mutex) and Thread Safety. <br />

### Distributed-Memory
-Will use Message Passing. <br />
-Messages use Send and Receive Functions. <br />
-Send can Block. <br />
-Can also be programmed using One-Sided Communications. <br />
-Partitioned Global Address Space Languages provide some shared-memory funtionality. <br />

## Performance
-Tparallel = Tserial/p, where p = threads or cores <br />
-Speedup = Tserial/Tparallel <br />
-Efficiency = Speedup/p <br />
-Toverhead = Tparallel - Tserial/p <br />
-Measuring Time of Execution - <br />
&nbsp; start = get_curr_time(); <br />
&nbsp; /* Code of interest */ <br />
&nbsp; finish = get_curr_time(); <br />
&nbsp; print finish - start; <br />
-Getting the (wall clock) time - <br />
&nbsp; MPI => MPI_Wtime <br />
&nbsp; OpenMP => omp_get_wtime <br />
-Synchronize all processes, then get the individual times of all processes. <br />
-Take the maximum time of the individual processes. <br />
-Run the program multiple times. <br />
-Take the minimum time from multiple runs as the runtime. <br />
