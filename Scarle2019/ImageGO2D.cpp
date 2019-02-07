#include "pch.h"
#include "ImageGO2D.h"
#include <codecvt>
#include "RenderData.h"


ImageGO2D::ImageGO2D(RenderData* _RD, string _filename)
{

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = "../DDS/" + _filename + ".dds";
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());

	resourceUpload.Begin();

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(_RD->m_d3dDevice.Get(), resourceUpload, wFilename.c_str(),
			m_texture.ReleaseAndGetAddressOf()));


	CreateShaderResourceView(_RD->m_d3dDevice.Get(), m_texture.Get(),
		_RD->m_resourceDescriptors->GetCpuHandle(m_resourceNum = _RD->m_resourceCount++));

	auto uploadResourcesFinished = resourceUpload.End(_RD->m_commandQueue.Get());

	uploadResourcesFinished.wait();
}


ImageGO2D::~ImageGO2D()
{
	m_texture.Reset();
}

void ImageGO2D::Render(RenderData* _RD)
{
	_RD->m_spriteBatch->Draw(_RD->m_resourceDescriptors->GetGpuHandle(m_resourceNum),
		GetTextureSize(m_texture.Get()),
		m_pos, nullptr, m_colour, m_orientation, m_origin, m_scale);
	//TODO::add sprite effects & layer Depth
	//TODO::example stuff for sprite sheet
}

void ImageGO2D::CentreOrigin()
{
	XMUINT2 size = GetTextureSize(m_texture.Get());

	m_origin.x = float(size.x / 2);
	m_origin.y = float(size.y / 2);
}

void ImageGO2D::Reset()
{
	m_texture.Reset();
}
