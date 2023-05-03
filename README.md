## Description
A simple operating system which allows basic interactions with the file system, executing scripts, multithreading and background execution. 

## Environment
The OS was implemented using C programming language in a Linux environment.

The following software are needed for running the OS:

1. Git (use the following tutorial https://www.atlassian.com/git/tutorials/install-git)
2. GNU C Compiler (>=9.4.0). On Mac, `gcc` calls `clang`, which should be compatible, but **you may face issues**. On Windows, you can use [cygwin](https://www.cygwin.com/) to install the GNU tools.
3. GNU Make (>=4.2.1). Same as above.

If you’d like to run the code in your own Linux virtual machine, you may need to install build essentials to be able to compile C code: <br>
```sudo apt-get install build-essential```

## Compilation
run ```make mysh``` to compile. <br>
run ```make clean``` to clean up all object files.<br>


   
   ## Shell
   ### Running the OS shell
* Interactive mode: In the command line type: ```./mysh```
* Batch mode: You can also use input files to run the shell. To use an input file, in the command line type: ```./mysh < file.txt```

### Commands

   |COMMAND |DESCRIPTION|
   |-------|--------|
   |help|Displays all the commands|
   |quit | Exits/terminates the shell |
   |set VAR STRING|Assigns a value STRING to shell memory|
   |echo [ STRING \| $VAR ]|prints STRING or value of VAR in terminal|
   |print VAR |Displays the STRING assigned to VAR|
   |my_ls| list all files present in the currect directory|
   |my_mkdir DIRNAME| creates a new directory with the name DIRNAME in the current directory|
   |my_touch FILENAME| creates a new empty file with the name FILENAME in the current directory|
   |my_cd DIRNAME| changes current directory to the directory DIRNAME|
   |run SCRIPT.TXT |Executes the file SCRIPT.TXT|
   |exec PROG1 [PROG2] [PROG3] POLICY [#] [MT] | Executes up to 3 programs concurrently according to the scheduling POLICY (optional: # allows for background execution and MT allows for multithreading)|
   
   ## Scheduling
   
   There are 5 scheduling policies:
   * FCFC (First Come First Serve)
   * SJF (Shortest Job First)
   * RR (Round Robin - step: 2)
   * RR30 (Round Robin - step: 30)
   * AGING
   
   ## Memory Managment
   










