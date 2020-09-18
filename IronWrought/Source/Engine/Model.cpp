#include "stdafx.h"
#include "Model.h"
#include <d3d11.h>

CModel::CModel() : myModelData() {
}

CModel::~CModel() {
}

void CModel::Init(SModelData data) {
	myModelData = std::move(data);
}

CModel::SModelData& CModel::GetModelData() {
	return myModelData;
}
