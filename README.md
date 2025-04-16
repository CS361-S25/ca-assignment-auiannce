[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/StQAS8iq)

# Continuous-State Cellular Automaton

## Description

This project shows a continuous-state cellular automaton using the Empirical framework. Instead of each cell being just alive or dead, cells can have values between 0 and 1. These values change over time based on the average values of neighboring cells.

The grid starts with several gliders (from Conway's Game of Life), but since the cell values are continuous and the rules are smoother, the gliders don't behave exactly the same. Instead, the automaton creates slow, changing patterns that move and fade over time.

## Pattern Details

- **Cell values:** Between 0 (dead) and 1 (fully alive)
- **Neighborhoods:**
  - Near neighbors: 8 surrounding cells
  - Distant neighbors: Cells within 2-cell distance, but not the near ones
- **Update rule:**
  - Averages from both neighborhoods are combined.
  - Alive cells may stay alive or fade out depending on the average.
  - Dead cells may become alive if the average is high enough.
  - New values are calculated using:
    ```
    new_value = (1 + neighborhood_avg) / 2.0
    ```

## Grid Behavior

- The edges of the grid wrap around (like Pac-Man), so it acts like a torus.
- The grid is updated smoothly every frame.
- The colors on the canvas change based on the cell value, with more blue and red as values get closer to 1.0.

## Sources Used

- Lenia (smooth cellular automata): https://chakazul.github.io/lenia.html
- Growing Neural Cellular Automata (Distill): https://distill.pub/2020/growing-ca/
- Conway's Game of Life (for glider pattern): https://conwaylife.com/

## How to Run

Use the provided `compile-run.sh` script. You need to have Empirical installed and set up. This will compile the program to run in a browser.
