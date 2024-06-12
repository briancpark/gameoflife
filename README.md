# [Game of Life](https://cs61c.org/fa20/projects/proj1/)

Brian Park

John Conway recently [passed away](https://www.nytimes.com/2020/04/15/technology/john-horton-conway-dead-coronavirus.html) from the coronavirus, and this project exercises one of his greatest contributions, [The Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) which we will implement in C!

![John Conway](https://upload.wikimedia.org/wikipedia/commons/thumb/0/04/John_H_Conway_2005_%28cropped%29.jpg/250px-John_H_Conway_2005_%28cropped%29.jpg)

*John Conway (2005)*

![glider guns](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e5/Gospers_glider_gun.gif/220px-Gospers_glider_gun.gif)

*Glider Guns in Conway's Game of Life*

## File I/O

For this project, we had to handle with `.ppm` files. To make sure the file is correctly parsed and information is correctly formatted, read, and written into `.ppm` files. We also format it into a easily workable data structure in C, where we can do bitwise operations and such with the individual pixel values! The structures used are `Color` and `Image` type where we store each color's (R, G, B) intensity value as a `uint8_t` type. Then we store the colors into a 2d row-major array inside of the `Image` struct. This is all handled in the helper code `imageloader.c`.

### Usage

We can test out the correctness of the file parsing through these commands.

This will run the imageloader and check that the program correctly parses and executes:

```sh
make imageloader
```

This will run a Valgrind memory leak test, to make sure no memory is leaked during the program execution:

```sh
make imagememcheck
```

## Steganography

Compiling and executing the code to crack the secret message can be done so by running:

```sh
make steganography
```

This will run a Valgrind memory leak test, to make sure no memory is leaked during the program execution:

```sh
make steganographymemcheck
```

## The Game of Life

Now to the fun part of this project. We will now implement John Conway's Game of Life! The `steganography.c` was just a warmup of how to manipute the pixels. Now we extend this idea of bit manipulation further with black and white images, as well as colored images! The code is in `gameoflife.c`. 

### The Rules

In order to understand what is going on in the code, we must first understand the rules of the Game of Life.

1. Any live cell with two or three live neighbours (in the surrounding 8 cells) survives to the next generation.
2. Any dead cell with three live neighbours becomes a live cell in the next generation.
3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.

Now let's see what this means for the rule `0x1808`. For easier readability, this converts in binary to `0b00 0001 1000 0000 1000`.

Thus, the alive and dead bits are as follows:
|     alive (1)     |      dead (0)     |
|:-----------------:|:-----------------:|
| 8	7 6 5 4 3 2 1 0 | 8	7 6	5 4	3 2	1 0 |
| 0	0 0	0 0	1 1	0 0 | 0 0 0	0 0	1 0	0 0 |

There are many other rules that follow other than the hexadecimal rule `0x1808`:
| Hex rule | Name | Description |
|:--------:|:----:|:-----------:|
| 0x1808 | Life | Highly complex behavior. |
| 0x154AA | Replicator | Edward Fredkin's replicating automaton: every pattern is eventually replaced by multiple copies of itself. |
| 0x4 | Seeds | All patterns are phoenixes, meaning that every live cell immediately dies, and many patterns lead to explosive chaotic growth. However, some engineered patterns with complex behavior are known. |
| 0x2024 | Unnamed | This rule supports a small self-replicating pattern which, when combined with a small glider pattern, causes the glider to bounce back and forth in a pseudorandom walk. |
| 0x3FE08 | Life without Death | Also known as Inkspot or Flakes. Cells that become alive never die. It combines chaotic growth with more structured ladder-like patterns that can be used to simulate arbitrary Boolean circuits. |
| 0x3018 | 34 Life | Was initially thought to be a stable alternative to Life, until computer simulation found that larger patterns tend to explode. Has many small oscillators and spaceships. |
| 0x3C1E8 | Diamoeba | Forms large diamonds with chaotically fluctuating boundaries. |
| 0x4C48 | 2x2 | If a pattern is composed of 2x2 blocks, it will continue to evolve in the same form; grouping these blocks into larger powers of two leads to the same behavior, but slower. Has complex oscillators of high periods as well as a small glider. |
| 0x1848 | HighLife | Similar to Life but with a small self-replicating pattern.
| 0x3B1C8 | Day & Night | Symmetric under on-off reversal. Has engineered patterns with highly complex behavior. |
| 0x6948 | Morley | Named after Stephen Morley; also called Move. Supports very high-period and slow spaceships. |
| 0x3D1D0 | Anneal | Also called the twisted majority rule. Symmetric under on-off reversal. Approximates the curve-shortening flow on the boundaries between live and dead cells. |

This will compile the `./gameoflife` executable:

```sh
make gameoflife
```

This will compile and run Valgrind to ensure there are no memory leaks in the code:

```sh
make gameoflifememcheck
```

This will run the Game of Life on a `.ppm` image with a rule set in hexadecimal:

```sh
./gameOfLife [file] [rule]
```

As a reference solution, the reference solution was availaible on the intructional servers as an executable, so I was able to `diff` check on all possible dimensions and sizes for correctness with this command:

```sh
oracleOfLife [filename] [rule]
```

To automate this process, I created a `bash` script. It is named `test.sh`, because programming in C through SSH made me feel like a hacker. Of course, this script doesn't work anymore now that the semester is over.

```sh
./test.sh
```

Finally, we can make the animation by compiling the frames together into a `.gif`! This is conveninetly done for us in a shell script file by executing:

```sh
./frames.csh GliderGuns 0x1808 100
```

This will run and produce indivual frames of the Game of Life on an image. For example, we create `100` frames in this example, with the rule set to `0x1808`! We store this into the directory `GliderGuns/`.
