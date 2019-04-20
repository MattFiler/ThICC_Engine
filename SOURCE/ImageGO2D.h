#pragma once
#include "GameObject2D.h"
#include "GameFilepaths.h"
#include <string>

using std::string;
struct RenderData;

class ImageGO2D :
	public GameObject2D
{
public:
	ImageGO2D(string _filename);
	virtual ~ImageGO2D();

	XMUINT2 GetSize() { return size; }

	void Render();

	void CentreOrigin();

	void UpdateSprite(string _filename);

	virtual void Reset() override;

protected:
	void InternalSpriteUpdate(string _filename);

	Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
	int m_resourceNum = -1;

	std::string current_filepath = "ITEM_PLACEHOLDER";
	std::string new_filepath = "ITEM_PLACEHOLDER";

	GameFilepaths m_filepath;
	XMUINT2 size;
};

