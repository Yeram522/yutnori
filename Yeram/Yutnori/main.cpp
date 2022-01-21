#include "YutNoriFrameWork.h"

int main()
{
	std::unique_ptr<FrameWork> pYutNoriFrameWork = std::make_unique<YutNoriFrameWork>();
	pYutNoriFrameWork->MainWorkFrameWork();

	return 0;
}