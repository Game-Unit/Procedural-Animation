#include "raylib.h"
#include "Point.h"
#include <vector>
#include <memory>

int main()
{
    InitWindow(800, 800, "Procedural Animations");

    std::vector<std::unique_ptr<Point>> points;

    int distanceBetweenBoadyParts = 3;

    int partSpawnCount = distanceBetweenBoadyParts;

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawText("Press Space To Spawn Boady Parts", 150, 50, 25, BLACK);

        if (IsKeyPressed(KEY_SPACE))
        {
            if (points.empty())
                points.push_back(std::make_unique<Point>(nullptr, 15, 2, false, true, 150));
            else
            {
                if (partSpawnCount == distanceBetweenBoadyParts)
                {
                    partSpawnCount = 0;
                    points.push_back(std::make_unique<Point>(points.back().get(), 10, 1, true));
                }
                points.push_back(std::make_unique<Point>(points.back().get(), 10, 1, false));
                partSpawnCount++;
            }
        }

        bool first = true;
        for (auto &p : points)
        {
            if (first)
            {
                //p->SetPosition(GetMousePosition());
                first = false;
            }
            p->Update();
        }

        DrawFPS(5, 5);
        EndDrawing();
    }
}
