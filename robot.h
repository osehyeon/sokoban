#pragma once
#include <gl/glut.h>   
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>

float offset1 = 2.2;
float offset2 = 3.2;
float offset3 = 1.2;
float poly1 = 10.0;
float poly1_speed = 1.0;

void multi_cube(void) {

	glDisable(GL_LIGHTING);
	glColor3f(0, 0.5, 0);
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, 15, 5);
	//body 
	glPushMatrix();
	glScaled(1.0, 1.0, 2.0);
	glutSolidCube(2);
	glPopMatrix();

	// arm left
	glPushMatrix();
	glTranslatef(offset1, 0.0, offset3);
	glTranslatef(-1.0, 0.0, 0.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(poly1, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScaled(1.0, 0.5, 0.5);
	glutSolidCube(2);
	glPopMatrix();
	glTranslatef(offset1, 0.0, 0.0);
	glTranslatef(-1.0, 0.0, 0.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(1.5 * poly1, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glScaled(1.0, 0.5, 0.5);
	glutSolidCube(2);
	glPopMatrix();

	// arm right
	glPushMatrix();
	glTranslatef(-1.0 * offset1, 0.0, offset3);
	glTranslatef(1.0, 0.0, 0.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(poly1, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0.0, 0.0);
	glPushMatrix();
	glScaled(1.0, 0.5, 0.5);
	glutSolidCube(2);
	glPopMatrix();
	glTranslatef(-1.0 * offset1, 0.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(1.5 * poly1, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0.0, 0.0);
	glScaled(1.0, 0.5, 0.5);
	glutSolidCube(2);
	glPopMatrix();

	// head 
	glPushMatrix();
	glTranslatef(0.0, 0.0, offset2);
	glTranslatef(0.0, 0.0, -1.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(poly1, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, 1.0);
	glScaled(0.5, 1.0, 1.0);
	glutSolidCube(2);
	glPopMatrix();

	// leg left
	glPushMatrix();
	glTranslatef(-0.6, 0.0, -1.0 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glPushMatrix();
	glScaled(0.5, 0.5, 1.0);
	glutSolidCube(2);
	glPopMatrix();
	glTranslatef(0.0, 0.0, -0.7 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(1.5 * poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glScaled(0.5, 0.5, 1.0);
	glutSolidCube(2);
	glPopMatrix();

	// leg right
	glPushMatrix();
	glTranslatef(0.6, 0.0, -1.0 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(-poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glPushMatrix();
	glScaled(0.5, 0.5, 1.0);
	glutSolidCube(2);
	glPopMatrix();
	glTranslatef(0.0, 0.0, -0.7 * offset2);
	glTranslatef(0.0, 0.0, 1.0);
	glutSolidSphere(0.5, 10, 10);
	glRotatef(-1.5 * poly1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
	glScaled(0.5, 0.5, 1.0);
	glutSolidCube(2);
	glPopMatrix();

	glEnable(GL_LIGHTING);

}

void poly(void) {
	if (poly1 > 30)
		poly1_speed = 0;
	if (poly1> 30 and poly1_speed == 0)
		poly1_speed = -1;
	if (poly1 < -30)
		poly1_speed = 0;
	if (poly1 < -30 and poly1_speed == 0)
		poly1_speed = 1;
}
