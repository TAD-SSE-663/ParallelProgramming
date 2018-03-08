# ParallelProgrammingP2
This solution consists of a group of console applications used to demonstrate parallel programming techniques such Pthreads, MPI, and OpenMP.
  
  
# General Notes on Parallel Computing
-We use multi-core ICs now because the limit of transistor size and heat dissipation is quickly being reached.  
-We can parallelize serial programs to make them faster on new processors.  
  
## Parallel Programs
-Work is coordinated between cores.  
-Cores communicate with one another.  
-Loads the cores have to process are balanced (Load Balancing).  
-Cores are synchronized so they run on similar time scales.  
-Concurrent programs have multiple tasks in progress at any instant.  
-Parallel and distributed programs usually have tasks that execute simultaneously.  
-Three methods of parallel programming covered: MPI, Pthreads, and OpenMP.  
  
### MPI (Message-Passing Interface)
-distributed memory systems  
-library with type definitions, functions, etc. for C  
-provides methods for sending messages  
  
### Pthreads (POSIX threads)
-shared memory systems  
-library with type definitions, functions, etc. for C  
-provides methods for accessing shared-memory locations  
  
### OpenMP
-shared memory systems  
-library and C compiler modifications  
-provides methods for accessing shared-memory locations  
  
  
## Memory Systems
-Distributed - cores have private memories  
-Shared - cores have access to the same memory  
  
  
  
# General Notes on Parallel Software and Parallel Hardware Background Information
## Von Neumann Architecture
-Consists of main memory, CPU/Core, and an interconnection between the two.  
  
### Main memory
-It is a collection of locations.  
-Every location consists of an address.  
-Instructions and data are stored in locations.  
  
### Central Processing Unit
-Divided into a Control Unit and an Arithmetic and Logic Unit (ALU)  
  
#### Control Unit
-Decides which instructions to execute.  
  
#### ALU
-Resonsible for executing the actual instructions.  
  
-Data is stored in special fast-storage called Registers.  
-The Control Unit has a register called the Program Counter -  
	The Program Counter stores the address of the next instruction.  
  
#### Interconnection (Bus)
-Collection of parallel wires.  
-Data is fetched and read from memory.  
-Data is written and stored to memory.  
  
-The separation is the Von Neumann Bottleneck -  
	Execution times are traditionally much faster than fetch times.  
  
  
## Threading
-Programs execute commands on what is called a Time Slice.  
-When a process is wating for a resource it blocks.  
-When on thread blocks, another thread can execute.  
-Switching between threads can be faster in some OS than switching processes.  
  
## Caching
-Caches are collections of memory locations that are quickly accessible.  
-Takes advantage of both Spatial (physically nearby) and Temporal (accessed in the near future) Locality.  
-Have different levels (L1, L2, etc.)  
-L1 miss, L2 hit for L2 access in many cases.  
  
## Parallel Software -
-Focus on Homogeneous Multiple Instruction, Multiple Data (MIMD) systems.  
-Programs will be Single Program, Multiple Data (SPMD).  
-Need to address Load Balance, Communication, and Synchronization.  
  
### Shared-Memory
-Will have Private and Shared Variables.  
-Problems will be Mutual Exclusion Lock (Mutex) and Thread Safety.  
  
### Distributed-Memory
-Will use Message Passing.  
-Messages use Send and Receive Functions.  
-Send can Block.  
-Can also be programmed using One-Sided Communications.  
-Partitioned Global Address Space Languages provide some shared-memory funtionality.  
  
  
## Performance
-Tparallel = Tserial/p, where p = threads or cores  
-Speedup = Tserial/Tparallel  
-Efficiency = Speedup/p  
-Toverhead = Tparallel - Tserial/p  
-Measuring Time of Execution -  
	start = get_curr_time();  
	/* Code of interest */  
	finish = get_curr_time();  
	print finish - start;  
-Getting the (wall clock) time -  
	MPI => MPI_Wtime  
	OpenMP => omp_get_wtime  
-Synchronize all processes, then get the individual times of all processes.  
-Take the maximum time of the individual processes.  
-Run the program multiple times.  
-Take the minimum time from multiple runs as the runtime.  
  
  
  
# Distributed-Memory Programming with MPI
# Shared-Memory Programming with Pthreads
# Shared-Memory Programming with OpenMP
