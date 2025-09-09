#include "Point.h"
#include "raymath.h"
#include <iostream>
#include <cmath>

Point::Point(Point *_connectedPoint, float _connectionDistance, float _connectionDistanceMultiplier, bool _hasLegs, bool _useInputs, float _speed)
    : m_connectedPoint(_connectedPoint),
      m_connectionDistance(_connectionDistance),
      m_connectionDistanceMultiplier(_connectionDistanceMultiplier),
      m_hasLegs(_hasLegs),
      m_useInputs(_useInputs),
      m_speed(_speed)
{
}

Point::~Point()
{
}

void Point::Update()
{
    if (m_connectedPoint != nullptr)
    {
        if (m_debug)
        {
            DrawLineV(m_position, m_connectedPoint->GetPosition(), RED);
            DrawCircleLines(m_connectedPoint->GetPosition().x, m_connectedPoint->GetPosition().y, 10, PINK);
        }

        Vector2 dir = {m_connectedPoint->GetPosition().x - m_position.x, m_connectedPoint->GetPosition().y - m_position.y};
        float length = sqrt(dir.x * dir.x + dir.y * dir.y); // pythagrus
        dir.x /= length;
        dir.y /= length;

        if (m_hasLegs)
        {
            // Perpendicular vector (rotate 90 degrees)
            Vector2 perp = {-dir.y, dir.x};

            // Scale perpendicular offset (distance from the point)
            float offset = 35.0f; // how far "beside" the point the circle should be
            perp.x *= offset;
            perp.y *= offset;

            // Final circle position = current point position + perpendicular offset
            Vector2 circlePos = {
                m_position.x + perp.x,
                m_position.y + perp.y};

            Vector2 circlePosIn = {
                m_position.x - perp.x,
                m_position.y - perp.y};

            if (m_debug)
            {
                DrawCircleLines(circlePos.x, circlePos.y, 10, ORANGE);
                DrawCircleLines(circlePosIn.x, circlePosIn.y, 10, ORANGE);
            }

            if (Vector2Distance(m_feetPos, circlePos) > 20)
            {
                float forwardOffset = 10.0f;
                m_feetPos = Vector2Add(circlePos, Vector2Scale(dir, forwardOffset));
            }

            if (Vector2Distance(m_feetPosIn, circlePosIn) > 20)
            {
                float forwardOffset = 10.0f;
                m_feetPosIn = Vector2Add(circlePosIn, Vector2Scale(dir, forwardOffset));
            }

            if (m_debug)
            {
                // DrawCircleLines(circlePos.x, circlePos.y + 2.5f, 2, PURPLE);
                DrawCircleLinesV(Vector2Add(m_feetPos, Vector2Scale(dir, 2)), 2, PURPLE);
                DrawCircleLinesV(Vector2Add(m_feetPosIn, Vector2Scale(dir, 2)), 2, PURPLE);
            }

            DrawLineV(m_position, m_feetPos, BLUE);
            DrawLineV(m_position, m_feetPosIn, BLUE);
        }

        dir.x *= m_connectionDistance * m_connectionDistanceMultiplier;
        dir.y *= m_connectionDistance * m_connectionDistanceMultiplier;
        m_position.x = m_connectedPoint->GetPosition().x - dir.x;
        m_position.y = m_connectedPoint->GetPosition().y - dir.y;
    }
    if (!m_debug)
    {
        DrawCircle(m_position.x, m_position.y, m_connectionDistance, BLUE);
    }

    if (m_useInputs)
    {
        Vector2 moveDirection = {};

        if (IsKeyDown(KEY_W))
            moveDirection = Vector2Add(moveDirection, {0, -1});
        if (IsKeyDown(KEY_S))
            moveDirection = Vector2Add(moveDirection, {0, 1});
        if (IsKeyDown(KEY_A))
            moveDirection = Vector2Add(moveDirection, {-1, 0});
        if (IsKeyDown(KEY_D))
            moveDirection = Vector2Add(moveDirection, {1, 0});

        moveDirection = Vector2Normalize(moveDirection);

        if (moveDirection.x != 0 || moveDirection.y != 0)
            m_latestMoveDir = moveDirection;

        // Normalize the forward direction
        Vector2 forward = Vector2Normalize(m_latestMoveDir);

        if (!m_debug)
        {
            // Create a perpendicular vector (sideways)
            Vector2 side = {-forward.y, forward.x}; // 90° rotated vector

            // Distances
            float forwardOffset = 10.0f; // how far in front
            float sideOffset = 5.0f;     // how far to the side

            // Combine offsets
            Vector2 offset = {
                forward.x * forwardOffset + side.x * sideOffset,
                forward.y * forwardOffset + side.y * sideOffset};

            // Final circle position
            Vector2 circlePos = {
                m_position.x + offset.x,
                m_position.y + offset.y};

            // Combine offsets
            Vector2 offsetIn = {
                forward.x * forwardOffset - side.x * sideOffset,
                forward.y * forwardOffset - side.y * sideOffset};

            // Final circle position
            Vector2 circlePosIn = {
                m_position.x + offsetIn.x,
                m_position.y + offsetIn.y};

            DrawCircle(circlePos.x, circlePos.y, 5, WHITE);
            DrawCircle(circlePosIn.x, circlePosIn.y, 5, WHITE);

            DrawCircle(circlePos.x, circlePos.y, 2, BLACK);
            DrawCircle(circlePosIn.x, circlePosIn.y, 2, BLACK);
        }

        if (m_debug)
        {
            DrawLineV(m_position, Vector2Add(m_position, Vector2Scale(moveDirection, 50)), GREEN);
        }
        moveDirection = Vector2Scale(moveDirection, m_speed * GetFrameTime());

        m_position = Vector2Add(m_position, moveDirection);
    }
}