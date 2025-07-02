#include <stdlib.h>
#include "object.h"
#include "memory.h"

void AddUser(Object *obj){
    obj->numUsers++;
}

void SetPosition(Object *obj, Vec3 pos){
}
void Rotate(Object *obj, Vec3 rot){
}
void Scale(Object *obj, Vec3 scale){
}

void RemoveUser(Object *obj){
    obj->numUsers--;
    if(obj->numUsers == 0)
    	Object_Free(obj);
 }

Object *Object_Create(){
	Object *obj =  malloc(sizeof(Object));
	memset(obj, 0 , sizeof(Object));
	obj->RemoveUser = RemoveUser;
	obj->AddUser = AddUser;
	obj->SetPosition = SetPosition;
	obj->Rotate = Rotate;
	obj->Scale = Scale;
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

    int k;
    for(k = 0; k < obj->model->numBB; k++){
        BoundingBox child;
        memset(&child, 0, sizeof(BoundingBox));
        child.cube = obj->model->bb[k].cube;
 
		if(child.cube.x < obj->bb.cube.x) obj->bb.cube.x = child.cube.x;       
		if(child.cube.y < obj->bb.cube.y) obj->bb.cube.y = child.cube.y;       
		if(child.cube.z < obj->bb.cube.z) obj->bb.cube.z = child.cube.z;
		if(child.cube.w > obj->bb.cube.w) obj->bb.cube.w = child.cube.w;       
		if(child.cube.h > obj->bb.cube.h) obj->bb.cube.h = child.cube.h;       
		if(child.cube.d > obj->bb.cube.d) obj->bb.cube.d = child.cube.d;       

		child.pos = obj->model->bb[k].pos;
		child.rot = obj->model->bb[k].rot;
		child.scale = obj->model->bb[k].scale;
		BoundingBox_AddChild(&obj->bb,  &child);
    }
	obj->bb.pos = (Vec3){0,0,0};
	obj->bb.rot = (Quat){0,0,0,1};
	obj->bb.scale = (Vec3){1,1,1};

	BoundingBox_UpdatePoints(&obj->bb);
}
void Object_Free(Object *obj){
	free(obj);
}
void Object_Freeze(Object *obj){}
