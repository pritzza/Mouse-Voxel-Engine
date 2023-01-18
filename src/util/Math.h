#pragma once

#include <optional>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Math
{
	static constexpr glm::vec3 I_HAT{ 1.f, 0.f, 0.f };
	static constexpr glm::vec3 J_HAT{ 0.f, 1.f, 0.f };
	static constexpr glm::vec3 K_HAT{ 0.f, 0.f, 1.f };

	static constexpr glm::mat4 IDENTITY_MATRIX{ glm::mat4(1.0f) };

    enum class Axis
    {
        X,
        Y,
        Z
    };

    enum class Face
    {
        Front,
        Back,
        Left,
        Right,
        Top,
        Bottom,
        INVALID
    };

    struct Line
    {
        glm::vec3 start;
        glm::vec3 end;
    };

    struct Plane
    {
        Axis axis;
        int value{};
    };

    struct FullCubeIntersection
    {
        glm::vec3 cubeCoords;
        glm::vec3 intersectionPosition;
        Face intersectedFace;
        float t;
    };

    struct CubeIntersection
    {
        Face intersectedFace;
        glm::vec3 cubeCoords;
    };

    // m*t + b
    struct LinearEquation
    {
        float m{};  // coefficient/slope

        float b{};  // constant term/intercept

        float valueAt(float t) const
        {
            return m * t + b;
        }
    };

    // enum to other type stuff
    char toChar(Axis axis);
    Axis toAxis(Face f);
    const std::string toString(Face f);
    Face toFace(Axis planeAxis, float component);

    glm::vec3 getOffset(Face intersectedFace);

    glm::vec3 getPoint(const Line& l, float t);

    std::optional<float> intersect(const Line& l, const Plane& p);

    float getComponent(const glm::vec3& v, Axis axis);

    std::optional<FullCubeIntersection> processIntersection(
        const Plane& plane,
        const glm::vec3& rayDirection,
        const Line& ray
    );

    void interpretIntersection(
        const std::optional<FullCubeIntersection>& intersection
    );

    std::vector<Plane> generatePlanes(const Line& ray);

    std::vector<CubeIntersection> getRayCubeIntersections(
        const glm::vec3& rayDirection,
        const glm::vec3& rayStartPos,
        float rayCastDistance
    );

    void test();

};