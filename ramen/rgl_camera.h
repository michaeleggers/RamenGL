#ifndef RGL_CAMERA_H
#define RGL_CAMERA_H

#include "rgl_math.h"

class Camera
{
  public:
    Camera(const Vec3f& position, const Vec3f& target)
    {
        Vec3f forward  = Normalize(target - position);
        m_Position     = position;
        m_Forward      = forward;
        Vec3f up       = Vec3f{ 0.0f, 1.0f, 0.0f };
        Vec3f right    = Normalize(Cross(m_Forward, up));
        m_Up           = Normalize(Cross(right, m_Forward));
        m_qOrientation = AngleAxis(m_Forward, 0.0f);
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
    void Orient(const Vec3f target)
    {
        Vec3f newForward = Normalize(target - m_Position);
        Vec3f newSide    = Normalize(Cross(newForward, Vec3f{ 0.0f, 1.0f, 0.0f }));
        Vec3f newUp      = Normalize(Cross(newSide, newForward));
        m_qOrientation   = AngleAxis(newForward, 0.0f);
    }

    void RotateAroundUp(const float& angle)
    {
        Quat qRot      = AngleAxis(Normalize(m_Up), angle);
        m_qOrientation = qRot * m_qOrientation;
        Mat4f rotMat   = AsMat4f(qRot);
        m_Forward
            = Vec3f{ rotMat
                     * Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f } }; // TODO: Create 3x3 rotation matrix to avoid conversions.
        m_Up = Vec3f{ rotMat * Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f } };
    }

  private:
    Vec3f m_Position;
    Vec3f m_Forward;
    Vec3f m_Up;
    Quat  m_qOrientation;
};

#endif
