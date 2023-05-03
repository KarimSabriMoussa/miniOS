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
run ```make clean``` to remove all object files.<br>


   
   ## Shell
   ### Running the OS shell
   The OS shell has 2 modes. An interactive mode which provides the user with a prompt and responds to commands as they are inputted and a batch mode where the user can redirect a file to the shell and it will run all commands within that script. After all the lines withing a file is executed the shell returns to interactive mode.

To start the shell in either mode can be done as follows:
* Interactive mode: In the command line type: ```./mysh```
* Batch mode: You can also use input files to run the shell. To use an input file, in the command line type: ```./mysh < file.txt```

### Commands

   |COMMAND |DESCRIPTION|
   |-------|--------|
   |help| Displays all the commands|
   |quit | Exits/terminates the shell |
   |set VAR STRING| Assigns a value STRING to shell memory|
   |echo [ STRING \| $VAR ]| Prints STRING or value of VAR in terminal|
   |print VAR | Displays the STRING assigned to VAR|
   |my_ls| List all files present in the currect directory|
   |my_mkdir DIRNAME| Creates a new directory with the name DIRNAME in the current directory|
   |my_touch FILENAME| Creates a new empty file with the name FILENAME in the current directory|
   |my_cd DIRNAME| Changes current directory to the directory DIRNAME|
   |run SCRIPT.TXT | Cxecutes the file SCRIPT.TXT|
   |exec PROG1.TXT [PROG2.TXT] [PROG3.TXT] POLICY [#] [MT] | Executes up to 3 programs concurrently according to the scheduling POLICY (optional: # allows for background execution and MT allows for multithreading)|
   
   ## Scheduling
   
   There are 5 scheduling POLICIES implemented:
   * FCFC (First Come First Serve)
   * SJF (Shortest Job First)
   * RR (Round Robin - step: 2)
   * RR30 (Round Robin - step: 30)
   * AGING (Older programs are given priority)
   
   ## Memory Managment
   
   
   A demand paging strategy was implemented to manage memory on the 'paging' branch. Any time the framestore is full and a new page needs to be added, a victim frame is chosen to be evicted. The frame is chosen based on an LRU (least recently used) strategy.

note: 2 macros need to be set to compile the version on the paging branch. It can be done as follows: <br>
```make mysh framesize=X varmemsize=Y``` where X and Y are collectively less than 1000. 








