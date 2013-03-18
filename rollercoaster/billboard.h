#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "common.h"

class Frame;
class ObjModel;
class Quad;

class Billboard
{
public:
	Billboard();
	~Billboard();

	void create(glm::vec3 p);
	void render();

private:
	Frame *frame_;
	ObjModel *model_;
	Quad *poster_;
};

#endif