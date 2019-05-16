#pragma once
#include <vector>
#include <string>

/* A struct to define a single animated map - we use this multiple times per material (e.g. animated diffuse, animated specular, etc) */
struct AnimatedMap {
	bool is_animated = false;
	float animation_time = 0.0f;
	std::vector<std::string> texture_names;
	std::vector<int> gpu_indexes;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> d3d12_textures;
	int current_anim_index = 0;

	float animation_timer = 0.0f;
};

/* A struct to hold a material's configurations as defined by the ThiCC toolkit. */
struct MaterialConfig {
	int material_index = -1;

	bool is_metallic = false;

	AnimatedMap animated_diffuse;  //Supported by V2 and V3
	AnimatedMap animated_specular; //Supported by V3
	AnimatedMap animated_normal;   //Supported by V3
	AnimatedMap animated_emissive; //Supported by V3
};