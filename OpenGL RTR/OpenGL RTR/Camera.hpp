//
//  Camera.hpp
//
//  Created by CGIS on 28/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace Graphics {

    enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT };

    class Camera
    {
    public:
        Camera();
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, float cameraSpeed);
        void loadCamera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, float cameraSpeed);

        glm::mat4 getViewMatrix();
		glm::vec3 getCameraTarget();
        glm::vec3 getCameraPosition();
		void move(MOVE_DIRECTION direction, float deltaTime);
        void rotate(float pitch, float yaw);		
        
    private:
        
        float cameraSpeed;
        float pitch;
        float yaw;
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraDirection;
        glm::vec3 cameraRightDirection;
    };
    
}

#endif /* Camera_hpp */
