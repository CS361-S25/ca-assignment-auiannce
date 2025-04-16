/* 
I used GitHub Copilot for assistance with some of the syntax, and Jared helped me with the logic and implementation of the UpdateCells function.
*/


#include "emp/web/Animate.hpp"   // For animation loop
#include "emp/web/web.hpp"       // For web interface
#include "emp/math/math.hpp"     // For mathematical utilities (like Mod)
#include "emp/math/Random.hpp"   // For random number generation

// Create a web document targeting a specific element with ID "target"
emp::web::Document document{"target"};

// Class that handles the Cellular Automaton animation
class ContinuousCAAnimator : public emp::web::Animate {
    // Grid dimensions and cell size in pixels
    const int grid_width_cells = 120;
    const int grid_height_cells = 100;
    const double cell_size = 10.0;

    // Canvas dimensions in pixels
    const double canvas_width = grid_width_cells * cell_size;
    const double canvas_height = grid_height_cells * cell_size;

    // Web canvas for drawing the grid
    emp::web::Canvas canvas{canvas_width, canvas_height, "canvas"};

    // Random number generator with fixed seed
    emp::Random random_gen = *new emp::Random(1);

    // 2D grids: one for current state, one for next state
    std::vector<std::vector<double>> cells;
    std::vector<std::vector<double>> newCells;

public:
    //sets up canvas, buttons, and initial state
    ContinuousCAAnimator() {
        document << canvas;
        document << GetToggleButton("Start"); // Add start/stop button
        document << GetStepButton("Step");    // Add step-by-step button

        // Initialize grid with zeros (dead cells)
        cells.resize(grid_width_cells, std::vector<double>(grid_height_cells, 0.0));
        newCells = cells;

        // Add some random gliders as initial state
        AddRandomGliders(80);
    }

    // Place random "glider" patterns on the grid
    void AddRandomGliders(int glider_count) {
        std::vector<std::pair<int, int>> glider_pattern = {
            {0, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}
        };

        for (int i = 0; i < glider_count; i++) {
            int start_x = random_gen.GetInt(0, grid_width_cells - 3);
            int start_y = random_gen.GetInt(0, grid_height_cells - 3);
            for (auto [dx, dy] : glider_pattern) {
                int cell_x = start_x + dx;
                int cell_y = start_y + dy;
                cells[cell_x][cell_y] = 1.0;
            }
        }
    }

    // Calculate average value of 8 surrounding neighbors
    float GetNearNeighborAverage(int x, int y) {
        float total = 0.0;
        int count = 0;

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; // Skip center
                int neighbor_x = emp::Mod(x + dx, grid_width_cells);
                int neighbor_y = emp::Mod(y + dy, grid_height_cells);
                total += cells[neighbor_x][neighbor_y];
                count++;
            }
        }

        return total / count;
    }

    // Calculate average value of 16 distant neighbors (2 cells away)
    float GetDistantNeighborAverage(int x, int y) {
        float total = 0.0;
        int count = 0;

        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                if (std::abs(dx) <= 1 && std::abs(dy) <= 1) continue; // Skip near and center
                int neighbor_x = emp::Mod(x + dx, grid_width_cells);
                int neighbor_y = emp::Mod(y + dy, grid_height_cells);
                total += cells[neighbor_x][neighbor_y];
                count++;
            }
        }

        return total / count;
    }

    // Update grid using average of near and distant neighbor values
    void UpdateCells() {
        for (int x = 0; x < grid_width_cells; x++) {
            for (int y = 0; y < grid_height_cells; y++) {
                float near_avg = GetNearNeighborAverage(x, y);
                float distant_avg = GetDistantNeighborAverage(x, y);
                float combined_avg = (near_avg + distant_avg) / 2.0;

                if (cells[x][y] >= 0.5) {
                    // Cell is alive
                    if (combined_avg <= 0.8) {
                        newCells[x][y] = (1 + combined_avg) / 2.0; //survival
                    } else {
                        newCells[x][y] = 0.0; // Overpopulation death
                    }
                } else {
                    // Cell is dead
                    if (combined_avg >= 0.275) {
                        newCells[x][y] = (1 + combined_avg) / 2.0; // birth
                    } else {
                        newCells[x][y] = 0.0; // Stay dead
                    }
                }
            }
        }
    }

    // Draw all cells to the canvas with color intensity based on value
    void DrawGrid() {
        canvas.Clear();

        for (int x = 0; x < grid_width_cells; x++) {
            for (int y = 0; y < grid_height_cells; y++) {
                double cell_value = cells[x][y];
                int blue = static_cast<int>(cell_value * 255);
                int red = static_cast<int>(cell_value * 100);
                std::string color = "rgb(" + std::to_string(red) + ", 0, " + std::to_string(blue) + ")";
                canvas.Rect(x * cell_size, y * cell_size,
                            cell_size, cell_size, color, color);
            }
        }
    }

    // Called once per frame in animation loop
    void DoFrame(){
        DrawGrid();      // Visualize current state
        UpdateCells();   // Compute next state
        cells = newCells; // Apply updates
    }
};

// Create instance of the animator
ContinuousCAAnimator animator;

// Run one frame to show initial grid
int main() {
    animator.Step(); 
}
