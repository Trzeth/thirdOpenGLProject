// 此工具用于将多个动画文件合并为一个文件
// 请保证你的动画文件使用一套骨骼
// 由于 3DSMax 不支持将多个动画导出至一个文件内

// 我一开始以为是内存泄漏 之后发现不是 可能是Exporter的问题？？
// 不知道为什么就是不Work 为什么啊！

// 可能就是Exporter的问题。 I give up. 加载多个模型就多个模型吧。

#include <iostream>
#include <vector>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/SceneCombiner.h>
#include <cassert>

using namespace std;

int main(int argc, char* arg5[])
{
	setlocale(LC_ALL, ".UTF8");

	argc = 4;
	const char* argv[] = { u8"123","TurnAround.DAE:TurnAround",u8"Walk.DAE:Walk", "output3.x:x" };
	//const char* argv[] = { "123","output.dae:TurnAround","output.dae:collada" };

	if (argc < 2) {
		cout << "At least input a file" << endl;
		return 0;
	}

	if (argc == 2)
	{
		string arg(argv[1]);

		if (arg == "-h") {
			cout << "[-h][-f][-i] FileToCombine1 FileToCombine2 ... OutputFile" << endl;
		}
		else if (arg == "-f")
		{
			cout << "Supported output format:" << endl;
			Assimp::Exporter exporter;
			int count = exporter.GetExportFormatCount();
			cout << "File Extension" << "\t" << "Extension You Should Write" << endl;
			for (int i = 0; i != count; i++) {
				auto info = exporter.GetExportFormatDescription(i);
				cout << info->fileExtension << "\t" << info->id << " " << info->description << endl;
			}
		}
		return 0;
	}

	//Combine

	Assimp::Importer importer;
	string s(argv[1]);

	const aiScene* ptr = importer.ReadFile(
		s.substr(0, s.find_last_of(':')), 0);
	if (!ptr || ptr->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !ptr->mRootNode) {
		printf("Assimp error while loading model: %s\n", importer.GetErrorString());
		return -1;
	}

	aiScene* mainScene = nullptr;
	aiCopyScene(ptr, &mainScene);

	std::vector<aiScene*> scenes(argc - 3);
	for (int i = 2; i != argc - 1; i++) {
		string s(argv[i]);
		const aiScene* ptr = importer.ReadFile(
			s.substr(0, s.find_last_of(':')), 0);

		if (!ptr || ptr->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !ptr->mRootNode) {
			printf("Assimp error while loading model: %s\n", importer.GetErrorString());
			return -1;
		}

		aiCopyScene(ptr, &scenes[i - 2]);
	}

	vector<aiString> animationName;
	for (int i = 1; i != argc - 1; i++) {
		string arg(argv[i]);
		if (arg.find_last_of(':') < 0) {
			cout << "You should specify animation name." << endl;
			return -1;
		}
		string animName = arg.substr(arg.find_last_of(':') + 1);
		animationName.push_back(aiString(animName));
	}

	int animationCount = mainScene->mNumAnimations;
	mainScene->mAnimations[0]->mName = animationName[0];

	for (int i = 0; i != scenes.size(); i++)
	{
		assert(scenes[i]->mNumAnimations == 1);
		animationCount += scenes[i]->mNumAnimations;
		scenes[i]->mAnimations[0]->mName = animationName[i + 1];
	}

	// Copy Animation
	aiAnimation** animationList = new aiAnimation * [animationCount];
	for (int i = 0; i != mainScene->mNumAnimations; i++) {
		Assimp::SceneCombiner::Copy(&animationList[i], mainScene->mAnimations[i]);
	}

	mainScene->mAnimations = animationList;

	for (int i = 0; i != scenes.size(); i++)
	{
		for (int j = 0; j != scenes[i]->mNumAnimations; j++)
		{
			Assimp::SceneCombiner::Copy(&mainScene->mAnimations[j + mainScene->mNumAnimations], scenes[i]->mAnimations[j]);
		}
		mainScene->mNumAnimations += scenes[i]->mNumAnimations;
	}

	string outputArg = string(argv[argc - 1]);
	string outputFileName(outputArg.substr(0, outputArg.find_last_of(':')));
	if (outputFileName.find_last_of(':') < 0) {
		cout << "You should specify output format." << endl;
		return -1;
	}
	string outputFormat = outputArg.substr(outputArg.find_last_of(':') + 1);

	Assimp::Exporter exporter;
	int result = exporter.Export(mainScene, outputFormat, outputFileName);
	return result;
}