//include begin
#pragma warning (disable : 4996)
#include <gl/glut.h>   
#include <vector>
#include <iostream>
#include <math.h>
#include <gl/gl.h>
#include <gl/freeglut.h>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
using std::vector;
//include end 

//define begin 
#define	PI	3.1415926
#define WIDTH 1300
#define HEIGHT 800
//define end 

//sturct begin 
struct Vec3f {
	float x, y, z;
};


struct face3i {
	int v1, v2, v3;
	int t1, t2, t3;
	int n1, n2, n3;
};

struct object {
	vector<Vec3f> vertices;
	vector<Vec3f> textures;
	vector<Vec3f> normals;
	vector<face3i> faces;
};


//struct end

//global variable begin 

//global variable end 

Vec3f obstracle;
Vec3f box;
Vec3f agent;
Vec3f goal;
Vec3f agent_speed = { 0,0,0 };
Vec3f box_speed = { 0,0,0 };

Vec3f obstracle_color = { 2.0,0.0,0.0 };
Vec3f box_color = { 2.5,2.5,0.5 };
Vec3f agent_color = { 3,3,3 };
Vec3f goal_color = { 0,0,2 };
Vec3f wall_color = { 2,2,2 };
Vec3f floor1_color = { 1.5,1.5,1.5 };
Vec3f floor2_color = { 0,2.0,1.0 };
Vec3f sky_color = { 0.0,3.0,6.0 };
Vec3f cube_color = { 0,2,0 };
Vec3f sphere_color = { 0,2,0 };
Vec3f cloud_color = { 0,0,3 };
Vec3f zed_color = { 2.0,2.0,0 };
Vec3f teapot_color = { 3.0,0.0,0 };
Vec3f bunny_color = { 3.0, 2.0, 0.0 };


object* obstracle_obj = new object;
object* box_obj = new object;
object* agent_obj = new object;
object* goal_obj = new object;
object* wall_obj = new object;
object* floor1_obj = new object;
object* floor2_obj = new object;
object* sky1_obj = new object;
object* sky2_obj = new object;
object* cube_obj = new object;
object* sphere_obj = new object;
object* cloud_obj = new object;
object* zed_obj = new object;
object* teapot_obj = new object;
object* bunny_obj = new object;

float field = 10.0;

float aSize = 1.0;
float bSize = 1.0;
float oSize = 1.0;
float gSize = 1.0;

int tag_AB = 0;
int tag_AO = 0;
int tag_BO = 0;
int tag_BG = 0;
int tag_BW = 0;
int tag_AG = 0;
int tag_shoot = 0;
int tag_type = 0;

float rotate = 90;
float rotate_speed = 10.0;

float c_distance = 25.0;
float c_theta = -1.0;
float c_phi = 1.0;
float up = 1.0;

int drag, tempX, tempY;
int button = 1;

int score = 0;
char text[] = "SCORE:";
float speed = 0.1;
float redRot = 0.0;

float offset1 = 2.2;
float offset2 = 3.2;
float offset3 = 1.2;
float poly1 = 10.0;
float poly1_speed = 1.0;

GLuint	texture;

BITMAPINFOHEADER	bitmapInfoHeader;
unsigned char* bitmapImage;

unsigned char* LoadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE* filePtr;
	BITMAPFILEHEADER	bitmapFileHeader;
	unsigned char* bitmapImage;
	int					imageIdx = 0;
	unsigned char		tempRGB;

	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// 비트맵 파일 헤드를 읽는다.
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// 윈도우즈 BMP 파일인지 확인한다. 
	if (bitmapFileHeader.bfType != 'MB') {
		fclose(filePtr);
		return	(NULL);
	}

	// 비트맵 정보 헤드를 읽는다.
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// 파일 포인터를 비트맵 데이터의 시작 위치로 옮긴다. 
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// 비트맵 이미지 데이터를 넣을 메모리를 할당한다. 
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	//메모리가 제대로 할당되었는지 확인 
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// 메모리에 비트맵 데이트를 읽어드림 
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// 제대로 들어갔는지 확인 
	if (bitmapImage == NULL) {
		fclose(filePtr);
		return NULL;
	}

	//BMP는 BGR형식이므로 R과 B를 서로 교체해야 함
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;


		//		bitmapImage[imageIdx+1] = 0; 
		//		bitmapImage[imageIdx+2] = 0; 
	}

	// 파일을 닫고 비트맵 이미지 데이터 반환 
	fclose(filePtr);
	return	bitmapImage;
}


void DrawObject(object* thing, Vec3f color)
{
	int f_no = thing->faces.size();

	for (int i = 0; i < f_no; i++) {
		glBegin(GL_POLYGON);
		glColor3f(color.x, color.y, color.z);
		glTexCoord2f(thing->textures[thing->faces[i].t1 - 1].x, thing->textures[thing->faces[i].t1 - 1].y);
		glNormal3f(thing->normals[thing->faces[i].n1 - 1].x, thing->normals[thing->faces[i].n1 - 1].y, thing->normals[thing->faces[i].n1 - 1].z);
		glVertex3f(thing->vertices[thing->faces[i].v1 - 1].x, thing->vertices[thing->faces[i].v1 - 1].y, thing->vertices[thing->faces[i].v1 - 1].z);

		glTexCoord2f(thing->textures[thing->faces[i].t2 - 1].x, thing->textures[thing->faces[i].t2 - 1].y);
		glNormal3f(thing->normals[thing->faces[i].n2 - 1].x, thing->normals[thing->faces[i].n2 - 1].y, thing->normals[thing->faces[i].n2 - 1].z);
		glVertex3f(thing->vertices[thing->faces[i].v2 - 1].x, thing->vertices[thing->faces[i].v2 - 1].y, thing->vertices[thing->faces[i].v2 - 1].z);

		glTexCoord2f(thing->textures[thing->faces[i].t3 - 1].x, thing->textures[thing->faces[i].t3 - 1].y);
		glNormal3f(thing->normals[thing->faces[i].n3 - 1].x, thing->normals[thing->faces[i].n3 - 1].y, thing->normals[thing->faces[i].n3 - 1].z);
		glVertex3f(thing->vertices[thing->faces[i].v3 - 1].x, thing->vertices[thing->faces[i].v3 - 1].y, thing->vertices[thing->faces[i].v3 - 1].z);
		glEnd();
	}
}


void DrawTextureCube(float tx, float ty, float tz)
{
	glPushMatrix();
	glTranslatef(tx, ty, tz);
	/*
	glBegin(GL_QUADS);  // 윗면
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-5, 5, 5);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(5, 5, 5);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(5, 5, -5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5, 5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 앞면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, -5, 5);
	glTexCoord2f(5, 0.0f); glVertex3f(5, 5, 5);
	glTexCoord2f(5, 10.0f); glVertex3f(-5, 5, 5);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-5, -5, 5);
	glEnd();

	glBegin(GL_QUADS);  // 오른쪽 면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, 5, -5);
	glTexCoord2f(50.0f, 0.0f); glVertex3f(5, 5, 5);
	glTexCoord2f(50.0f, 10.0f); glVertex3f(5, -5, 5);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 왼쪽 면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5, -5, 5);
	glTexCoord2f(30.0f, 0.0f); glVertex3f(-5, 5, 5);
	glTexCoord2f(30.0f, 30.0f); glVertex3f(-5, 5, -5);
	glTexCoord2f(0.0f, 30.0f); glVertex3f(-5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 아랫 면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, -5, 5);
	glTexCoord2f(40.0f, 0.0f); glVertex3f(-5, -5, 5);
	glTexCoord2f(40.0f, 40.0f); glVertex3f(-5, -5, -5);
	glTexCoord2f(0.0f, 40.0f); glVertex3f(5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 뒷 면
	glTexCoord2f(5, 5); glVertex3f(5, 5, -5);
	glTexCoord2f(5, 0.0f); glVertex3f(5, -5, -5);
	glTexCoord2f(0, 0.0f); glVertex3f(-5, -5, -5);
	glTexCoord2f(0, 5); glVertex3f(-5, 5, -5);
	glEnd();
	*/

	glBegin(GL_QUADS);  // 윗면 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5, 5, 5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5, 5, 5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5, 5, -5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5, 5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 앞면 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, -5, 5);
	glTexCoord2f(1, 0.0f); glVertex3f(5, 5, 5);
	glTexCoord2f(1, 1.0f); glVertex3f(-5, 5, 5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5, -5, 5);
	glEnd();

	glBegin(GL_QUADS);  // 오른쪽 면 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, 5, -5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5, 5, 5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5, -5, 5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 왼쪽 면 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5, -5, 5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-5, 5, 5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-5, 5, -5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 아랫 면 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, -5, 5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-5, -5, 5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-5, -5, -5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);  // 뒷 면 
	glTexCoord2f(1, 1); glVertex3f(5, 5, -5);
	glTexCoord2f(1, 0.0f); glVertex3f(5, -5, -5);
	glTexCoord2f(0, 0.0f); glVertex3f(-5, -5, -5);
	glTexCoord2f(0, 1); glVertex3f(-5, 5, -5);
	glEnd();

	glPopMatrix();
}

void multi_cube(void) {
	glDisable(GL_TEXTURE_2D);
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, 15, 6);
	//body 
	glPushMatrix();
	glScaled(1.0, 1.0, 2.0);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();

	// arm left
	glPushMatrix();
	glTranslatef(offset1, 0.0, offset3);
	glTranslatef(-1.0, 0.0, 0.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(poly1, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScaled(1.0, 0.5, 0.5);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();
	glTranslatef(offset1, 0.0, 0.0);
	glTranslatef(-1.0, 0.0, 0.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(1.5 * poly1, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glScaled(1.0, 0.5, 0.5);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();

	// arm right
	glPushMatrix();
	glTranslatef(-1.0 * offset1, 0.0, offset3);
	glTranslatef(1.0, 0.0, 0.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(poly1, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0.0, 0.0);
	glPushMatrix();
	glScaled(1.0, 0.5, 0.5);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();
	glTranslatef(-1.0 * offset1, 0.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(1.5 * poly1, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0.0, 0.0);
	glScaled(1.0, 0.5, 0.5);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();

	// head 
	glPushMatrix();
	glTranslatef(0.0, 0.0, offset2);
	glTranslatef(0.0, 0.0, -1.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(poly1, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, 1.0);
	glScaled(0.5, 1.0, 1.0);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();

	// leg left
	glPushMatrix();
	glTranslatef(-0.6, 0.0, -1.0 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glPushMatrix();
	glScaled(0.5, 0.5, 1.0);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();
	glTranslatef(0.0, 0.0, -0.7 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(1.5 * poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glScaled(0.5, 0.5, 1.0);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();

	// leg right
	glPushMatrix();
	glTranslatef(0.6, 0.0, -1.0 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(-poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glPushMatrix();
	glScaled(0.5, 0.5, 1.0);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();
	glTranslatef(0.0, 0.0, -0.7 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	DrawObject(sphere_obj, sphere_color);
	glRotatef(-1.5 * poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glScaled(0.5, 0.5, 1.0);
	glRotatef(90, 0, 0, 1);
	DrawObject(cube_obj, cube_color);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void poly(void) {
	if (poly1 > 30)
		poly1_speed = 0;
	if (poly1 > 30 and poly1_speed == 0)
		poly1_speed = -1;
	if (poly1 < -30)
		poly1_speed = 0;
	if (poly1 < -30 and poly1_speed == 0)
		poly1_speed = 1;
}

void init_light(void)
{
	float	lightPositionA[] = { 0.0f, 20.0f, 50.0f, 1.0f };
	float	diffuseLightA[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float	specularLightA[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float	diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float	specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float	lightPosition[] = { 0.0f, 20.0f, 50.0f, 1.0f };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80.0f);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLightA);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLightA);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPositionA);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);
	glMateriali(GL_FRONT, GL_SHININESS, 128);


	glPushMatrix();  /// 광원에 대한 정의 
	glRotatef(redRot, 0, 0, 1);
	glTranslatef(lightPositionA[0], lightPositionA[1], lightPositionA[2]);
	glPushAttrib(GL_LIGHTING_BIT);  /// 상태 정보 저장 
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(1.0f, 100, 100);
	glEnable(GL_LIGHTING);
	glPopAttrib();                  /// 상태 복구 
	glPopMatrix();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);

	bitmapImage = LoadBitmapFile("butterfly2.bmp", &bitmapInfoHeader);


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
		bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapImage);
}


void file_load(const char* path, object* thing)
{

	FILE* f = fopen(path, "r+");
	if (f == NULL) {
		printf("No such files : %s\n", path);
		return;
	}
	while (true) {
		char lineHeader[128];
		int line = fscanf(f, "%s", lineHeader);
		if (line == EOF) break;
		if (strcmp(lineHeader, "v") == 0) {
			Vec3f vertice;
			fscanf(f, "%f %f %f\n", &vertice.x, &vertice.y, &vertice.z);
			thing->vertices.push_back(vertice);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vec3f texture;
			fscanf(f, "%f %f %f\n", &texture.x, &texture.y, &texture.z);
			thing->textures.push_back(texture);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vec3f normal;
			fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			thing->normals.push_back(normal);
		}

		else if (strcmp(lineHeader, "f") == 0) {
			face3i face;
			int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
				&face.v1, &face.t1, &face.n1, &face.v2, &face.t2, &face.n2, &face.v3, &face.t3, &face.n3);
			thing->faces.push_back(face);
		}
	}
	fclose(f);
}

void draw(void)
{
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(goal.x, goal.y, goal.z);
	glRotatef(90, 1, 0, 0);
	DrawObject(goal_obj, goal_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(obstracle.x, obstracle.y, obstracle.z);
	glRotatef(90, 1, 0, 0);
	DrawObject(obstracle_obj, obstracle_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-obstracle.x, obstracle.y, obstracle.z);
	glRotatef(90, 1, 0, 0);
	DrawObject(obstracle_obj, obstracle_color);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	DrawObject(wall_obj, wall_color);
	DrawObject(floor1_obj, floor1_color);
	DrawObject(floor2_obj, floor2_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(agent.x, agent.y, agent.z);
	glRotatef(90, 1, 0, 0);
	glRotatef(rotate, 0, 1, 0);
	DrawObject(agent_obj, agent_color);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	DrawObject(sky1_obj, sky_color);
	DrawObject(sky2_obj, sky_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 50, 20);
	DrawObject(cloud_obj, cloud_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(30, 10, 0);
	glRotatef(90, 1, 0, 0);
	//glRotatef(90, 0, 1, 0);
	DrawObject(zed_obj, zed_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-30, -10, 0);
	DrawObject(teapot_obj, teapot_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 30, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	DrawObject(bunny_obj, bunny_color);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(box.x, box.y, box.z);
	DrawObject(box_obj, box_color);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	DrawTextureCube(-10, -30, 5);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void tag(void)
{
	float distanceAB = sqrt((agent.x - box.x) * (agent.x - box.x) + (agent.y - box.y) * (agent.y - box.y));
	float distanceAO = sqrt((agent.x - obstracle.x) * (agent.x - obstracle.x) + (agent.y - obstracle.y) * (agent.y - obstracle.y));
	float distanceBO = sqrt((box.x - obstracle.x) * (box.x - obstracle.x) + (box.y - obstracle.y) * (box.y - obstracle.y));
	float distanceAON = sqrt((agent.x + obstracle.x) * (agent.x + obstracle.x) + (agent.y - obstracle.y) * (agent.y - obstracle.y));
	float distanceBON = sqrt((box.x + obstracle.x) * (box.x + obstracle.x) + (box.y - obstracle.y) * (box.y - obstracle.y));
	float distanceBG = sqrt((box.x - goal.x) * (box.x - goal.x) + (box.y - goal.y) * (box.y - goal.y));
	float distanceAG = sqrt((agent.x - goal.x) * (agent.x - goal.x) + (agent.y - goal.y) * (agent.y - goal.y));
	float distanceAE = sqrt((agent.x + 10.0) * (agent.x + 10.0) + (agent.y + 10.0) * (agent.y + 10.0));
	float distanceAEc = sqrt((agent.x - 4.0) * (agent.x - 4.0) + (agent.y + 6.0) * (agent.y + 6.0));
	float distanceAEf = sqrt((agent.x - 0.0) * (agent.x - 0.0) + (agent.y - 5.0) * (agent.y - 5.0));
	float distanceBE = sqrt((box.x + 10.0) * (box.x + 10.0) + (box.y + 10.0) * (box.y + 10.0));
	float distanceBEc = sqrt((box.x - 4.0) * (box.x - 4.0) + (box.y + 6.0) * (box.y + 6.0));
	float distanceBEf = sqrt((box.x - 0.0) * (box.x - 0.0) + (box.y - 5.0) * (box.y - 5.0));
	float distanceAM = sqrt((agent.x - 40) * (agent.x - 40) + (agent.y + 40) * (agent.y + 40));

	if (distanceAB < (aSize + bSize))
		tag_AB = 1;
	else
		tag_AB = 0;

	if (distanceAO < (aSize + oSize) or distanceAON < (aSize + oSize))
		tag_AO = 1;
	else
		tag_AO = 0;

	if (distanceBO < (bSize + oSize) or distanceBON < (bSize + oSize))
		tag_BO = 1;
	else
		tag_BO = 0;

	if (distanceBG < (bSize + gSize))
		tag_BG = 1;
	else
		tag_BG = 0;
	if (distanceAG < (aSize + gSize))
		tag_AG = 1;
	else
		tag_AG = 0;

	if (distanceAE < 8 and distanceAE >= 7.5)
		agent.z = 0.25;
	else if (distanceAE < 7.5 and distanceAE >= 7)
		agent.z = 0.5;
	else if (distanceAE < 7 and distanceAE >= 6.5)
		agent.z = 0.75;
	else if (distanceAE < 6.5 and distanceAE >= 6)
		agent.z = 1.0;
	else if (distanceAE < 6 and distanceAE >= 5.5)
		agent.z = 1.0;
	else if (distanceAE < 5.5 and distanceAE >= 5)
		agent.z = 0.75;
	else if (distanceAE < 5 and distanceAE >= 4.5)
		agent.z = 0.5;
	else if (distanceAE < 4.5 and distanceAE >= 4)
		agent.z = 0.25;

	else if (distanceAEc < 2 and distanceAEc >= 1.5)
		agent.z = 0.25;
	else if (distanceAEc < 1.5 and distanceAEc >= 1.0)
		agent.z = 0.5;
	else if (distanceAEc < 1.0 and distanceAEc >= 0.5)
		agent.z = 0.75;
	else if (distanceAEc < 0.5)
		agent.z = 1.0;

	else if (distanceAEf < 2 and distanceAEf >= 1.5)
		agent.z = 0.25;
	else if (distanceAEf < 1.5 and distanceAEf >= 1.0)
		agent.z = 0.5;
	else if (distanceAEf < 1.0 and distanceAEf >= 0.5)
		agent.z = 0.75;
	else if (distanceAEf < 0.5)
		agent.z = 1.0;

	else if (distanceAM < 30 and distanceAM >= 27)
		agent.z = 1;
	else if (distanceAM < 27 and distanceAM >= 24)
		agent.z = 2;
	else if (distanceAM < 24 and distanceAM >= 21)
		agent.z = 3;
	else if (distanceAM < 21 and distanceAM >= 18)
		agent.z = 4;
	else if (distanceAM < 18 and distanceAM >= 15)
		agent.z = 5;
	else if (distanceAM < 15 and distanceAM >= 12)
		agent.z = 6;
	else if (distanceAM < 12 and distanceAM >= 9)
		agent.z = 7;
	else if (distanceAM < 9 and distanceAM >= 6)
		agent.z = 8;
	else if (distanceAM < 6 and distanceAM >= 3)
		agent.z = 9;
	else if (distanceAM < 3)
		agent.z = 10;

	else
		agent.z = 0;

	if (distanceBE < 8 and distanceBE >= 7.5)
		agent.z = 0.25;
	else if (distanceBE < 7.5 and distanceBE >= 7)
		box.z = 0.5;
	else if (distanceBE < 7 and distanceBE >= 6.5)
		box.z = 0.75;
	else if (distanceBE < 6.5 and distanceBE >= 6)
		box.z = 1.0;
	else if (distanceBE < 6 and distanceBE >= 5.5)
		box.z = 1.0;
	else if (distanceBE < 5.5 and distanceBE >= 5)
		box.z = 0.75;
	else if (distanceBE < 5 and distanceBE >= 4.5)
		box.z = 0.5;
	else if (distanceBE < 4.5 and distanceBE >= 4)
		box.z = 0.25;

	else if (distanceBEc < 2 and distanceBEc >= 1.5)
		box.z = 0.25;
	else if (distanceBEc < 1.5 and distanceBEc >= 1.0)
		box.z = 0.5;
	else if (distanceBEc < 1.0 and distanceBEc >= 0.5)
		box.z = 0.75;
	else if (distanceBEc < 0.5)
		box.z = 1.0;

	else if (distanceBEf < 2 and distanceBEf >= 1.5)
		box.z = 0.25;
	else if (distanceBEf < 1.5 and distanceBEf >= 1.0)
		box.z = 0.5;
	else if (distanceBEf < 1.0 and distanceBEf >= 0.5)
		box.z = 0.75;
	else if (distanceBEf < 0.5)
		box.z = 1.0;

	else
		box.z = 0;

}

void space(void)
{
	/*
	if (agent.x > field - aSize || agent.x < -field+ aSize || agent.y > field - aSize || agent.y < -field + aSize || tag_AG)
	{
		button = 0;
	}
	else {
		button = 1;
	}
	*/
	if (agent.x > field - aSize and agent.x < field + 1.0 + aSize and agent.y < field - aSize and agent.y > -field + aSize)
		button = 0;
	else if (agent.x > -field - 1.0 - aSize and agent.x < -field + aSize and agent.y < field - aSize and agent.y > -field + aSize)
		button = 0;
	else if (agent.y > field - aSize and agent.y < field + 1.0 + aSize and agent.x < field - aSize and agent.x > -field + aSize)
		button = 0;
	else if (agent.y > -field - 1.0 - aSize and agent.y < -field + aSize and agent.x < field - aSize and agent.x > -field + aSize)
		button = 0;
	else
		button = 1;

	if (box.x > field - bSize) {
		box.x = field - bSize, tag_BW = 1;
		box_speed.x = -box_speed.x;
	}
	else if (box.x < -field + bSize) {
		box.x = -field + bSize, tag_BW = 1;
		box_speed.x = -box_speed.x;
	}
	else if (box.y > field - bSize) {
		box.y = field - bSize, tag_BW = 1;
		box_speed.y = -box_speed.y;
	}
	else if (box.y < -field + bSize) {
		box.y = -field + bSize, tag_BW = 1;
		box_speed.y = -box_speed.y;
	}
	else
		tag_BW = 0;

	if (tag_AB and tag_BW)
		button = 0;

}

void reset(void)
{
	srand((unsigned int)time(NULL));
	float posb = rand() % 8 - 4;
	float poso = rand() % 4 - 8;
	float posg = rand() % 16 - 8;
	box = { posb,-5,0 };
	agent = { 0,0,0 };
	obstracle = { poso,3,0 };
	goal = { posg,8,0 };
	rotate = 90;

	box_speed.x = 0;
	box_speed.y = 0;
}


void defeat(void)
{
	score--;
	reset();
}

void victory(void)
{
	score++;
	reset();
}


void event(void)
{

	if (tag_AB) {
		box.x += agent_speed.x;
		box.y += agent_speed.y;
	}

	if (tag_AO)
		defeat();

	if (tag_BO)
		defeat();


	if (tag_BG)
		victory();

	if (tag_shoot) {
		box_speed.x = agent_speed.x;
		box_speed.y = agent_speed.y;
		tag_shoot = 0;
	}

	if (tag_type) {
		agent.x = 20;
		agent.y = -15;
		tag_type = 0;
	}

}

void axis(void) {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // x축 
	glVertex3f(-10.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); // y축 
	glVertex3f(0.0, -10.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);// z축 
	glVertex3f(0.0, 0.0, -10.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}


void camera(void) {
	Vec3f CameraPos;
	CameraPos.x = c_distance * cos(c_theta) * cos(c_phi) + agent.x;
	CameraPos.y = c_distance * sin(c_theta) * cos(c_phi) + agent.y;
	CameraPos.z = c_distance * sin(c_phi) + agent.z;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CameraPos.x, CameraPos.y, CameraPos.z, agent.x, agent.y, agent.z, 0.0, 0.0, up);
}

void mykey(unsigned char key, int x, int y)
{
	agent_speed.x = speed * cos(rotate * 2 * PI / 360);
	agent_speed.y = speed * sin(rotate * 2 * PI / 360);

	switch (key) {
	case 'w':			agent.x += agent_speed.x * button; agent.y += agent_speed.y * button; break;
	case 's':			agent.x -= agent_speed.x; agent.y -= agent_speed.y; break;
	case 'd':			rotate += rotate_speed; break;
	case 'a':			rotate -= rotate_speed; break;
	case 'z':			reset(); break;
	case 'q':			speed += 0.1; break;
	case 'e':			speed -= 0.1; break;
	case 'f':			tag_shoot = 1; break;
	case 'n':			tag_type = 1; break;

	default:	break;
	}

	if (rotate > 360.0)
		rotate = 0;
	else if (rotate < -360.0)
		rotate = 0;

	glutPostRedisplay();
}

void special_key(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:			c_theta += 0.01;	break;
	case GLUT_KEY_RIGHT:		c_theta -= 0.01;	break;
	case GLUT_KEY_DOWN:         c_phi -= 0.01;		break;
	case GLUT_KEY_UP:			c_phi += 0.01;		break;
	default:	break;

	}

	if (c_phi > 3.0 * PI / 2.0)
		c_phi -= 2.0 * PI;
	else if (c_phi < -3.0 * PI / 2.0)
		c_phi += 2.0 * PI;

	if (c_phi > PI / 2.0)
		up = -1.0;
	else if (c_phi < -1.0 * PI / 2.0)
		up = -1.0;
	else
		up = 1.0;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		tempX = x;
		tempY = y;
	}
	glutPostRedisplay();
}

void motion(int x, int y) {

	if (x > tempX) {
		c_theta -= 0.1;
	}
	else if (x < tempX) {
		c_theta += 0.1;
	}
	if (y > tempY) {
		c_phi += 0.1;
	}
	else if (y < tempY) {
		c_phi -= 0.1;
	}

	tempX = x;
	tempY = y;

	/*
	if (c_phi > 3.0 * PI / 2.0)
		c_phi -= 2.0 * PI;
	else if (c_phi < -3.0 * PI / 2.0)
		c_phi += 2.0 * PI;

	if (c_phi > PI / 2.0)
		up = -1.0;
	else if (c_phi < -1.0 * PI / 2.0)
		up = -1.0;
	else
		up = 1.0;
	*/

	if (c_phi < 0.1)
		c_phi = 0.1;
	else if (c_phi > 1.5)
		c_phi = 1.5;
	glutPostRedisplay();
}

void mouse_wheel(int wheel, int direction, int x, int y)
{
	if (direction > 0)
	{
		c_distance += 1.0;
	}
	else if (direction < 0)
	{
		c_distance -= 1.0;
	}
	else
	{
		c_distance = c_distance;
	}
	glutPostRedisplay();
}

void frame_reset(void) {  //화면 색상 초기화 및 버퍼 초기화 
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderBitmapCharacterString(float x, float y, float z, void* font, char* string) {
	char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);
}

void renderBitmapCharacterInt(float x, float y, float z, void* font, int score) {
	char c[100];
	sprintf_s(c, "%d", score);
	glRasterPos3f(x, y, z);
	for (int i = 0; c[i] != '\0'; i++)
		glutBitmapCharacter(font, c[i]);
}
void renderBitmapCharacterStringInt(float x, float y, float z, void* font, char* string, int score) {
	char c1[100];
	char* c2;
	sprintf_s(c1, "%d", score);
	glRasterPos3f(x, y, z);
	for (c2 = string; *c2 != '\0'; c2++)
		glutBitmapCharacter(font, *c2);
	for (int i = 0; c1[i] != '\0'; i++)
		glutBitmapCharacter(font, c1[i]);
}


void display(void)
{
	frame_reset();
	camera();
	//axis();
	init_light();
	draw();
	tag();
	event();
	space();
	renderBitmapCharacterStringInt(-15, 10, 3, GLUT_BITMAP_TIMES_ROMAN_24, text, score);
	multi_cube();
	poly();
	glFlush();
	glutSwapBuffers();


	poly1 += poly1_speed;
	redRot += 0.1;

	box.x += box_speed.x;
	box.y += box_speed.y;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 15.0);
	gluPerspective(60.0, 1.0, 1.0, 400.0);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	file_load("goal.obj", goal_obj);
	file_load("box.obj", box_obj);
	file_load("agent.obj", agent_obj);
	file_load("wall.obj", wall_obj);
	file_load("obstracle.obj", obstracle_obj);
	file_load("floor1.obj", floor1_obj);
	file_load("floor2.obj", floor2_obj);
	file_load("sky1.obj", sky1_obj);
	file_load("sky2.obj", sky2_obj);
	file_load("cube.obj", cube_obj);
	file_load("sphere.obj", sphere_obj);
	file_load("cloud.obj", cloud_obj);
	file_load("zed.obj", zed_obj);
	file_load("teapot.obj", teapot_obj);
	file_load("bunny.obj", bunny_obj);

	srand((unsigned int)time(NULL));
	float posb = rand() % 8 - 4;
	float poso = rand() % 4 - 8;
	float posg = rand() % 16 - 8;
	box = { posb,-5,0 };
	agent = { 0,0,0 };
	obstracle = { poso,3,0 };
	goal = { posg,8,0 };

}

void main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Title");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);  //화면의 width 및 height가 변할 시 발생 
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMouseWheelFunc(mouse_wheel);
	glutSpecialFunc(special_key);
	glutKeyboardFunc(mykey);
	glutMainLoop();
}