#include "Camera.h"

void Camera::SetPerspective(const float verticalFovDegrees, const float nearPlane, const float farPlane) {
    m_FovDegrees = verticalFovDegrees;
    m_Near = nearPlane;
    m_Far  = farPlane;
}

dm::float4x4 Camera::GetViewProjMatrix(const float aspect) const {
    const dm::float4x4 proj = dm::perspProjD3DStyle(dm::radians(m_FovDegrees), aspect, m_Near, m_Far);
    return dm::affineToHomogeneous(GetWorldToViewMatrix()) * proj;
}
