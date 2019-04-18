#include "pch.h"
#include "ImageGO2D.h"
#include <codecvt>
#include "RenderData.h" 

ImageGO2D::ImageGO2D(string _filename)
{
	new_filepath = _filename;
	InternalSpriteUpdate(_filename);
}

ImageGO2D::~ImageGO2D()
{
	m_texture.Reset();
}

void ImageGO2D::Render()
{
	if (new_filepath != current_filepath) {
		InternalSpriteUpdate(new_filepath);
	}

	Locator::getRD()->m_spriteBatch->Draw(Locator::getRD()->m_resourceDescriptors->GetGpuHandle(m_resourceNum),
		GetTextureSize(m_texture.Get()),
		m_pos, nullptr, m_colour, m_orientation, m_origin, m_scale);
	//TODO::add sprite effects & layer Depth
	//TODO::example stuff for sprite sheet
}

void ImageGO2D::CentreOrigin()
{
	m_origin.x = float(size.x / 2);
	m_origin.y = float(size.y / 2);
}

void ImageGO2D::UpdateSprite(string _filename) {
	new_filepath = _filename;
}

void ImageGO2D::InternalSpriteUpdate(string _filename) {
	current_filepath = _filename;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = m_filepath.generateFilepath(_filename, m_filepath.IMAGE);
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	ResourceUploadBatch resourceUpload(Locator::getRD()->m_d3dDevice.Get());

	resourceUpload.Begin();

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(Locator::getRD()->m_d3dDevice.Get(), resourceUpload, wFilename.c_str(),
			m_texture.ReleaseAndGetAddressOf()));


	CreateShaderResourceView(
		Locator::getRD()->m_d3dDevice.Get(),
		m_texture.Get(),
		Locator::getRD()->m_resourceDescriptors->GetCpuHandle(m_resourceNum = Locator::getRD()->m_resourceCount++));

	size = GetTextureSize(m_texture.Get());

	auto uploadResourcesFinished = resourceUpload.End(Locator::getRD()->m_commandQueue.Get());

	uploadResourcesFinished.wait();
}

void ImageGO2D::Reset()
{
	m_texture.Reset();
}
