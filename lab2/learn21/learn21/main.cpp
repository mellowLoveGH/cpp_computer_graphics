#pragma once
#include <windows.h>
#include "main.h"
#include "freeimage.h"
#include <string>
#include <iostream>
using namespace std;

#define RELATIVEPATH "\\model\\"
#define MODELFILE "robot.3ds"
#define LEFTARMNAME "armL"
#define lEFTHANDNAME "handL"

//
char* constToChar(const char* str) {
	char* lan = new char[11];
	strcpy_s(lan, 11, str);
	return lan;
}
//
void printMat(float Mat[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << " " << Mat[i][j];
		}
		cout << "" << endl;
	}
		
}

FIBITMAP **g_dib;
GLubyte **g_textureImage;

float astart = 0, arange = -PI, apos = astart + arange / 2, astep = 0.1;
float hstart = PI / 2, hrange = -PI, hpos = hstart + hrange / 2, hstep = 0.1;
float a = acos(1 - 2 * (apos - astart) / arange), h = acos(1 - 2 * (hpos - hstart) / hrange);


//���½ڵ�ľ���
void UpdateMatrix(Lib3dsNode *node)
{
	if (!node) return;
	float newMatrix[4][4], t[4][4];
	float tx, ty, tz, theta, rx, ry, rz, sx, sy, sz, (*localMatrix)[4] = NULL;

	SetIdentity(t);

	//��ȡ���ڵ�ľ���
	if (node->parent)
		GetNodeMatrix(newMatrix, node->parent);
	else SetIdentity(newMatrix);

	//�Բ�ͬ�ڵ�����Դ�
	if (strcmp(node->name, LEFTARMNAME) == 0)
	{
		//����Ǹ첲�ڵ�

		//node����ڸ��ڵ�ľֲ�����ϵ�ĸ��任��������
		tx = ATX; ty = ATY; tz = ATZ;
		theta = ATHETA; rx = ARX; ry = ARY; rz = ARZ;
		sx = ASX; sy = ASY; sz = ASZ;

		//�ֲ��任������ArmComT;
		localMatrix = ArmComT;
	}
	else if (strcmp(node->name, lEFTHANDNAME) == 0)
	{
		//������ֽڵ�

		tx = HTX; ty = HTY; tz = HTZ;
		theta = HTHETA; rx = HRX; ry = HRY; rz = HRZ;
		sx = HSX; sy = HSY; sz = HSZ;

		localMatrix = HandComT;
	}
	else return;



	//3dģ�����ɶ���ڵ�(node)��ɣ��ڵ�֮���и��ӹ�ϵ����������ڵ����ֱ۽ڵ�ĸ��ڵ㣩��
	//Ҫ�õ�������Ч�����ڵ�Ӧ����������丸�ڵ�ľֲ�����ϵ�任�������������ľֲ�����ϵ�任
	//node����ڸ��ڵ�ֲ�����ϵ�任�ĸ�����,�Լ����������ֲ�����ϵ�ı任���󱾺������ṩ
	//����newMatrix�洢��node�ĸ��ڵ�ı任����
	//�������ṩ�ı�����newMatrix���м��㣬�õ�����node�ı任���󣬾���ļ���� transformation.h

	//��Ӵ���
	//
	float Mat[4][4];
	GetNodeMatrix(Mat, node);
	//
	float Txyz[4][4];
	float R[4][4];
	float Sxyz[4][4];
	matTxyz(Txyz, tx, ty, tz);
	matR(R, theta, rx, ry, rz);	
	matSxyz(Sxyz, sx, sy, sz);
	//
	MatMul(localMatrix, R, localMatrix);
	MatMul(localMatrix, Sxyz, localMatrix);
	MatMul(localMatrix, Txyz, localMatrix);
	MatMul(localMatrix, newMatrix, localMatrix);
	//

	if (strcmp(node->name, lEFTHANDNAME) == 0) {
		//cout << "update hand" << endl;

	}
	else {		
		//cout << "update arm" << endl;		
		//set the child
		float cMat[4][4];
		//GetNodeMatrix(cMat, node->childs);
		tx = HTX; ty = HTY; tz = HTZ;
		theta = HTHETA; rx = HRX; ry = HRY; rz = HRZ;
		sx = HSX; sy = HSY; sz = HSZ;
		matTxyz(Txyz, tx, ty, tz);
		matR(R, theta, rx, ry, rz);
		matSxyz(Sxyz, sx, sy, sz);
		matRz(HandComT, 0);
		Copy(cMat, HandComT);
		MatMul(cMat, R, cMat);
		MatMul(cMat, Sxyz, cMat);
		MatMul(cMat, Txyz, cMat);
		MatMul(cMat, localMatrix, cMat);
		SetNodeMatrix(cMat, node->childs);
	}	
	
	

	//���¾���
	SetNodeMatrix(localMatrix, node);
	//cout << "reset " << node->name << endl;
}

//��ʼ��
void MyInit()
{
	gl_width = defSCREEN_WIDTH;
	gl_height = defSCREEN_HEIGHT;

	matRx(ArmComT, astart + arange * (1 - cos(a)) / 2);
	matRz(HandComT, hstart + hrange * (1 - cos(h)) / 2);

	SetIdentity(RobotT);
	SetIdentity(RobotS);
	SetIdentity(RobotR);

	matTxyz(RobotT, 0, 0, 0);
	matSxyz(RobotS, 0.15, 0.15, 0.15);
	//matRy(RobotR, -PI / 6);
	matRy(RobotR, 0);

	g_pixel.x = g_pixel.y = 0;

	texName = NULL;

	char filename[255];
	//����ģ��
	::GetCurrentDirectoryA(255, filename);
	strcat_s(filename, RELATIVEPATH);
	strcat_s(filename, MODELFILE);
	g_model = lib3ds_file_open(filename);
	if (!g_model) {
		printf("����ģ�ͳ���");
		exit(1);
	}
	lib3ds_file_eval(g_model, 0);

	//����
	int nmat = g_model->nmaterials;
	if (nmat != 0) {
		g_dib = new FIBITMAP*[nmat];
		g_textureImage = new GLubyte*[nmat];
		GLubyte **pixels = new  GLubyte*[nmat];

		//��������
		for (int i = 0; i < nmat; i++)
		{
			//��ȡ�����ļ�����·��
			::GetCurrentDirectoryA(255, filename);
			strcat_s(filename, RELATIVEPATH);
			strcat_s(filename, g_model->materials[i]->texture1_map.name);

			// ���ͼ���ļ�������
			FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filename, 0);

			//��ȡ����ָ��
			g_dib[i] = FreeImage_Load(fifmt, filename, 0);

			//g_dib[i]=FreeImage_ConvertTo24Bits(g_dib[i]);

			//Ϊ�������ݷ���ռ�
			g_textureImage[i] = new GLubyte[FreeImage_GetWidth(g_dib[i]) * FreeImage_GetHeight(g_dib[i]) * 3];

			//����������ݵ�ָ��
			pixels[i] = (GLubyte*)FreeImage_GetBits(g_dib[i]);

			//ͼ���ļ��Ĵ���˳���� BGR, ������Ҫ����ת����RGB����������
			for (int pix = 0; pix < FreeImage_GetWidth(g_dib[i]) * FreeImage_GetHeight(g_dib[i]); pix++)
			{
				g_textureImage[i][pix * 3 + 0] = pixels[i][pix * 3 + 2];
				g_textureImage[i][pix * 3 + 1] = pixels[i][pix * 3 + 1];
				g_textureImage[i][pix * 3 + 2] = pixels[i][pix * 3 + 0];
			}

		}

	}

	//���¸첲���ֵľ���
	//UpdateMatrix(GetNodeByName(g_model->nodes, constToChar(LEFTARMNAME) ));
	//UpdateMatrix(GetNodeByName(g_model->nodes, constToChar(lEFTHANDNAME) ));
	UpdateMatrix(GetNodeByName(g_model->nodes, LEFTARMNAME));
	UpdateMatrix(GetNodeByName(g_model->nodes, lEFTHANDNAME));
}


//��������Ϊname�Ľڵ�
Lib3dsNode* GetNodeByName(Lib3dsNode*node, const char*name)
{
	if (node) {
		if (strcmp(node->name, name) == 0)return node;
		else {
			Lib3dsNode *n = GetNodeByName(node->childs, name);
			if (n)return n;
			else return GetNodeByName(node->next, name);
		}
	}
	else return NULL;
}


//��ȡ�ڵ����
bool GetNodeMatrix(float(*M)[4], Lib3dsNode *node)
{
	if (node == NULL)return false;
	Copy(M, node->matrix);
	Transpose(M, M);
	return true;
}

//���ýڵ����
bool SetNodeMatrix(float(*M)[4], Lib3dsNode *node)
{
	if (node == NULL)return false;
	float t[4][4];
	Copy(t, M);
	Transpose(t, t);
	Copy(node->matrix, t);
	return true;
}

//���ƽڵ�
void RenderNode(Lib3dsNode *node)
{
	Lib3dsNode		*p;
	Lib3dsMesh		*mesh;
	Lib3dsFace		*face;
	Lib3dsMaterial  *mat;
	Lib3dsMeshInstanceNode *meshData;

	Lib3dsVector	*norm_verts;
	Lib3dsVector	*norm_faces;

	int         i;
	unsigned	fi;
	float		M[4][4];

	assert(node);
	assert(g_model);

	if (node->type == LIB3DS_NODE_MESH_INSTANCE)
	{
		if (strcmp(node->name, "$$$DUMMY") == 0) {
			return;
		}

		if (!node->user_id)
		{
			mesh = lib3ds_file_mesh_for_node(g_model, node);
			assert(mesh);
			if (!mesh) {
				return;
			}

			node->user_id = glGenLists(1);
			glNewList(node->user_id, GL_COMPILE);

			norm_verts = (Lib3dsVector*)malloc(3 * sizeof(Lib3dsVector)*mesh->nfaces);
			norm_faces = (Lib3dsVector*)malloc(sizeof(Lib3dsVector)*mesh->nfaces);

			lib3ds_matrix_copy(M, mesh->matrix);
			lib3ds_matrix_inv(M);
			glMultMatrixf(&M[0][0]);

			lib3ds_mesh_calculate_face_normals(mesh, (float(*)[3])norm_faces);
			lib3ds_mesh_calculate_vertex_normals(mesh, (float(*)[3])norm_verts);

			for (fi = 0; fi < mesh->nfaces; ++fi) {
				face = &(mesh->faces[fi]);
				mat = 0;

				if (face->material >= 0 && face->material < g_model->nmaterials) {
					mat = g_model->materials[face->material];

				}

				if (texName)
					glBindTexture(GL_TEXTURE_2D, texName[face->material]);

				if (mat)
				{
					static GLfloat a[4] = { 0,0,0,1 };
					float s;

					glMaterialfv(GL_FRONT, GL_AMBIENT, a);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);

					//s = pow(2, 10.0*mat->shininess);
					s = mat->shininess;

					if (s > 128.0) {
						s = 128.0;
					}

					glMaterialf(GL_FRONT, GL_SHININESS, s);
				}
				else {
					float a[] = { 0.2, 0.2, 0.2, 1.0 };
					float d[] = { 0.8, 0.8, 0.8, 1.0 };
					float s[] = { 0.0, 0.0, 0.0, 1.0 };

					glMaterialfv(GL_FRONT, GL_AMBIENT, a);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
					glMaterialfv(GL_FRONT, GL_SPECULAR, s);

				}

				glBegin(GL_TRIANGLES);

				glNormal3fv(norm_faces[fi].v);

				for (i = 0; i < 3; ++i) {
					glNormal3fv(norm_verts[3 * fi + i].v);
					glTexCoord2fv(mesh->texcos[face->index[i]]);
					glVertex3fv(mesh->vertices[face->index[i]]);

				}

				glEnd();
			}

			free(norm_faces);
			free(norm_verts);

			glEndList();
		}

		if (node->user_id) {
			glPushMatrix();

			meshData = (Lib3dsMeshInstanceNode*)node;

			glMultMatrixf(&node->matrix[0][0]);

			float t[4][4];
			SetIdentity(t);
			matTxyz(t, -meshData->pivot[0], -meshData->pivot[1], -meshData->pivot[2]);
			Transpose(t, t);
			glMultMatrixf(&t[0][0]);

			glCallList(node->user_id);

			glPopMatrix();
		}
	}

	// �ݹ�
	for (p = node->childs; p != 0; p = p->next) {
		RenderNode(p);
	}

}


//����
void Draw(Lib3dsFile *model)
{
	if (!g_model) {
		return;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	for (Lib3dsNode *nod = g_model->nodes; nod != 0; nod = nod->next) {
		RenderNode(nod);
	}

	glPopMatrix();

}


//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	static float armtheta = 0.6, handtheta = PI / 2;//�ڶ��Ƕ�
	static float t[4][4];

	SetIdentity(t);

	if (uchKey == 27) {
		exit(0);
	}
	switch (uchKey) {

		//������ƽ��
	case 'd':
	case 'D'://x����
		matTxyz(t, 10, 0, 0);
		MatMul(RobotT, t, RobotT);
		break;
	case 'a':
	case 'A'://x��С
		matTxyz(t, -10, 0, 0);
		MatMul(RobotT, t, RobotT);
		break;
	case 'w':
	case 'W'://y����
		matTxyz(t, 0, 10, 0);
		MatMul(RobotT, t, RobotT);
		break;
	case 's':
	case 'S'://y��С
		matTxyz(t, 0, -10, 0);
		MatMul(RobotT, t, RobotT);
		break;
	case 'q':
	case 'Q'://z����
		matTxyz(t, 0, 0, 10);
		MatMul(RobotT, t, RobotT);
		break;
	case 'e':
	case 'E'://z��С
		matTxyz(t, 0, 0, -10);
		MatMul(RobotT, t, RobotT);
		break;

		//����������
	case 'z':
	case 'Z'://�Ŵ�
		matSxyz(t, 1.05, 1.05, 1.05);
		MatMul(RobotS, t, RobotS);
		break;
	case 'x':
	case 'X'://��С
		matSxyz(t, 0.95, 0.95, 0.95);
		MatMul(RobotS, t, RobotS);
		break;

		//�ڶ�
	case 'i':
	case 'I'://�첲
		a += PI2 * astep / abs((long) arange);
		if (a >= PI2)a -= PI2;
		matRx(ArmComT, astart + arange * (1 - cos(a)) / 2);
		UpdateMatrix(GetNodeByName(g_model->nodes, LEFTARMNAME));
		UpdateMatrix(GetNodeByName(g_model->nodes, lEFTHANDNAME));
		break;
	case 'k':
	case 'K'://��
		h += PI2 * hstep / abs((long) hrange);
		if (h >= PI2)h -= PI2;
		matRz(HandComT, hstart + hrange * (1 - cos(h)) / 2);
		UpdateMatrix(GetNodeByName(g_model->nodes, lEFTHANDNAME));
		break;
	}
	glutPostRedisplay();
}


//��Ӧ���ⰴ��
void Special(int iKey, int iXPos, int iYPos)
{

}

//��Ӧ��갴��
void Mouse(int iButton, int iState, int iXPos, int iYPos)
{
	if (iState == GLUT_DOWN)
	{
		g_pixel.x = iXPos;
		g_pixel.y = iYPos;
	}
}

//��Ӧ����һ����갴ť�ƶ����
void Motion(int iXPos, int iYPos)
{
	float t[4][4];
	GLfloat rx = iYPos - g_pixel.y, ry = iXPos - g_pixel.x, rz = 0.0;
	GLfloat delta = PI / 60;

	SetIdentity(t);
	//cout << rx << ", " << ry << endl;
	matR(t, delta, rx, ry, rz);
	//printMat(t);
	MatMul(RobotR, t, RobotR);

	g_pixel.x = iXPos;
	g_pixel.y = iYPos;

	glutPostRedisplay();
}

//��Ӧ������״�ı�
void Reshape(int iWidth, int iHeight)
{
	//glutReshapeWindow(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	gl_width = iWidth;
	gl_height = iHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1.0*gl_width / gl_height, 10.0, 20000.0);

	glViewport(0, 0, iWidth, iHeight);
}


//��ʼ��
void Init()
{
	glClearColor(0.45, 0.55, 0.9, 0.0);
	glShadeModel(GL_SMOOTH);

	//glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	GLfloat light_position[4] = { 1,1,1,0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glDisable(GL_LIGHT1);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int nmat = g_model->nmaterials;

	if (nmat != 0)
	{
		//����
		texName = new GLuint[nmat];
		glGenTextures(nmat, texName);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		for (int i = 0; i < nmat; i++)
		{
			glBindTexture(GL_TEXTURE_2D, texName[i]);

			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, FreeImage_GetWidth(g_dib[i]),
				FreeImage_GetHeight(g_dib[i]),
				GL_RGB, GL_UNSIGNED_BYTE, g_textureImage[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(g_dib[i]),
				FreeImage_GetHeight(g_dib[i]), 0, GL_RGB, GL_UNSIGNED_BYTE,
				g_textureImage[i]);

			FreeImage_Unload(g_dib[i]);
			delete g_textureImage[i];
		}

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	}
	glEnable(GL_TEXTURE_2D);

}

//���ƻص�����
void Display()
{
	float t[4][4];
	SetIdentity(t);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1000, 0, 0, 0, 0, 1, 0);

	MatMul(t, t, RobotT);
	MatMul(t, t, RobotR);
	MatMul(t, t, RobotS);

	Transpose(t, t);
	glMultMatrixf(&t[0][0]);

	Draw(g_model);

	glutSwapBuffers();
}

//������
int main(int argc, char** argv)
{
	MyInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH, defSCREEN_HEIGHT);
	glutCreateWindow("�����ˣ� �첲: i   ��: k    ����: �ƶ�(w,a,s,d,q,e),����(z,x),��ת(����϶�)");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
	return 0;
}

