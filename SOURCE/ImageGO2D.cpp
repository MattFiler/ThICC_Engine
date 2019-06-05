#include "pch.h"
#include "ImageGO2D.h"
#include <codecvt>
#include "RenderData.h" 
#include "DeviceData.h"

/* Create */
ImageGO2D::ImageGO2D(std::string _filename)
{
	new_filepath = _filename;
	InternalSpriteUpdate(_filename);
}

/* Destroy */
ImageGO2D::~ImageGO2D()
{
	m_texture.Reset();
}

/* Render */
void ImageGO2D::Render(DirectX::SpriteBatch* spritebatch)
{
	//We default to nullptr to save passing the default batch every time
	if (spritebatch == nullptr) {
		spritebatch = Locator::getRD()->m_2dSpriteBatchFullscreen.get();
	}

	//If a change is queued, do it before we render
	if (new_filepath != current_filepath) {
		InternalSpriteUpdate(new_filepath);
	}

	if (is_visible) 
	{
		//Render
		spritebatch->Draw(Locator::getRD()->m_2dResourceDescriptors->GetGpuHandle(m_resourceNum),
			size,
			m_pos, nullptr, m_colour, m_orientation, m_origin, m_scale);
	}
}

/* Work out centre */
void ImageGO2D::CentreOrigin()
{
	m_origin.x = float(size.x / 2);
	m_origin.y = float(size.y / 2);
}

/* Queue a sprite switch */
void ImageGO2D::UpdateSprite(std::string _filename) {
	new_filepath = _filename;
}

/* Change sprite */
void ImageGO2D::InternalSpriteUpdate(std::string _filename) {
	current_filepath = _filename;

	//Start upload
	ResourceUploadBatch resourceUpload(Locator::getRD()->m_d3dDevice.Get());
	resourceUpload.Begin();

	//Get filepath
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string fullpath = m_filepath.generateFilepath(_filename, m_filepath.IMAGE);
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	try {
		//Create texture object
		CreateDDSTextureFromFile(Locator::getRD()->m_d3dDevice.Get(), resourceUpload, wFilename.c_str(),
			m_texture.ReleaseAndGetAddressOf());

		//Update image
		CreateShaderResourceView(
			Locator::getRD()->m_d3dDevice.Get(),
			m_texture.Get(),
			Locator::getRD()->m_2dResourceDescriptors->GetCpuHandle(m_resourceNum = Locator::getRD()->m_resourceCount++));

		//Work out size
		size = GetTextureSize(m_texture.Get());
	}
	catch (...) {
		DebugText::print("Loading image '" + _filename + "' FAILED! Is the asset name correct?");
		DebugText::print(fullpath);
		m_texture.Reset();
	}

	//End upload
	auto uploadResourcesFinished = resourceUpload.End(Locator::getRD()->m_commandQueue.Get());
	uploadResourcesFinished.wait();
}

/* Reset */
void ImageGO2D::Reset()
{
	m_texture.Reset();
}

XMUINT2 ImageGO2D::GetSize()
{
	return size;
}

void ImageGO2D::SetSize(Vector2 _size) {
	size.x = _size.x;
	size.y = _size.y;
}