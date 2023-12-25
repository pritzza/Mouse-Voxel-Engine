#include "Math.h"

#include <algorithm>
#include <cmath>
#include <iostream>

inline static std::mt19937 mt;

char Math::toChar(Axis axis)
{
    switch (axis)
    {
        case Axis::X:   return 'x';
        case Axis::Y:   return 'y';
        case Axis::Z:   return 'z';
    }

    assert(false);

    return '?';
}

Math::Axis Math::toAxis(Face f)
{
    switch (f)
    {
        case Face::Left:    return Axis::X;
        case Face::Right:   return Axis::X;

        case Face::Top:     return Axis::Y;
        case Face::Bottom:  return Axis::Y;

        case Face::Front:   return Axis::Z;
        case Face::Back:    return Axis::Z;
    }

    assert(false);

    return Axis::X;
}

const std::string Math::toString(Face f)
{
    switch (f)
    {
    case Face::Front:   return "Front";
    case Face::Back:    return "Back";
    case Face::Left:    return "Left";
    case Face::Right:   return "Right";
    case Face::Top:     return "Top";
    case Face::Bottom:  return "Bottom";
    }

    assert(false);

    return "INVALID";
}

Math::Face Math::toFace(Axis planeAxis, float component)
{
    switch (planeAxis)
    {
    case Axis::X:
        if (component > 0)
            return Face::Left;
        if (component < 0)
            return Face::Right;

    case Axis::Y:
        if (component > 0)
            return Face::Bottom;
        if (component < 0)
            return Face::Top;

    case Axis::Z:
        if (component > 0)
            return Face::Front;
        if (component < 0)
            return Face::Back;
    }

    assert(false);

    return Face::INVALID;
}

// origin of our cubes are their front bottom left corner, when cubes faces the camera
// when intersecting say the back of a cube, the z coordinate of the
// actual point of intersection will have a z coordinate 1 greater
// than the coordinate identifier of the actual cube it hits
// this gives that difference between the point of intersection and the actual cube
glm::vec3 Math::getOffset(Face intersectedFace)
{
    glm::vec3 offset{ 0, 0, 0 };

    switch (intersectedFace)
    {
        //case Face::Right:   offset.x = -1;  break;
        //case Face::Top:     offset.y = -1;  break;
        //case Face::Back:    offset.z = -1;  break;
    }

    return offset;
}

glm::vec3 Math::getPoint(const Line& l, float t)
{
    //                  coefficient (m),     intercept (b)
    LinearEquation eqX{ l.end.x - l.start.x, l.start.x };
    LinearEquation eqY{ l.end.y - l.start.y, l.start.y };
    LinearEquation eqZ{ l.end.z - l.start.z, l.start.z };

    return glm::vec3{
        eqX.valueAt(t),
        eqY.valueAt(t),
        eqZ.valueAt(t),
    };
}

std::optional<float> Math::intersect(const Line& l, const Plane& p)
{
    LinearEquation eq;

    switch (p.axis)
    {
        //                                 coefficient (m),     intercept (b)
    case Axis::X: eq = LinearEquation{ l.end.x - l.start.x, l.start.x }; break;
    case Axis::Y: eq = LinearEquation{ l.end.y - l.start.y, l.start.y }; break;
    case Axis::Z: eq = LinearEquation{ l.end.z - l.start.z, l.start.z }; break;
    }

    std::optional<float> t{ std::nullopt };

    if (eq.m != 0)
        t = (p.value - eq.b) / eq.m;

    return t;
}

float Math::getComponent(const glm::vec3& v, Axis axis)
{
    switch (axis)
    {
    case Axis::X:   return v.x;
    case Axis::Y:   return v.y;
    case Axis::Z:   return v.z;
    }

    assert(false);

    return 0;
}

std::optional<Math::FullCubeIntersection> Math::processIntersection(const Plane& plane, const glm::vec3& rayDirection, const Line& ray)
{
    const std::optional<float> t{ intersect(ray, plane) };

    const bool isPlaneParallel{ t.has_value() };

    if (isPlaneParallel)
    {
        const float tVal{ t.value() };

        const bool isHit{ 0 <= tVal && tVal <= 1 };

        if (isHit)
        {
            // component of the ray direction that is the same as the plane's axis
            float component{ getComponent(rayDirection, plane.axis) };

            // the face that got hit
            Face cubeFace{ toFace(plane.axis, component) };

            // intersection point
            const glm::vec3 ip{ getPoint(ray, tVal) };

            const glm::vec3 faceOffset{ getOffset(cubeFace) };

            static constexpr float VERY_SMALL_NUMBER{ 0.000001f };

            // intersected cube coordinates
            // add very small number in attempt to circumvent floating point error
            const glm::vec3 ic{
                (int)(ip.x + faceOffset.x + VERY_SMALL_NUMBER),
                (int)(ip.y + faceOffset.y + VERY_SMALL_NUMBER),
                (int)(ip.z + faceOffset.z + VERY_SMALL_NUMBER),
            };

            return FullCubeIntersection{ ic, ip, cubeFace, tVal };
        }
    }

    return std::nullopt;
}

void Math::interpretIntersection(const std::optional<FullCubeIntersection>& intersection)
{
    const bool intersectionOccured{ intersection.has_value() };

    if (intersectionOccured)
    {
        FullCubeIntersection i{ intersection.value() };

        std::cout << "Intersection on the " << toString(i.intersectedFace)
            << " face of cube ["
            << i.cubeCoords.x << ", "
            << i.cubeCoords.y << ", "
            << i.cubeCoords.z << "] at exactly ("
            << i.intersectionPosition.x << ", "
            << i.intersectionPosition.y << ", "
            << i.intersectionPosition.z << "), when t = "
            << i.t << '\n';
    }
}

std::vector<Math::Plane> Math::generatePlanes(const Line& ray)
{
    // each component of the displacement vector is the number of
    // each plane that the vector passes through
    glm::vec3 displacement{ ray.end - ray.start };

    std::vector<Plane> planes;

    const int xDir{ int(displacement.x / abs(displacement.x)) };
    const int yDir{ int(displacement.y / abs(displacement.y)) };
    const int zDir{ int(displacement.z / abs(displacement.z)) };

    for (int x = 0; x < abs(displacement.x); ++x)
    {
        // x + 1 because we create the plane infront and in the direction of the vector
        const int planeValue{ (int)(((x + 1) * xDir) + ray.start.x) };
        planes.emplace_back(Plane{ Axis::X, planeValue });
    }
    for (int y = 0; y < abs(displacement.y); ++y)
    {
        const int planeValue{ (int)(((y + 1) * yDir) + ray.start.y) };
        planes.emplace_back(Plane{ Axis::Y, planeValue });
    }
    for (int z = 0; z < abs(displacement.z); ++z)
    {
        const int planeValue{ (int)(((z + 1) * zDir) + ray.start.z) };
        planes.emplace_back(Plane{ Axis::Z, planeValue });
    }

    return planes;
}

std::vector<Math::CubeIntersection> Math::getRayCubeIntersections(
    const glm::vec3& rayDirection, 
    const glm::vec3& rayStartPos, 
    float rayCastDistance
)
{
    glm::vec3 elongatedRay{ glm::normalize(rayDirection) * rayCastDistance };

    Line ray{
        rayStartPos,
        elongatedRay + rayStartPos,
    };

    const std::vector<Plane> planes{ generatePlanes(ray) };

    std::vector<FullCubeIntersection> intersections;

    // find intersection between view ray and planes
    for (const Plane& plane : planes)
    {
        const std::optional<FullCubeIntersection> intersection{ processIntersection(
            plane, rayDirection, ray
        ) };

        if (intersection.has_value())
            intersections.emplace_back(intersection.value());
    }

    const auto sortByIncreasingTValue =
        [](const FullCubeIntersection& a, const FullCubeIntersection& b)
    {
        return a.t < b.t;
    };

    std::sort(intersections.begin(), intersections.end(), sortByIncreasingTValue);

    std::vector<CubeIntersection> conciseIntersections{ 
         intersections.size(), CubeIntersection{}
    };

    for (int i = 0; i < intersections.size(); ++i)
    {
        conciseIntersections[i].cubeCoords = intersections[i].cubeCoords;
        conciseIntersections[i].intersectedFace = intersections[i].intersectedFace;
    }

    return conciseIntersections;
}

void Math::test()
{
    // initialize conditions

    const float n{ 0.435534564 };

    const glm::vec3 viewDir{ glm::normalize(glm::vec3{
        sin(n), 0.f, cos(n)
    }) };

    glm::vec3 viewPos{ 0.5f, 0.5f, 0.5f };

    static constexpr float castDistance{ 5.f };

    // create view ray
    glm::vec3 viewTarget{ viewDir * castDistance };

    std::cout << "view: <" << viewTarget.x << ", " << viewTarget.y << ", " << viewTarget.z << ">\n";

    Line viewRay{
        viewPos,
        viewTarget + viewPos,
    };

    // create NUM_PLANES empty planes
    const std::vector<Plane> planes{ generatePlanes(viewRay) };

    std::cout << "Num planes: " << planes.size() << '\n';;

    // find intersection between view ray and planes
    for (const Plane& plane : planes)
    {
        const std::optional<FullCubeIntersection> intersection{ processIntersection(
            plane, viewDir, viewRay
        ) };

        interpretIntersection(intersection);
    }
}

int Math::toIndex(const glm::ivec3& coord, const glm::ivec3& dim)
{
    return coord.x + (coord.y * dim.x) + (coord.z * dim.y * dim.x);
}

glm::ivec3 Math::toCoord(int index, const glm::ivec3& dim)
{
    const int x{ index % dim.x };
    const int y{ (index / dim.x) % dim.y };
    const int z{ index / (dim.x * dim.y) };
    return glm::ivec3{ x, y, z };
}

bool Math::isInside(int val, int lower, int upper)
{
    return lower <= val && val < upper;
}

bool Math::isInside(const glm::ivec3& coord, const glm::ivec3& bounds)
{
    return isInside(coord, glm::ivec3{ 0 }, bounds);
}

bool Math::isInside(
    const glm::ivec3& coord, 
    const glm::ivec3& lower, 
    const glm::ivec3& upper
)
{
    return isInside(coord.x, lower.x, upper.x) &&
           isInside(coord.y, lower.y, upper.y) &&
           isInside(coord.z, lower.z, upper.z);
}

std::array<glm::ivec3, 27> Math::getCubeSurroundingCoords()
{
    std::array<glm::ivec3, 27> surroundingOffsets;

    const glm::ivec3 surroundingDim{ glm::ivec3{ 3, 3, 3 } };

    for (int i = 0; i < 27; ++i)
        surroundingOffsets[i] = Math::toCoord(i, surroundingDim) - glm::ivec3{ 1 };

    return surroundingOffsets;
}

void Math::seed_rng(int seed)
{
    mt.seed(seed);
}

// return uniformly random int between [min, max)
int Math::rng(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max - 1);
    return dist(mt);
}