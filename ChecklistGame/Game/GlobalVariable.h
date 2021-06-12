#pragma once
#include <vector>
#include <thirdEngine/Renderer/Texture.h>

enum class TaskState {
	Finished,
	NotFinished
};

struct GlobalVariable
{
	GlobalVariable(int taskCount) :currentTime(0), clothes()
	{
		taskState.resize(taskCount);
		for (int i = 0; i != taskCount; i++) {
			taskState[i] = TaskState::NotFinished;
		}

		clothIndex = 0;
	}
	float currentTime;

	// 5 min to finish
	const float totalTime = 300;

	/*!
	 * 0. 栏杆刷漆
	 * 1. 浇花
	 * 2. 修草坪
	 * 3. 整理屋子
	 * 4. 收拾房间
	*/
	std::vector<TaskState> taskState;

	std::vector<Texture> clothes;
	unsigned int clothIndex;
};
