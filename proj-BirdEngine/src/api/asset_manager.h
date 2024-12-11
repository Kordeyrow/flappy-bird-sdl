#pragma once


class AssetManagerAPI {
public:
	AssetID load_texture(AssetPath path);
private:
	AssetManagerAPI();
	~AssetManagerAPI();
};

