#ifndef CALENDAR_H
#define CALENDAR_H

struct app{
	float start_time;
	float end_time;
	unsigned int room;
};

typedef struct app* appPTR;

struct ad{
	appPTR* app_arr;
	unsigned int noa;
	unsigned int cap;
};



//FUNCTION PROTOTYPES
//-------------------
//Create AD (appointment diary)
struct ad* 	CreateAD(unsigned int cap);
//Create appointment
struct app*	CreateApp(float start_time, float end_time, unsigned int room);	
//Insert appointment
int 		InsertApp(struct ad* myad, struct app* myapp);
//Remove appointment
//Find appointment in AD
//Destroy AD
void 		DestroyAD(struct ad* myad, struct app* myapp);
//Print AD

#endif
