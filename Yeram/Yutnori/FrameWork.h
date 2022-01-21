#pragma once
class FrameWork
{
protected:
	virtual void BeginFrameWork() {}
	virtual void EndFrameWork() {}

protected:
	virtual void MainWork() = 0;

public:
	void MainWorkFrameWork();
};

