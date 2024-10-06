#include "Dissolve.h"

void Dissolve::ShaderCompile()
{
	vertexShaderBlob = ShaderCompiler::GetInstance()->CompileShader(L"resources/hlsl/Dissolve/Dissolve.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
	pixelShaderBlob = ShaderCompiler::GetInstance()->CompileShader(L"resources/hlsl/Dissolve/Dissolve.PS.hlsl", L"ps_6_0");
}

void Dissolve::CreateRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	D3D12_ROOT_PARAMETER rootParameters[9] = {};
	//色に関するルートパラメーター
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CSVで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PIXELShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号を0にバインド

	//頂点位置に関するルートパラメーター(WorldTransform)
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CSVで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VERTEXShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号を0にバインド
	//頂点位置に関するルートパラメーター(ViewProjection)
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CSVで使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VERTEXShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 1;//レジスタ番号を1にバインド
	//頂点位置に関するルートパラメーター(Camera)
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CSVで使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//VERTEXShaderで使う
	rootParameters[5].Descriptor.ShaderRegister = 2;//レジスタ番号を1にバインド

	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ
	//テクスチャで使う
	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//0から始まる
	descriptorRange[0].NumDescriptors = 1;//数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	//DescriptorTable
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数
	//ライティングのrootParameters
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CSVで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 1;//レジスタ番号を1にバインド

	//StructuredBuffer用で使う
	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRangeForStructured[1] = {};
	descriptorRangeForStructured[0].BaseShaderRegister = 0;//0から始まる
	descriptorRangeForStructured[0].NumDescriptors = 1;//数は1つ
	descriptorRangeForStructured[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	descriptorRangeForStructured[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	//DescriptorTable
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[6].DescriptorTable.pDescriptorRanges = descriptorRangeForStructured;//Tableの中身の配列を指定
	rootParameters[6].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForStructured);

	//
	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange2[1] = {};
	descriptorRange2[0].BaseShaderRegister = 0;//0から始まる
	descriptorRange2[0].NumDescriptors = 1;//数は1つ
	descriptorRange2[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	descriptorRange2[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	//DescriptorTable
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[7].DescriptorTable.pDescriptorRanges = descriptorRange2;//Tableの中身の配列を指定
	rootParameters[7].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange2);//Tableで利用する数

	//value用
	rootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CSVで使う
	rootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//VERTEXShaderで使う
	rootParameters[8].Descriptor.ShaderRegister = 3;//レジスタ番号を0にバインド

	//Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//0～1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;//ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;//レジスタ番号0番
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&PipelineStateObject_.rootSignature));
	assert(SUCCEEDED(hr));
}

void Dissolve::CreateInputLayOut()
{
	//頂点レイアウト
	inputElementarrayDescs[0].SemanticName = "POSITION";
	inputElementarrayDescs[0].SemanticIndex = 0;
	inputElementarrayDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementarrayDescs[0].InputSlot = 0;
	inputElementarrayDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	//UV座標レイアウト
	inputElementarrayDescs[1].SemanticName = "TEXCOORD";
	inputElementarrayDescs[1].SemanticIndex = 0;
	inputElementarrayDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementarrayDescs[1].InputSlot = 0;
	inputElementarrayDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	//法線レイアウト
	inputElementarrayDescs[2].SemanticName = "NORMAL";
	inputElementarrayDescs[2].SemanticIndex = 0;
	inputElementarrayDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementarrayDescs[2].InputSlot = 0;
	inputElementarrayDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementarrayDescs[3].SemanticName = "WEIGHT";
	inputElementarrayDescs[3].SemanticIndex = 0;
	inputElementarrayDescs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementarrayDescs[3].InputSlot = 1;
	inputElementarrayDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementarrayDescs[4].SemanticName = "INDEX";
	inputElementarrayDescs[4].SemanticIndex = 0;
	inputElementarrayDescs[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;
	inputElementarrayDescs[4].InputSlot = 1;
	inputElementarrayDescs[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc.pInputElementDescs = inputElementarrayDescs;
	inputLayoutDesc.NumElements = _countof(inputElementarrayDescs);
}

void Dissolve::CreateBlendState()
{
	//NormalBlendに設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}

void Dissolve::CreateRasterizarState()
{
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void Dissolve::CreatePipelineStateObject()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = PipelineStateObject_.rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	//書き込むRTV
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//深度 DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込み
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	hr = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&PipelineStateObject_.graphicsPipelineState));
	assert(SUCCEEDED(hr));
}
