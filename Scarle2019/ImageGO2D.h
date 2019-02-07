#pragma once
#include "GameObject2D.h"
#include <string>

using std::string;
struct RenderData;

//GEP:: Builds on the Base Game Object 2D class to allow images to be displayed to screen.
// These need to made into dds format objects which can be done using the Images project in the Asset Pipeline solution folder.
// Based on this part of the DirectXTK12 wiki documentation
//https://github.com/Microsoft/DirectXTK12/wiki/Sprites-and-textures 

//Check out spritebatch.h to see the other draw functions which allow a number of other effects to be applied to textures
//for example you could create a sprite sheet approach using the source rectangle options

class ImageGO2D :
	public GameObject2D
{
public:
	ImageGO2D(RenderData* _RD, string _filename);
	virtual ~ImageGO2D();

	void Render(RenderData* _RD);

	void CentreOrigin();

	virtual void Reset() override;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
	int m_resourceNum = -1;

};

