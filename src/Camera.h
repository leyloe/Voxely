#ifndef VOXELY_CAMERA_H
#define VOXELY_CAMERA_H

#include <donut/app/Camera.h>
#include <donut/core/math/math.h>

using namespace donut;

class Camera : public app::FirstPersonCamera {
public:
    void SetPerspective(float verticalFovDegrees, float nearPlane, float farPlane);

    [[nodiscard]] dm::float4x4 GetViewProjMatrix(float aspect) const;

private:
    float m_FovDegrees = 60.f;
    float m_Near = 0.1f;
    float m_Far  = 1000.f;
};

#endif //VOXELY_CAMERA_H
