----------
 Overview
----------
This directory contains some files for modifying the serval library to count the number of instructions it takes to run a function.

This repository tests a sorting function and a hash table implementation by calculating the number of instructions as a function of some input arguments. Rosette is then used to query arguments that minimize and maximize the number of instructions.

In the hash table implementation, maximizing the number of instructions means choosing hash keys that all hash to the same table index. Minimizing the number of instructions means choosing hash keys that all hash to different table indexes.

In the selection sort implementation, minimizing the number of instructions means choosing a list that is already sorted. Unfortunately quick-sort implementation is too complex to run the analysis on arrays longer than 2 elements.

------------------
 Running The Code
------------------
The Makefile works by copying modified files to the unsat/serval-tools docker container and running sort.c and sort.rkt as though they are a test.

Serval Github: https://github.com/uw-unsat/serval
Serval Docker Container: https://hub.docker.com/r/unsat/serval-tools

When starting the docker container, go to /serval/ and make sure to run the following commands:
  raco pkg install --auto
  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/lib64"

If you cd to /serval/tests/ and run "make all", it should pass all tests successfully. If not, look at ".gitlab-ci.yml" in the serval repository for setup commands.

Once the unsat/serval-tools docker container is running, you can use the Makefile to copy the relevant files form this directory and run the relevant commands to test the program. Just make sure to use "docker ps" to get your container's ID and set it in this Makefile.

Use the command "make" in this directory and you should see results print to the console.

-----------------------
 Setting the Test Case
-----------------------
Testing functions are defined in sort.c and I got them to work with integer arguments. There might be ways to pass in arrays or pointers, but I couldn't figure it out. See test_quicksort_3, test_insertion_sort_3, and test_hash_3 for examples.

The Makefile in the docker container's /serval/test/Makefile describes how c files are compiled. The flags are pretty complex which is why I copy files over as a test case of serval instead of compiling the project in this directory.

The functions designed in sort.c are tested in sort.rkt. Inside the (parameterize) block which creates the llvm machine, a function from sort.c is run with symbolic arguments giving a symbolic representation of the number of instructions. This value is then minimized and maximized to find example arguments for best and worst case runtimes.

Currently the code scales very poorly. For example, quicksort runs almost instantly for arrays of length 2 but causes the Makefile to time out for arrays of length 3. Future improvements probably mean enforcing restrictions on the input arguments so the solver can run faster.

-------------------------
 Modifications to serval
-------------------------
I had to modify serval a little to add instruction counting. I modified llvm.rkt and parse.rkt, and these files are copied to the docker container before

In llvm.rkt, I first added the field instr-count to the machine struct which keeps track of the current number of executed instructions.

I also added the function (current-instr-count) which returns the current instruction count and (incr-current-instr-count!) which increments the current instruction count by 1.

In many of the following instructions, I added (incr-current-instr-count!) so running the instruction also increments the instruction count. I didn't get to all the instructions so you might want to add incrementing to the rest, but the code still seems to count instructions pretty well.

In parse.rkt, I didn't make any modifications but I added a few print statements to help with debugging. It probably isn't needed and can be removed from this directory.
