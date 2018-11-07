#ifndef _MYENGINE_GLBUFFER_H
#define _MYENGINE_GLBUFFER_H

static unsigned int open_buffers = 0;

class GLBuffer
{
public:
	GLBuffer() 
	{
		open_buffers++;
	};

	virtual ~GLBuffer() 
	{
		open_buffers--;
	};
};

#endif //!_MYENGINE_GLBUFFER_H