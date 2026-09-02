// Copyright (c) 2026 BYK. All Rights Reserved.
//
// This file is part of SCGL.
//
// SCGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, under version 2 of the License.
//
// SCGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SCGL. If not, see <https://www.gnu.org/licenses/>.
//
// Maintainer: BYK <bykdev@proton.me>

#pragma once
#include <cglm/cglm.h>
#include <cglm/call.h>
#include <cglm/struct.h>
#include "Abstractions.h"

const float MOVEMENT_SPEED = 0.1f;

vec3 viewDirection = {0.0f, 0.0f, -1.0f};
vec3 UP = {0.0f, 1.0f, 0.0f};

vec2 oldMousePosition = {0.0f, 0.0f};
vec3 strafeDirection = {0.0f, 0.0f, 0.0f};
vec3 camPosition = {0.0f, 0.0f, 0.0f};
vec3 position = {0.0f, 0.0f, 0.0f,};

void MouseUpdate(vec2 newMousePosition)
{
    vec2 mouseDelta;
    glm_vec2_sub(newMousePosition, oldMousePosition, mouseDelta);

    if(glm_vec2_norm(mouseDelta) > 50.0f)
    {
	glm_vec2_copy(newMousePosition, oldMousePosition);
	return;
    }
    const float ROTATIONAL_SPEED = 0.25f;
    glm_vec3_cross(viewDirection, UP, strafeDirection);
    mat4 rot1 = GLM_MAT4_IDENTITY_INIT;
    mat4 rot2 = GLM_MAT4_IDENTITY_INIT;
    mat3 collapsedRotator;
    Mat4Rotate(rot1, strafeDirection, camPosition, (-mouseDelta[1]) * ROTATIONAL_SPEED);
    Mat4Rotate(rot2, UP, camPosition, (-mouseDelta[0]) * ROTATIONAL_SPEED);
    mat4* rotator = Mat4Mul(rot1, rot2);
    glm_mat4_pick3(*rotator, collapsedRotator);
    glm_mat3_mulv(collapsedRotator, viewDirection, viewDirection);
    glm_vec2_copy(newMousePosition, oldMousePosition);
}

void GetWorldToViewMatrix(mat4 worldToViewMatrix)
{
    vec3 posViewDirVec;
    glm_vec3_add(position, viewDirection, posViewDirVec);
    glm_lookat(position, posViewDirVec, UP, worldToViewMatrix);
}

void MoveForeward()
{
    vec3 step;
    glm_vec3_scale(viewDirection, MOVEMENT_SPEED, step);
    glm_vec3_add(position, step, position);
}

void MoveBackward()
{
    vec3 step;
    glm_vec3_scale(viewDirection, (-MOVEMENT_SPEED), step);
    glm_vec3_add(position, step, position);
}

void StrafeLeft()
{
    vec3 step;
    glm_vec3_scale(strafeDirection, (-MOVEMENT_SPEED), step);
    glm_vec3_add(position, step, position);
}

void StrafeRight()
{
    vec3 step;
    glm_vec3_scale(strafeDirection, MOVEMENT_SPEED, step);
    glm_vec3_add(position, step, position);
}

void MoveUp()
{
    vec3 step;
    glm_vec3_scale(UP, MOVEMENT_SPEED, step);
    glm_vec3_add(position, step, position);
}

void MoveDown()
{
    vec3 step;
    glm_vec3_scale(UP, (-MOVEMENT_SPEED), step);
    glm_vec3_add(position, step, position);
}
