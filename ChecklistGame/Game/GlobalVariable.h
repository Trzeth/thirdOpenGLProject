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

		clothIndex = 3;
	}
	float currentTime;

	// 5 min to finish
	const float totalTime = 300;

	/*!
	 * 0. 栏杆刷漆
	 * 1. 浇花
	 * 2. 收拾房间
	 * 3. 换衣服
	 * 4. 买材料
	 * 5. 收集食材
	*/
	std::vector<TaskState> taskState;

	std::vector<Texture> clothes;
	unsigned int clothIndex;
};
