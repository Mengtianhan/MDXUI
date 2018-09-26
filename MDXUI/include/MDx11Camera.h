


///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution
// Copyright (C) 2017-2018 by Mengjin (sh06155@hotmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
///////////////////////////////////////////////////////////////////////////////


#pragma once
#include "DxConfig.h"

class DXUI_API MDx11Camera
{
public:
	MDx11Camera();
	virtual ~MDx11Camera();

	void dx_SetPosition(XMFLOAT3 position);
	void dx_SetUpPosition(XMFLOAT3 position);
	void dx_SetLookAtPos(XMFLOAT3 lookat);
	XMFLOAT3 dx_GetPosition() const;
	XMFLOAT3 dx_GetUpDirection() const;
	XMFLOAT3 dx_GetLookAtDirection() const;



	void dx_SetProjectInfo(float fovy, float aspect, float nearz, float farz);
	void dx_SetOrthoInfo(float width, float height, float nearz, float farz);
	void dx_LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);
	void dx_LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);



	//
	// 基本操作  
	//
	void dx_Walk(float dist);          //前后行走  
	void dx_Strafe(float dist);        //左右平移  
	void dx_Pitch(float angle);        //上下点头  
	void dx_RotateY(float angle);      //左右插头  

	void dx_UpDateMatrix();
	XMMATRIX dx_GetViewMatrix() const;
	XMMATRIX dx_GetModeViewMatrix() const;
	XMMATRIX dx_GetProjectMatrix() const;
	XMMATRIX dx_GetViewProjectMatrax() const;
	XMFLOAT4X4 dx_GetIdentityMatrix() const;
private:
	XMFLOAT3	mPosition;
	XMFLOAT3	mRight;
	XMFLOAT3	mUp;
	XMFLOAT3	mLook;

    XMFLOAT3    mEyePos;
    XMFLOAT3    mUpPos;
    XMFLOAT3    mLookPos;

	
	float		mNearZ;
	float		mFarZ;
	float		mAspect;
	float		mFovY;
	float		mNearWindowHeight;
	float		mFarWindowHeight;

	XMFLOAT4X4	mView;
	XMFLOAT4X4	mProj;
};

