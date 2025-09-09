#pragma once
#include "raylib.h"

class Point
{
private:
    Vector2 m_position = {400, 400};
    Point* m_connectedPoint = {};
    float m_connectionDistance = 0;
    float m_connectionDistanceMultiplier = 0;
    
    // movement
    bool m_useInputs = false;
    float m_speed = 0;
    
    // legs
    bool m_hasLegs = true;
    Vector2 m_feetPos = {};
    Vector2 m_feetPosIn = {0, -1};

    bool m_debug = false;

    Vector2 m_latestMoveDir = {};

public:
    Point(Point* _connectedPoint, float _connectionDistance, float _ConnectionDistanceMultiplayer, bool _hasLegs, bool _useImputs = false, float _speed = 1);
    ~Point();

    Vector2 GetPosition() { return m_position; }

    void Update();

    void SetPosition(Vector2 _position) { m_position = _position; }
};