#pragma once
#include "engineMath.h"

class CollisionComponent;

class Physics
{
public:
    class AABB
    {
    public:
        AABB() { }
        AABB(const Vector3& min, const Vector3& max)
            : mMin(min)
            , mMax(max)
        {}

        Vector3 mMin, mMax;
    };

    class LineSegment
    {
    public:
        LineSegment() {}
        LineSegment(const Vector3& from, const Vector3& to)
            : mFrom(from)
            , mTo(to)
        {}

        Vector3 mFrom, mTo;
    };

    static bool Intersect(const AABB& a, const AABB& b, AABB* pOverlap = nullptr);
    static bool Intersect(const LineSegment& segment, const AABB& box, Vector3* pHitPoint = nullptr);
    static bool UnitTest();

    void AddObj(CollisionComponent* pObj);
    void RemoveObj(CollisionComponent* pObj);
    bool RayCast(const LineSegment& segment, Vector3* pHitPoint);

private:
    std::vector<CollisionComponent*> mObj;
};