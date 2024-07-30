#include"TextureManager.h"


TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Initialize(DirectXCommon* directX)
{
	directX_ = directX;
	sRVManager_ = SRVManager::GetInstance();
	descriptorSizeRTV = directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV = directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

uint32_t TextureManager::LoadTexture(const std::string& filePath)
{
	uint32_t index = 0;
	for (uint32_t index_i = 0; index_i < kMaxTexture; index_i++) {
		if (textures_.at(index_i).IsUsed) {
			if (filePath == textures_.at(index_i).name) {
				return index_i;
			}
		}
	}

	for (uint32_t index_i = 0; index_i < kMaxTexture; index_i++) {
		if (textures_.at(index_i).IsUsed == false) {
			index = index_i;
			break;
		}
	}

	//名前としてファイルのパスを登録
	textures_.at(index).name = filePath;

	textures_.at(index).IsUsed = true;

	std::string fullpath = filePath;

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = ImageFileOpen(fullpath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textures_.at(index).textureResource = CreateTextureResource(directX_->GetDevice(), metadata);
	intermediateResource_[index] = UploadTextureData(textures_.at(index).textureResource, mipImages);
	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (metadata.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;

	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	}


	//SRVを作成するDescriptorHeapの場所を決める
	textures_.at(index).textureSrvHandle = sRVManager_->GetDescriptorHandle();
	//先頭はImGuiが使っているので次のを使う
	textures_.at(index).textureSrvHandle.CPU.ptr += directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textures_.at(index).textureSrvHandle.GPU.ptr += directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの作成
	directX_->GetDevice()->CreateShaderResourceView(textures_.at(index).textureResource.Get(), &srvDesc, textures_.at(index).textureSrvHandle.CPU);

	return index;
}

const CD3DX12_GPU_DESCRIPTOR_HANDLE  TextureManager::GetGPUHandle(uint32_t textureHandle)
{
	CD3DX12_GPU_DESCRIPTOR_HANDLE  GPUHandle = textures_.at(textureHandle).textureSrvHandle.GPU;
	return GPUHandle;
}

DirectX::ScratchImage TextureManager::ImageFileOpen(const std::string& filePath)
{
	//テクスチャファイルを読み込みプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);

	if (filePathW.ends_with(L".dds")) {
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else {
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}

	//ファイル名やディレクトリ名がちがうかも
	assert(SUCCEEDED(hr));
	//ミップマップの作成
	DirectX::ScratchImage mipImage{};

	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		mipImage = std::move(image);
	}
	else {
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 4, mipImage);
	}


	return mipImage;
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata)
{
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
	//利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;//細かい設定を行う

	//Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	hr = device->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,//初回のResourceState　Textureは基本読むだけ
		nullptr,//Clear最適値　使わないためnullptr
		IID_PPV_ARGS(&resource)//作成するResourceポインタへのポインタ
	);

	return resource;
}

[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA>subresources;
	DirectX::PrepareUpload(directX_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = directX_->CreateBufferResource(intermediateSize);
	UpdateSubresources(directX_->GetcommandList(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	//Tetureへの転送後は利用できるようにD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	directX_->GetcommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

