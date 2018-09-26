

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



//------------------------------------------------
//  该类用于管理dx11用到的各类buffer
//  其中主要包括:
//  顶点buffer 
//  索引buffer
//  共享资源buffer
//  纹理资源buffer
//  常量buffer,
//-----------------------------------------------

#pragma once

#include "DxConfig.h"

class DXUI_API MDx11BufferManage
{
public:
	MDx11BufferManage(ID3D11Device*	d3dDevice);
	virtual ~MDx11BufferManage();

	MDx11BufferManage(const MDx11BufferManage&) = delete;
	MDx11BufferManage& operator=(const MDx11BufferManage&) = delete;

	//
	// 绑定device
	// 因为创建buffer需要用到device
	//
	void  dx_BindDevice(ID3D11Device*	d3dDevice);

	void dx_Clear();

	void dx_ReleaseBuffer(ID3D11Buffer*& buffer);

    //
    // 创建动态buffer
    //
    template<class T>
    ID3D11Buffer* dx_CreateDynamicBuffer(unsigned count,unsigned bindflag = D3D11_BIND_VERTEX_BUFFER){
        ID3D11Buffer* pDynamicBuffer;
        D3D11_BUFFER_DESC pointBufDesc;

        pointBufDesc.ByteWidth = sizeof(T)* count;
        pointBufDesc.Usage = D3D11_USAGE_DYNAMIC;
        pointBufDesc.BindFlags = bindflag;
        pointBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        pointBufDesc.MiscFlags = 0;
        HRESULT hr = p_d3dDevice->CreateBuffer(&pointBufDesc, nullptr, &pDynamicBuffer);
        if (FAILED(hr)){
            throw std::runtime_error("Create Dynamic Buffer fail.......");
        }
        return pDynamicBuffer;
    }

	template<class T>
	ID3D11Buffer* dx_CreateDynamicBuffer(unsigned count, ID3D11Buffer*& outBuffer,unsigned bindflag = D3D11_BIND_VERTEX_BUFFER){
		safe_release(outBuffer);
		D3D11_BUFFER_DESC pointBufDesc;
		pointBufDesc.ByteWidth = sizeof(T)* count;
		pointBufDesc.Usage = D3D11_USAGE_DYNAMIC;
		pointBufDesc.BindFlags = bindflag;
		pointBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pointBufDesc.MiscFlags = 0;
		HRESULT hr = p_d3dDevice->CreateBuffer(&pointBufDesc, nullptr, &outBuffer);
		if (FAILED(hr)){
			throw std::runtime_error("Create Dynamic Buffer fail.......");
		}
		return outBuffer;
	}

	//
	// 创建顶点buffer，并且保存管理
	//
	template<typename T>
	ID3D11Buffer*  dx_CreateVectexBuffer(T* points, unsigned count, D3D11_USAGE usage = D3D11_USAGE_DEFAULT){
		if (p_d3dDevice){
			D3D11_BUFFER_DESC buffer_desc;
			ZeroMemory(&buffer_desc, sizeof(buffer_desc));
			buffer_desc.Usage = usage;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.ByteWidth = sizeof(T)* count;

			D3D11_SUBRESOURCE_DATA res_data;
			ZeroMemory(&res_data, sizeof(res_data));
			res_data.pSysMem = points;

			ID3D11Buffer* vertex_buffer{ nullptr };

			HRESULT hr = p_d3dDevice->CreateBuffer(&buffer_desc, &res_data, &vertex_buffer);
			if (FAILED(hr)){
				MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
				throw std::runtime_error(par);
			}
			return vertex_buffer;
		}
		else{
			return nullptr;
		}
	}

	//
	// 使用该函数创建的buffer不管理
	//
	template<typename T>
	ID3D11Buffer*  dx_CreateVectexBuffer(T* points, unsigned count, ID3D11Buffer*& outVertex,D3D11_USAGE usage = D3D11_USAGE_DEFAULT){
		if (p_d3dDevice){
			D3D11_BUFFER_DESC buffer_desc;
			ZeroMemory(&buffer_desc, sizeof(buffer_desc));
			buffer_desc.Usage = usage;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.ByteWidth = sizeof(T)* count;

			D3D11_SUBRESOURCE_DATA res_data;
			ZeroMemory(&res_data, sizeof(res_data));
			res_data.pSysMem = points;
			safe_release(outVertex);
			HRESULT hr = p_d3dDevice->CreateBuffer(&buffer_desc, &res_data, &outVertex);
			if (FAILED(hr)){
				MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
				throw std::runtime_error(par);
			}
			return outVertex;
		}
		else{
			return nullptr;
		}
	}

	//
	// 针对XMFLOAT3提供的特化版本
	//
	ID3D11Buffer*  dx_CreateVectexBuffer(XMFLOAT3* points, unsigned count,\
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

	

	//
	// 创建索引buffer
	//
	template<typename T,typename U>
	std::pair<ID3D11Buffer*, ID3D11Buffer*> dx_CreateIndexBuffer(T* points, \
		unsigned points_count, U* indexs, unsigned indexs_count, D3D11_USAGE usage = D3D11_USAGE_DEFAULT){
		if (p_d3dDevice){
			//
			// 先创建顶点缓存
			//
			D3D11_BUFFER_DESC buffer_desc;
			ZeroMemory(&buffer_desc, sizeof(buffer_desc));
			buffer_desc.Usage = usage;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.ByteWidth = sizeof(T)* points_count;

			D3D11_SUBRESOURCE_DATA res_data;
			ZeroMemory(&res_data, sizeof(res_data));
			res_data.pSysMem = points;

			ID3D11Buffer* vertex_buffer{ nullptr };
			HRESULT hr = p_d3dDevice->CreateBuffer(&buffer_desc, &res_data, &vertex_buffer);
			if (FAILED(hr)){
				MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
				throw std::runtime_error(par);
			}

			ZeroMemory(&buffer_desc, sizeof(buffer_desc));
			buffer_desc.Usage = usage;
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.ByteWidth = sizeof(U)* indexs_count;
			buffer_desc.CPUAccessFlags = 0;

			ZeroMemory(&res_data, sizeof(res_data));
			res_data.pSysMem = indexs;

			ID3D11Buffer* index_buffer{ nullptr };
			hr = p_d3dDevice->CreateBuffer(&buffer_desc, &res_data, &index_buffer);
			if (FAILED(hr)){
				MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
				throw std::runtime_error(par);
			}
			return std::make_pair(vertex_buffer, index_buffer);
		}
		else{
			return std::make_pair(nullptr, nullptr);
		}
	}

	//
	// IMMUTABLE
	//
	template<typename T>
	ID3D11Buffer* dx_CreateIndexBuffer(T* __data, size_t size, D3D11_USAGE usage = D3D11_USAGE_DEFAULT){
		if (p_d3dDevice == nullptr)
			return nullptr;

		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.Usage = usage;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.ByteWidth = sizeof(T)* size;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA res_data;
		res_data.pSysMem = __data;
		ID3D11Buffer* index_buffer{ nullptr };
		HRESULT hr = p_d3dDevice->CreateBuffer(&buffer_desc, &res_data, &index_buffer);
		if (FAILED(hr)){
			MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
			throw std::runtime_error(par);
		}
		return index_buffer;
	}

	template<typename T>
	ID3D11Buffer* dx_CreateIndexBuffer(T* __data, size_t size, ID3D11Buffer*& outbuffer,D3D11_USAGE usage = D3D11_USAGE_DEFAULT){
		if (p_d3dDevice == nullptr)
			return nullptr;

		safe_release(outbuffer);
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.Usage = usage;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.ByteWidth = sizeof(T)* size;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA res_data;
		res_data.pSysMem = __data;

		HRESULT hr = p_d3dDevice->CreateBuffer(&buffer_desc, &res_data, &outbuffer);
		if (FAILED(hr)){
			MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
			safe_release(outbuffer);
			return outbuffer;
		}
		return outbuffer;
	}



	//
	// 对于buffer的更新操作
	// 首先buffer必须是可以访问的
	// 也就是说
	// 该buffer的类型不能是default类型
	// 关于这一点使用时候自行把握
	// 获取需要更新的buffer指针之后，在读写完成需要调用
	// dx_EndUpdateBuffer()函数
	// map_type 表示映射的类型，默认情况下使用新分配的内存，将原有内存丢弃
	// D3D11_MAP_WRITE_NO_OVERWRITE 表示我们更新的内存是未初始化的部分，可以在更新的同时进行渲染
	// D3D11_MAP_READ 将buffer从GPU 拷贝到CPU的system内存中,这是相当不高效的
	// start_pos 就是一个内存的定位点，比如我们只需要截取一部分内存进行更新我们就可以通过该参数指定
	// flag 通常为0，具体可以参考SDK文档
	//
	template<typename T>
	T* dx_GetNeedUpdateBuffer(ID3D11DeviceContext* pContex,ID3D11Resource* src_buffer, unsigned start_pos = 0, \
		D3D11_MAP map_type = D3D11_MAP_WRITE_DISCARD, unsigned flag = 0)
	{
		if (!pContex || !src_buffer){
			MString par = MString::Format(L"CreateBuffer Failed\nID3D11DeviceContext is null or src_buffer is null\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
			throw std::runtime_error(par);
		}
		D3D11_MAPPED_SUBRESOURCE __data;
		HRESULT hr = pContex->Map(src_buffer, start_pos, map_type, flag, &__data);
		if (FAILED(hr)){
			MString par = MString::Format(L"Map Failed\nID3D11DeviceContext is null or src_buffer is null\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
			throw std::runtime_error(par);
		}
		T* __result_ptr = reinterpret_cast<T*>(__data.pData);
		return __result_ptr;
	}
	


	//
	// 该函数在调用 dx_GetNeedUpdateBuffer 之后调用方才有效
	// 否则调用该函数毫无意义
	//
	void dx_EndUpdateBuffer(ID3D11DeviceContext* pContex, ID3D11Resource* src_buffer,unsigned start_pos = 0);


	//
	// 针对XMFLOAT3提供的特化版本
	//
	std::pair<ID3D11Buffer*, ID3D11Buffer*> dx_CreateIndexBuffer(XMFLOAT3* points, \
		unsigned points_count, unsigned* indexs, unsigned indexs_count,  D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
		 
	//
	// 常量buffer
	//
	ID3D11Buffer* dx_CreateConstBuffer(size_t size);

	//
	// Ostream_Buffer
	//
	template<class T>
	ID3D11Buffer* dx_CreateOStremBuffer(T t, size_t size){
		assert(p_d3dDevice);
		ID3D11Buffer* p_os_buffer{ nullptr };
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth = sizeof(T)* size;
		HRESULT hr = p_d3dDevice->CreateBuffer(&buffer_desc, nullptr, &p_os_buffer);
		if (FAILED(hr)){
			MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
			throw std::runtime_error(par);
		}
		return p_os_buffer;
	}



	//
	// InstanceBuffer
	//
	template<class T>
	ID3D11Buffer* dx_CreateInstanceBuffer(size_t count, D3D11_BUFFER_DESC* buffer_desc = nullptr){
		D3D11_BUFFER_DESC  desc;
		if (buffer_desc){
			desc = *buffer_desc;
		}
		else{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(T)*count;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
		}
		ID3D11Buffer* instance_buffer{ nullptr };
		HRESULT hr = p_d3dDevice->CreateBuffer(&desc, nullptr, &instance_buffer);
		if (FAILED(hr)){
			MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
			throw std::runtime_error(par);
		}
		return instance_buffer;
	}



	template<class T>
	ID3D11Buffer* dx_CreateInstanceBuffer(size_t count, ID3D11Buffer* outbuffer, D3D11_BUFFER_DESC* buffer_desc = nullptr){
		D3D11_BUFFER_DESC  desc;
		if (buffer_desc){
			desc = *buffer_desc;
		}
		else{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(T)*count;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
		}
		safe_release(outbuffer);
		HRESULT hr = p_d3dDevice->CreateBuffer(&desc, nullptr, &outbuffer);
		if (FAILED(hr)){
			MString par = MString::Format(L"CreateBuffer Failed\nFILE:%1\nFun:%2\nLine:%3", __FILE__, __FUNCTION__, __LINE__);
			throw std::runtime_error(par);
		}
		return outbuffer;
	}


	//
	// 加载纹理资源
	// 创建纹理，同时创建纹理采样器
	//
	std::pair<ID3D11ShaderResourceView*, ID3D11SamplerState*> dx_CreateTexTureResouce(const std::string& filename, \
		D3DX11_IMAGE_LOAD_INFO* pinfo = nullptr, \
		ID3DX11ThreadPump* pthread = nullptr, HRESULT* rel = nullptr);



	//
	// 只创建一张纹理
	//
	ID3D11ShaderResourceView* dx_CreateTexTureResouceOnly(const std::string& filename,
		D3DX11_IMAGE_LOAD_INFO* pinfo = nullptr, \
		ID3DX11ThreadPump* pthread = nullptr, HRESULT* rel = nullptr);



	//
	// 创建纹理数组
	//
	ID3D11ShaderResourceView* dx_CreateTexture2DArraySRV(ID3D11DeviceContext* pContex,\
		std::vector<std::string>& filenames, \
		DXGI_FORMAT format = DXGI_FORMAT_FROM_FILE, \
		unsigned filter = D3DX11_FILTER_NONE,\
		unsigned mipFilter = D3DX11_FILTER_LINEAR, \
		D3DX11_IMAGE_LOAD_INFO* pinfo = nullptr, \
		ID3DX11ThreadPump* pthread = nullptr,\
		HRESULT* rel = nullptr);


	//
	// 创建1D的随机纹理资源
	//
	ID3D11ShaderResourceView* CreateRandomTexture1DSRV(ID3D11Device* device);


	//
	// 创建纹理采样器
	//
	ID3D11SamplerState* dx_CreatTextureSampler(D3D11_SAMPLER_DESC* sample_desc = nullptr);
	
private:
	ID3D11Device*			p_d3dDevice;

	//
	// 纹理资源
	// 可以考虑为由使用者管理
	//
	ID3D11ShaderResourceView* p_texture_resource;//纹理资源
	ID3D11SamplerState*		  p_texture_sampler; //纹理采样器	
};

