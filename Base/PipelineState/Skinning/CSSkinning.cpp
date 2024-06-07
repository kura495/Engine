#include "CSSkinning.h"

void CSSkinning::ShaderCompile()
{
	computeShaderBlob = ShaderCompiler::GetInstance()->CompileShader(L"resources/hlsl/Skinning.CS.hlsl", L"vs_6_0");
	assert(computeShaderBlob != nullptr);
}

void CSSkinning::CreateRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	//StructuredBuffer用で使う
	//Palette
	D3D12_DESCRIPTOR_RANGE Palette[1] = {};
	Palette[0].BaseShaderRegister = 0;//0から始まる
	Palette[0].NumDescriptors = 1;//数は1つ
	Palette[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	Palette[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = Palette;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(Palette);
	//StructuredBuffer用で使う
	//Vertex
	D3D12_DESCRIPTOR_RANGE Vertex[1] = {};
	Vertex[0].BaseShaderRegister = 1;//0から始まる
	Vertex[0].NumDescriptors = 1;//数は1つ
	Vertex[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	Vertex[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = Vertex;//Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(Vertex);

	//StructuredBuffer用で使う
	//Influences
	D3D12_DESCRIPTOR_RANGE Influences[1] = {};
	Influences[0].BaseShaderRegister = 2;//0から始まる
	Influences[0].NumDescriptors = 1;//数は1つ
	Influences[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	Influences[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = Influences;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(Influences);



	//outputVertices
	D3D12_DESCRIPTOR_RANGE outputVertices[1] = {};
	outputVertices[0].BaseShaderRegister = 0;//0から始まる
	outputVertices[0].NumDescriptors = 1;//数は1つ
	outputVertices[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//SRVを使う
	outputVertices[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&PipelineStateObject_.rootSignature));
	assert(SUCCEEDED(hr));
}

void CSSkinning::CreateInputLayOut()
{
}

void CSSkinning::CreateBlendState()
{
}

void CSSkinning::CreateRasterizarState()
{
}

void CSSkinning::CreatePipelineStateObject()
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.CS = {
		.pShaderBytecode = computeShaderBlob->GetBufferPointer(),
		.BytecodeLength = computeShaderBlob->GetBufferSize(),
	};
	computePipelineStateDesc.pRootSignature = PipelineStateObject_.rootSignature.Get();
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&PipelineStateObject_.graphicsPipelineState));
	assert(SUCCEEDED(hr));
}
