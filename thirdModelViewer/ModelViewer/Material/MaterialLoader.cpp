#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "MaterialLoader.h"

#include <string>
#include <sstream>
#include <rapidjson/document.h>
#include <thirdEngine/Renderer/Shader.h>

using namespace std;
using namespace rapidjson;

Material MaterialLoader::LoadFromFile(const std::string& filepath)
{
	const string curDir(filepath.substr(0, filepath.find_last_of('/') + 1));

	string mtlCode;
	ifstream fs;
	fs.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		fs.open(filepath);
		stringstream ss;
		ss << fs.rdbuf();
		fs.close();
		mtlCode = ss.str();
	}
	catch (ifstream::failure e)
	{
		printf("ERROR::MATERIAL::FILE_NOT_SUCCESFULLY_READ\n");
	}

	Document mtlDocument;

	if (mtlDocument.Parse(mtlCode.c_str()).HasParseError()) {
		printf("ERROR::MATERIAL::FILE_NOT_SUCCESFULLY_PARSE\n");
	}

	string MtlName = mtlDocument.HasMember("Name") ? "Unnamed" : mtlDocument["name"].GetString();

	Shader shader;
	if (mtlDocument.HasMember("Shader")) {
		const Value& shaderInfo = mtlDocument["Shader"];

		ShaderLoader shaderLoader;
		shader = shaderLoader.BuildFromFile(shaderInfo["Vertex"].GetString(), shaderInfo["Fragment"].GetString());
	}

	const Value& properties = mtlDocument["Properties"];
	Material material;
	TextureLoader textureLoader;

	for (Value::ConstValueIterator it = properties.Begin(); it != properties.End(); it++)
	{
		string key = (*it)["KeyName"].GetString();
		const Value& val = (*it)["Value"];
		switch (val.GetType())
		{
		case kStringType:
			// Image Path
			material.SetProperty(key,
				textureLoader.LoadFromFile(TextureType::Diffuse, curDir + val.GetString())
			);
			break;
		case kArrayType:
		{
			const auto& arr = val.GetArray();

			if (!arr.Empty() && arr[0].GetType() == kStringType) {
				//Cubemap
			}
			else
			{
				//vec3 vec4
				switch (arr.Size()) {
				case 3:
					material.SetProperty(key, glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat()));
					break;
				case 4:
					material.SetProperty(key, glm::vec4(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat(), arr[3].GetFloat()));
					break;
				}
			}
			break;
		}
		case kNumberType:
			//flt
			material.SetProperty(key, val.GetFloat());
			break;
		case kTrueType:
		case kFalseType:
		case kNullType:
		default:
			//Default Null
			break;
		}
	}

	return material;
}