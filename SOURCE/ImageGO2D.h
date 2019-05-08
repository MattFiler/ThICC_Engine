#pragma once
#include "GameObject2D.h"
#include "GameFilepaths.h"
#include <string>

struct ThICC_RenderData;

class ImageGO2D :
	public GameObject2D
{
public:
	ImageGO2D(std::string _filename);
	virtual ~ImageGO2D();

	XMUINT2 GetSize();
	void SetSize(Vector2 _size);

	void Render();

	void CentreOrigin();

	void UpdateSprite(std::string _filename);

	virtual void Reset() override;

	std::string& GetFilepath() { return current_filepath; }

protected:
	void InternalSpriteUpdate(std::string _filename);

	Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;
	int m_resourceNum = -1;

	std::string current_filepath = "ITEM_PLACEHOLDER";
	std::string new_filepath = "ITEM_PLACEHOLDER";

	GameFilepaths m_filepath;
	XMUINT2 size;
};

