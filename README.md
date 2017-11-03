# Iterated Prisoner’s Dilemma

A Coevolutionary Genetic Programming Approach.

### Makes Use of

The following third party libraries and specifications were used.

* C++11
* Kasper Peters' [tree.hh library](http://tree.phi-sci.com/index.html)

### Running

Use the included ```run.sh``` to compile and run the program. You will need to supply the configuration file as an argument.

```bash
./run.sh cfg/default.cfg
```

### Output

The program will output the following. The filepath of each in configurable in the passed .cfg file.

* Log File - Prints a header for each run and a series of tab-delimited rows detailing run information.
* Standard Out - Mirrors the logfile.
* Solution File - Prints the best agent's tree in pre-order format.

### Author

Stuart Miller - sm67c@mst.edu - [Github](https://github.com/stewythe1st)