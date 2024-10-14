# Boruvka's Algorithm in a multicore CPU

Boruvka's algorithm is used to find the Minimum Spanning Tree (MST) of a graph. I have implemented a parallel version of the same in
the C programming language and have used Open MP as a tool to unlock parallelism. Implementation details can be found below.

## How to run:

For now the code can simply take as an argument the graph name and print out all the vertices and edges with their corresponding edge weights.

```bash
git clone https://github.com/sarath-a-12/Boruvka-Parallel-Implementation.git
```

You can see the graphs stored in the /graphs folder.

The list of graphs available in the folder are as follows:

- **ljournal**
- **u10m**
- **USA**
- **wikipedia**
- **x2**

Now you can simply view the graph vertices and edges using the following command

```bash
./run <graphname>
```

> **Note** : A graph name is to be provided as an argument to the run command, else it will throw an error and exit.

After the command, you can simply navigate the graph vertices and edge weights using the **j** (_for moving down_) and **k** (_for moving up_).
Press **q** to quit.
