#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	Camera();
	virtual void Run() = 0;

protected:
	bool quit;

	void Exit();

	virtual void Initialize() = 0;
};
#endif
