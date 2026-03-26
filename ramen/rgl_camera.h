#ifndef RGL_CAMERA_H
#define RGL_CAMERA_H

#include "rgl_math.h"

class Camera
{
  public:
    Camera(const Vec3f& position)
    {
        m_Position     = position;
        m_Forward      = RAMEN_CAMERA_FORWARD;
        m_Up           = RAMEN_WORLD_UP;
        m_Right        = RAMEN_WORLD_RIGHT;
        m_qOrientation = AngleAxis(m_Up, 0.0f);
    }

    const Vec3f& GetPosition() const
    {
        return m_Position;
    }

    const Vec3f& GetForward() const
    {
        return m_Forward;
    }

    const Vec3f& GetUp() const
    {
        return m_Up;
    }

    const Vec3f GetRight() const
    {
        return Normalize(Cross(m_Forward, m_Up));
    }

    /* Orient camera so that it looks at target. */
    // void Orient(const Vec3f target)
    // {
    //     Vec3f newForward = Normalize(target - m_Position);
    //     Vec3f newSide    = Normalize(Cross(newForward, Vec3f{ 0.0f, 1.0f, 0.0f }));
    //     Vec3f newUp      = Normalize(Cross(newSide, newForward));
    //     m_qOrientation   = AngleAxis(newForward, 0.0f);
    // }

    void RotateAroundWorldUp(const float& angle)
    {
        Quat q         = AngleAxis(RAMEN_WORLD_UP, angle);
        m_qOrientation = q * m_qOrientation;
        Mat4f R        = ToMat4f(m_qOrientation);
        m_Forward      = Vec3f{ R * Vec4f{ RAMEN_CAMERA_FORWARD, 0.0f } };
        // m_Forward      = Rotate(RAMEN_WORLD_FORWARD, m_qOrientation); // TODO: Implement.
        m_Up    = Vec3f{ R * Vec4f{ RAMEN_WORLD_UP, 0.0f } };
        m_Right = Vec3f{ R * Vec4f{ RAMEN_WORLD_RIGHT, 0.0f } };
    }

    void RotateAroundUp(const float& angle)
    {
        Quat q         = AngleAxis(m_Up, angle);
        m_qOrientation = q * m_qOrientation;
        Mat4f R        = ToMat4f(m_qOrientation);
        m_Forward      = Vec3f{ R * Vec4f{ RAMEN_CAMERA_FORWARD, 0.0f } };
        m_Right        = Vec3f{ R * Vec4f{ RAMEN_WORLD_RIGHT, 0.0f } };
    }

    void RotateAroundSide(const float& angle)
    {
        Quat q         = AngleAxis(GetRight(), angle);
        m_qOrientation = q * m_qOrientation;
        Mat4f R        = ToMat4f(m_qOrientation);
        m_Forward      = Vec3f{ R * Vec4f{ RAMEN_CAMERA_FORWARD, 0.0f } };
        m_Up           = Vec3f{ R * Vec4f{ RAMEN_WORLD_UP, 0.0f } };
    }

    void RotateAroundSide(const Quat& q)
    {
        m_qOrientation = q * m_qOrientation;
        Mat4f R        = ToMat4f(m_qOrientation);
        m_Forward      = Vec3f{ R * Vec4f{ RAMEN_CAMERA_FORWARD, 0.0f } };
        m_Up           = Vec3f{ R * Vec4f{ RAMEN_WORLD_UP, 0.0f } };
    }

    void RotateAroundForward(const float& angle)
    {
        Quat q         = AngleAxis(m_Forward, angle);
        m_qOrientation = q * m_qOrientation;
        Mat4f R        = ToMat4f(m_qOrientation);
        m_Up           = Vec3f{ R * Vec4f{ RAMEN_WORLD_UP, 0.0f } };
        m_Right        = Vec3f{ R * Vec4f{ RAMEN_WORLD_RIGHT, 0.0f } };
    }

    void Roll(const float& angle)
    {
        RotateAroundForward(angle);
    }

    void Pitch(const float& angle)
    {
        RotateAroundSide(angle);
    }

    void Yaw(const float& angle)
    {
        RotateAroundUp(angle);
    }

  private:
    Vec3f m_Position;
    Vec3f m_Forward;
    Vec3f m_Up;
    Vec3f m_Right;
    Quat  m_qOrientation;
};

#endif
