/*	Copyright © 2018 Maksim Lukyanov

	This file is part of Cross++ Game Engine.

	Cross++ Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Cross++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#include "Shaders/MultiLightShader.h"
#include "System.h"
#include "Light.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"

using namespace cross;

MultiLightShader::MultiLightShader() :
        MultiLightShader("Engine/Shaders/Sources/MultiLight.vert", "Engine/Shaders/Sources/MultiLight.frag")
{ }

MultiLightShader::MultiLightShader(const String& vert, const String& frag) :
	Shader(vert, frag)
{ }

void MultiLightShader::Compile(){
	Compile(game->GetCurrentScene()->GetLights());
}

void MultiLightShader::Compile(const List<Light*>& lights){
	int pointCount = 0;
	int spotCount = 0;
	int directionalCount = 0;
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::Type::POINT:{
			pointCount++;
		}break;
		case Light::Type::SPOT:{
			spotCount++;
		}break;
		case Light::Type::DIRECTIONAL:{
			directionalCount++;
		}break;
		default:
			CROSS_ASSERT(false, "Unknown light type");
		}
	}

	if(!pointCount && !spotCount && !directionalCount){
		CROSS_FAIL(false, "Current shader required at leas 1 light from scene");
	}

	Compile(pointCount, spotCount, directionalCount);
}

void MultiLightShader::Compile(S32 pointCount, S32 spotCount, S32 directionalCount){
	uPointLights.Clear();
	uDirectionalLights.Clear();
	uPointLights.Clear();
	AddMacro("DIRECTIONAL_LIGHT_COUNT", directionalCount);
	AddMacro("POINT_LIGHT_COUNT", pointCount);
	AddMacro("SPOT_LIGHT_COUNT", spotCount);

	Shader::Compile();
	for(S32 i = 0; i < pointCount; ++i) {
		String structName = "uPointLights[" + String(i) + "]";
		LightUniforms uniforms;
		uniforms.position = glGetUniformLocation(program, String(structName + ".position"));
		uniforms.color = glGetUniformLocation(program, String(structName + ".color"));
		uniforms.intensity = glGetUniformLocation(program, String(structName + ".intensity"));
		uPointLights.Add(uniforms);
	}

	for(S32 i = 0; i < directionalCount; ++i) {
		String structName = "uDirectionalLights[" + String(i) + "]";
		LightUniforms uniforms;
		uniforms.direction = glGetUniformLocation(program, String(structName + ".direction"));
		uniforms.color = glGetUniformLocation(program, String(structName + ".color"));
		uDirectionalLights.Add(uniforms);
	}

	for(S32 i = 0; i < spotCount; ++i) {
		String structName = "uSpotLights[" + String(i) + "]";
		LightUniforms uniforms;
		uniforms.position = glGetUniformLocation(program, String(structName + ".position"));
		uniforms.direction = glGetUniformLocation(program, String(structName + ".direction"));
		uniforms.color = glGetUniformLocation(program, String(structName + ".color"));
		uniforms.intensity = glGetUniformLocation(program, String(structName + ".intensity"));
		uniforms.cut_off = glGetUniformLocation(program, String(structName + ".cut_off"));
		uniforms.outer_cut_off = glGetUniformLocation(program, String(structName + ".outer_cut_off"));
		uSpotLights.Add(uniforms);
	}
}

void MultiLightShader::OnDraw(){
	const List<Light*>& lights = game->GetCurrentScene()->GetLights();
	int pointCount = 0;
	int spotCount = 0;
	int directionalCount = 0;
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::Type::POINT:{
			SAFE(glUniform3fv(uPointLights[pointCount].position, 1, light->GetPosition().GetData()));
			SAFE(glUniform4fv(uPointLights[pointCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform1f(uPointLights[pointCount].intensity, light->GetIntensity()));
			pointCount++;
		}break;
		case Light::Type::SPOT:{
			SAFE(glUniform3fv(uSpotLights[spotCount].position, 1, light->GetPosition().GetData()));
			SAFE(glUniform4fv(uSpotLights[spotCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform1f(uSpotLights[spotCount].intensity, light->GetIntensity()));
			SAFE(glUniform3fv(uSpotLights[spotCount].direction, 1, light->GetEntity()->GetDirection().GetData()));
			SAFE(glUniform1f(uSpotLights[spotCount].cut_off, cosf(light->GetCutOff() / 180.f * PI)));
			SAFE(glUniform1f(uSpotLights[spotCount].outer_cut_off, cosf(light->GetOuterCutOff() / 180.f * PI)));
			spotCount++;
		}break;
		case Light::Type::DIRECTIONAL:
			SAFE(glUniform4fv(uDirectionalLights[directionalCount].color, 1, light->GetColor().GetData()));
			SAFE(glUniform3fv(uDirectionalLights[directionalCount].direction, 1, light->GetEntity()->GetDirection().GetData()));
			directionalCount++;
			break;
		default:
			CROSS_ASSERT(false, "Unknown light type");
		}
	}
}