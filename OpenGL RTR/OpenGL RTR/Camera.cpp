//
//  Camera.cpp
//  Lab5
//
//  Created by CGIS on 28/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#include "Camera.hpp"

namespace Graphics {
    
    Camera::Camera() { }

    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, float cameraSpeed)
    {
        this->pitch = 0.0f;
        this->yaw = -90.0f;
        this->cameraSpeed = cameraSpeed;
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    void Camera::loadCamera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, float cameraSpeed)
    {
        this->pitch = 0.0f;
        this->yaw = -90.0f;
        this->cameraSpeed = cameraSpeed;
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    
    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(cameraPosition, cameraPosition + cameraDirection , glm::vec3(0.0f, 1.0f, 0.0f));
    }

	glm::vec3 Camera::getCameraTarget()
	{
		return cameraTarget;
	}
    
    void Camera::move(MOVE_DIRECTION direction, float deltaTime)
    {
        switch (direction) {
            case MOVE_FORWARD:
                cameraPosition += cameraDirection * (cameraSpeed * deltaTime);
                break;
                
            case MOVE_BACKWARD:
                cameraPosition -= cameraDirection * (cameraSpeed * deltaTime);
                break;
                
            case MOVE_RIGHT:
                cameraPosition += cameraRightDirection * (cameraSpeed * deltaTime);
                break;
                
            case MOVE_LEFT:
                cameraPosition -= cameraRightDirection * (cameraSpeed * deltaTime);
                break;
        }
    }
    
    void Camera::rotate(float xoffset, float yoffset)
    {
        this->pitch += yoffset;
        this->yaw += xoffset;

        this->pitch = (this->pitch > 89.0f) ? 89.0f : ((this->pitch < -89.0f) ? -89.0f : this->pitch);

		glm::vec3 front;

		front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
		front.y = sin(glm::radians(this->pitch));
		front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));

		this->cameraDirection = glm::normalize(front);
		this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    
}
