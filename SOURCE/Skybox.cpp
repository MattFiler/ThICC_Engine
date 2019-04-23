#include "pch.h"
#include "Skybox.h"
#include "RenderData.h"

Skybox::Skybox() {
	skybox = GeometricPrimitive::CreateSphere(1.0f);
}

Skybox::~Skybox() {
	skybox.reset();
	m_irradianceIBL.Reset();
	m_radianceIBL.Reset();
}

void Skybox::Render() {
	skybox->Draw(Locator::getRD()->m_commandList.Get());
}