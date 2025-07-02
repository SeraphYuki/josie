#include <stdlib.h>
#include "object.h"
#include "memory.h"

static void AddUser(Object *obj){
    obj->numUsers++;
}


static void RemoveUser(Object *obj){
    obj->numUsers--;
    if(obj->numUsers == 0)
    	Object_Free(obj);
 }

static void ObjUpdate(Object *obj){
 	BoundingBox_UpdatePoints(&obj->bb);
 }


Object *Object_Create(){
	Object *obj =  malloc(sizeof(Object));
	memset(obj, 0 , sizeof(Object));
	obj->RemoveUser = RemoveUser;
	obj->AddUser = AddUser;
	obj->ObjUpdate = ObjUpdate;
	return obj;
 }

Object *Object_Copy(Object *obj){
	Object *obj2 = malloc(sizeof(Object));
	memcpy(obj2, obj, sizeof(Object));
	return obj2;
}

void Object_SetModel(Object *obj, Model *model){
	obj->model= model; 
	memset(&obj->bb, 0, sizeof(BoundingBox));

	if(obj->model->numBB == 1){
        obj->bb.cube = obj->model->bb[0].cube;
		obj->bb.pos = obj->model->bb[0].pos;
		obj->bb.rot = obj->model->bb[0].rot;
		obj->bb.scale = obj->model->bb[0].scale;
		BoundingBox_UpdatePoints(&obj->bb);
		return;
	}

	obj->bb.cube.x = obj->bb.cube.y = obj->bb.cube.z = HUGE_VAL;
	obj->bb.cube.w = obj->bb.cube.h = obj->bb.cube.d = -HUGE_VAL;
	obj->bb.pos = (Vec3){0,0,0};
	obj->bb.rot = (Vec3){0,0,0};
	obj->bb.scale = (Vec3){1,1,1};
    int k;
    for(k = 0; k < obj->model->numBB; k++){
        BoundingBox child;
        memset(&child, 0, sizeof(BoundingBox));
        child.cube = obj->model->bb[k].cube;

		child.pos = obj->model->bb[k].pos;
		child.rot = obj->model->bb[k].rot;
		child.scale = obj->model->bb[k].scale;
		BoundingBox_AddChild(&obj->bb,  &child);
		BoundingBox_UpdatePoints(&child);
 
		if(child.wsCube.x < obj->bb.cube.x) obj->bb.cube.x = child.wsCube.x;       
		if(child.wsCube.y < obj->bb.cube.y) obj->bb.cube.y = child.wsCube.y;       
		if(child.wsCube.z < obj->bb.cube.z) obj->bb.cube.z = child.wsCube.z;
		if(child.wsCube.w+child.wsCube.z > obj->bb.cube.w) 
			obj->bb.cube.w = child.wsCube.w+child.wsCube.z;
		if(child.wsCube.h+child.wsCube.y > obj->bb.cube.h)
			obj->bb.cube.h = child.wsCube.h+child.wsCube.y;       
		if(child.wsCube.d+child.wsCube.z > obj->bb.cube.d)
			obj->bb.cube.d = child.wsCube.d+child.wsCube.z;
    }

	obj->bb.cube.w -= obj->bb.cube.x;
    obj->bb.cube.h -= obj->bb.cube.y;
    obj->bb.cube.d -= obj->bb.cube.z;

	BoundingBox_UpdatePoints(&obj->bb);
}
void Object_Free(Object *obj){
	free(obj);
}
void Object_Freeze(Object *obj){}
