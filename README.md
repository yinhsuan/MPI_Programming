# MPI_Programming

### Part 1: Getting Familiar with MPI Programming

### Q1

### 1-1. How do you control the number of MPI processes on each node? (3 points)


#### According to the [reference](https://www.open-mpi.org/doc/v4.0/man1/mpirun.1.php):
```C
mpirun [ -np X ] [ --hostfile <filename> ] <program>
```
- -np: Run **X copies of program** in the current     run-time environment.
- \-\-hostfile: A slot is an allocation unit for a process. The number of slots on a node indicate **how many processes** can potentially execute on that node.

#### Therefore, when we execute the command, for example:

```shell
mpirun -np 4 --hostfile hosts ./matmul < /home/.grade/HW4/data-set/data1_1
```
#### with the host file:
```shell
pp2 slots=1
pp3 slots=1
pp5 slots=1
pp7 slots=1
```
#### means that we would have 4 copies of program to be executed. And each node (ex: pp2/ pp3/ pp5/ pp7) would have <u>**one process**</u> seperately to execute the program.


```diff
+ A1-1: Use `-np` to specify the total copies of program to be executed. And then use `--hostfile` to specify which nodes to be used and the number of slots (MPI processes) to be executed on each node.
```

### 1-2. Which functions do you use for retrieving the rank of an MPI process and the total number of processes? (2 points)

:::info
#### A1-2:
#### A. To find out process rank:
```C
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
```
#### B. To find out number of process
```C
MPI_Comm_size(MPI_COMM_WORLD, &world_size);
```
:::

### Q2

### 1. Why MPI_Send and MPI_Recv are called “blocking” communication? (2 points)

```diff
+ A2-1: 
+ Because these two funciton <u>**will not return**</u> until they finish communication. Therefore, the process would be blocked when these two function calls.

+ The process can continue its execution only after these two function(MPI_Send and MPI_Recv) returns.
```

### 2. Measure the performance (execution time) of the code for 2, 4, 8, 12, 16 MPI processes and plot it. (1 points)

#### A. Table 1:
| Process#  | Execution Time | Speed Up | Node [Slots] |
| ------------- | ------------- | ------------- | ------------- |
| 1  | [12.310] s  | 1.000x  | pp2[slots=1]  |
| 2  | [6.144] s  | 2.004x  | pp2[slots=1] pp3[slots=1]  |
| 4  | [3.327] s  | 3.700x  | pp2[slots=2] pp3[slots=2]  |
| 8  | [1.572] s  | 7.831x  | pp2[slots=2] pp3[slots=2] pp4[slots=2] pp5[slots=2]  |
| 12  | [1.076] s  | 11.443x  | pp2[slots=3] pp3[slots=3] pp4[slots=3] pp5[slots=3]  |
| 16  | [0.824] s  | 14.948x  | pp2[slots=4] pp3[slots=4] pp4[slots=4] pp5[slots=4]  |

#### B. Graph 1:
![](https://i.imgur.com/7bVKIaR.png)





### Q3

### 1. Measure the performance (execution time) of the code for 2, 4, 8, 16 MPI processes and plot it. (1 points)

#### A. Table 2:
| Process#  | Execution Time | Speed Up | Node [Slots] |
| ------------- | ------------- | ------------- | ------------- |
| 1  | [12.828] s  | 1.000x  | pp2[slots=1]  |
| 2  | [6.419] s  | 1.999x  | pp2[slots=1] pp3[slots=1]  |
| 4  | [3.387] s  | 3.788x  | pp2[slots=2] pp3[slots=2]  |
| 8  | [1.717] s  | 7.470x  | pp2[slots=2] pp3[slots=2] pp4[slots=2] pp5[slots=2]  |
| 16  | [0.996] s  | 12.882x  | pp2[slots=4] pp3[slots=4] pp4[slots=4] pp5[slots=4]  |

#### B. Graph 2:
![](https://i.imgur.com/e40nAK7.png)

### 2. How does the performance of binary tree reduction compare to the performance of linear reduction? (3 points)

![](https://i.imgur.com/3m5XQKW.png)

#### According to the result of Table 1 & Table 2, we can observe that the performance are almost the same. However, if we put those two table in the same diagram, we can immediately see that the execution time of PI_Block_Tree is slightly larger. Therefore, the performance of PI_Block_Linear is better.


```diff
+ A3-2: The performance of <u>**linear reduciton**</u> is better then binary tree reduction.
```

### 3. Increasing the number of processes, which approach (linear/tree) is going to perform better? Why? Think about the number of messages and their costs. (3 points)

#### The linear approach is going to perform better because the tree approach needs to execute log2(N) rounds of `send`, `receive` and `addition` operations. And each round, the number of nodes evolved in the operation would be half of the previous round. 

#### That is to say, if we have 8 nodes in the first round of operation, we would have 4 nodes to send the message, and 4 nodes to receive the message and add the received number into its own result. And then in the next round, we would have 4 nodes to do the operation (2 nodes for sending the message and 2 nodes for reveiving and adding the message). And so on.

#### However, the linear approach only need to execute those operation once.

```diff
+ A3: <u>**Linear**</u> is going to perform better.
```

### Q4

### 1. Measure the performance (execution time) of the code for 2, 4, 8, 12, 16 MPI processes and plot it. (1 points)

#### A. Table 3:
| Process#  | Execution Time | Speed Up | Node [Slots] |
| ------------- | ------------- | ------------- | ------------- |
| 1  | [12.149] s  | 1.000x  | pp2[slots=1]  |
| 2  | [6.120] s  | 1.985x  | pp2[slots=1] pp3[slots=1]  |
| 4  | [3.137] s  | 3.873x  | pp2[slots=2] pp3[slots=2]  |
| 8  | [1.569] s  | 7.743x  | pp2[slots=2] pp3[slots=2] pp4[slots=2] pp5[slots=2]  |
| 12  | [1.059] s  | 11.477x  | pp2[slots=3] pp3[slots=3] pp4[slots=3] pp5[slots=3]  |
| 16  | [1.041] s  | 11.669x  | pp2[slots=4] pp3[slots=4] pp4[slots=4] pp5[slots=4]  |

#### B. Graph 3:
![](https://i.imgur.com/qjNUyfZ.png)



### 2. What are the MPI functions for non-blocking communication? (2 points)

```diff
+ A4-2:

| Function  | Description | Reference |
| ------------- | ------------- | ------------- |
| MPI_Isend  | MPI_Isend starts a nonblocking send  | [link](https://www.open-mpi.org/doc/v4.0/man3/MPI_Isend.3.php)  |
| MPI_Irecv  | MPI_Irecv starts a nonblocking receive  | [link](https://www.open-mpi.org/doc/v4.1/man3/MPI_Irecv.3.php)  |
| MPI_Wait  | A call to MPI_Wait returns when the operation identified by request is complete  | [link](https://www.open-mpi.org/doc/v4.0/man3/MPI_Wait.3.php)  |
| MPI_Waitany  | A call to MPI_Waitany can be used to wait for the completion of one out of several requests  | [link](https://www.open-mpi.org/doc/v4.0/man3/MPI_Waitany.3.php)  |
| MPI_Waitall  | Blocks until all communication operations associated with active handles in the list complete, and returns the status of all these operations  | [link](https://www.open-mpi.org/doc/v4.1/man3/MPI_Waitall.3.php)  |
| MPI_Test  | A call to MPI_Test returns flag = true if the operation identified by request is complete  | [link](https://www.open-mpi.org/doc/v4.0/man3/MPI_Test.3.php)  |
| MPI_Testany  | MPI_Testany tests for completion of either one or none of the operations associated with active handles   | [link](https://www.open-mpi.org/doc/v4.0/man3/MPI_Testany.3.php)  |
```

### 3. How the performance of non-blocking communication compares to the performance of blocking communication? (3 points)

| Process #  | 1 | 2 | 4 | 8 | 12 | 16 |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| Block  | [12.310] s  | [6.144] s  | [3.327] s  | [1.572] s  | [1.076] s  | [0.824] s  |
| Nonblock  | [12.149] s  | [6.120] s  | [3.137] s  | [1.569] s  | [1.059] s  | [1.041] s  |

![](https://i.imgur.com/t4kCUBa.png)


```diff
+ A4: The execution time of PI_Nonblocking_Linear is smaller than PI_Blocking_Linear in most of the case. Therefore, <u>**non-blocking communication**</u> has better performance.
```

### Q5

### Measure the performance (execution time) of the code for 2, 4, 8, 12, 16 MPI processes and plot it. (1 points)

#### A. Table 4:
| Process#  | Execution Time | Speed Up | Node [Slots] |
| ------------- | ------------- | ------------- | ------------- |
| 1  | [12.994] s  | 1.000x  | pp2[slots=1]  |
| 2  | [6.414] s  | 2.026x  | pp2[slots=1] pp3[slots=1]  |
| 4  | [3.344] s  | 3.886x  | pp2[slots=2] pp3[slots=2]  |
| 8  | [1.715] s  | 7.576x  | pp2[slots=2] pp3[slots=2] pp4[slots=2] pp5[slots=2]  |
| 12  | [1.086] s  | 11.962x  | pp2[slots=3] pp3[slots=3] pp4[slots=3] pp5[slots=3]  |
| 16  | [1.008] s  | 12.895x  | pp2[slots=4] pp3[slots=4] pp4[slots=4] pp5[slots=4]  |

#### B. Graph 4:
![](https://i.imgur.com/ps7jowZ.png)



### Q6

### 1. Measure the performance (execution time) of the code for 2, 4, 8, 12, 16 MPI processes and plot it. (1 points)

#### A. Table 5:
| Process#  | Execution Time | Speed Up | Node [Slots] |
| ------------- | ------------- | ------------- | ------------- |
| 1  | [13.412] s  | 1.000x  | pp2[slots=1]  |
| 2  | [6.528] s  | 2.055x  | pp2[slots=1] pp3[slots=1]  |
| 4  | [3.257] s  | 4.118x  | pp2[slots=2] pp3[slots=2]  |
| 8  | [1.638] s  | 8.189x  | pp2[slots=2] pp3[slots=2] pp4[slots=2] pp5[slots=2]  |
| 12  | [1.075] s  | 12.477x  | pp2[slots=3] pp3[slots=3] pp4[slots=3] pp5[slots=3]  |
| 16  | [0.824] s  | 16.270x  | pp2[slots=4] pp3[slots=4] pp4[slots=4] pp5[slots=4]  |

#### B. Graph 5:
![](https://i.imgur.com/6BgDct0.png)


### Part 2: Matrix Multiplication with MPI

### Q7

### 1. Describe what approach(es) were used in your MPI matrix multiplication for each data set.

```diff
+ A7: 

+ Broadcast matrix A and matrix B to all the process. And then, splite matrix A's row into several division (a1+a2+...+aN = A) according to process#(N). Each process would calculate part of the result of C (ex: c1 = a1 * B).

+ After finishing the calculation in each process, they would return the result (c1, c2 ... cN) to the master process (rank 0). In the end, the master process would add up the received result and generate the final result: matrix C.

```
