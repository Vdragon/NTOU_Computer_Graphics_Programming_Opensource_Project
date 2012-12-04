/*
main.cpp
	請見本檔案的header檔案以得到更多關於本檔案的說明
*/
/*||||| 程式所include之函式庫的標頭檔 | Included Library Headers |||||*/
/* Standard C++ Library Definitions */
	#include <cstdio>
	#include <iostream>
		/* for I/O functions */
	#include <cstdlib>
		/* for EXIT_* constant */

/* GLUT Library Definitions */
#ifdef _WIN32
    #include <windows.h>
        /* glut.h depends on windows.h on Windows platform */
#endif
	#include <GL/glut.h>

/* Vdragon's C Libraries */
	#include "Show_software_info/Show_software_info.h"
	#include "pauseProgram/Pause_program.h"
	#include "Project_specific_configurations/Software_info.h"
	#include "Messages_templates/zh_TW.h"

/*||||| 常數與巨集 | Constants & Macros |||||*/
	/* application window size */
		#define WINDOW_WIDTH 600
		#define WINDOW_HEIGHT 360

	/* message of cbMouse() */
		#define MESSAGE_MOUSE_BUTTON_PRESSED "按下了"
		#define MESSAGE_MOUSE_BUTTON_RELEASED "放開了"
		#define MESSAGE_MOUSE_BUTTON_LEFT "滑鼠左鍵"
		#define MESSAGE_MOUSE_BUTTON_RIGHT "滑鼠右鍵"
		#define MESSAGE_MOUSE_BUTTON_MIDDLE "滑鼠中鍵"
		#define MESSAGE_MOUSE_BUTTON_UNKNOWN "未知的滑鼠按鈕"

	/* message of wide width characters */
		#define CJK_CHINESE_PERIOD "。"

/*||||| Definition of data type, enumeration, data structure and class |||||*/
	/* 選操作選單的項目 */
		enum menu_selectOperation{
			SELECTOPERATION_EXIT,
		};

		enum menu_selectDrawMode{
			DRAW_POINT, DRAW_LINE, DRAW_SQUARE, DRAW_POLYGON
		};

/*||||| 函式雛型 | Function Prototypes |||||*/
/* callback functions */
	void cbKeyboard(unsigned char key, int mouse_x, int mouse_y);
		/* callback function of keyboard events */
	void cbMouse(int button, int state, int x, int y);
		/* 滑鼠事件的 callback 函式 */
	void cbMouseMotion(int x, int y);
		/* 滑鼠拖曳事件的 callback 函式 */
	void cbDisplay(void);
		/* 視窗顯示事件的 callback 函式 */
	void cbReshape(int new_width, int new_height);
		/* 視窗變形事件的 callback 函式 */
	void cbMenuMain(int selection);
	void cbMenuSelectOperation(int selection);
	void cbMenuSelectDrawMode(int selection);

/* draw functions */
	void drawPoint3f(int x, int y, float red, float green, float blue, float size);
		/* procedure to draw a point in specified color, position, and size */
	void drawSqare3f(int x, int y, float red, float green, float blue, unsigned edge_length);
		/* function to draw a square in specified color, position, and size */

/*||||| 全域變數 | Global Variables |||||*/
	/* Current window width and height */
		int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;
	/* current draw parameters */
		int draw_mode = DRAW_POINT;
		int draw_size = 10;
	/* WORKAROUND : Mouse callback function will still be called with "button released" when the clicked object is actually a menu, workaround this problem by checking if the mouse release event is actually just after menu callback. */
		char workaround_menu_clicked = 'n';

/*||||| 主要程式碼 | Main Code |||||*/
int main(int argc, char *argv[]){

	int handle_submenu_selectOperation, handle_submenu_selectDrawMode;
		/* a handle of submenus */

	show_software_info(__FILE__);
/* restart_program: */

	/* GLUT 初始化？ */
	glutInit(&argc, argv);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow(SOFTWARE_NAME);
		/* 因為我們沒有要建立多個視窗，我們不需要使用此函式傳回的視窗(window)ID */

	/* Register callback functions */
		glutKeyboardFunc(cbKeyboard);
		glutMouseFunc(cbMouse);
		glutMotionFunc(cbMouseMotion);
		glutDisplayFunc(cbDisplay);
		glutReshapeFunc(cbReshape);

	/* 註冊選單與選單的 callback 函式 */
		/* 註冊一個選單的 callback 函式並獲取一個 menu handle */
			handle_submenu_selectOperation = glutCreateMenu(cbMenuSelectOperation);
				/* 新增選單中的項目 */
					glutAddMenuEntry("Exit program", SELECTOPERATION_EXIT);
		handle_submenu_selectDrawMode = glutCreateMenu(cbMenuSelectDrawMode);
			glutAddMenuEntry("Point", DRAW_POINT);
			glutAddMenuEntry("Line", DRAW_LINE);
			glutAddMenuEntry("Square", DRAW_SQUARE);
			glutAddMenuEntry("Polygon", DRAW_POLYGON);

		/* 註冊主選單 */
			glutCreateMenu(cbMenuMain);
		/* 新增子選單於主選單上 */
			glutAddSubMenu("Select operation", handle_submenu_selectOperation);
			glutAddSubMenu("Select draw mode", handle_submenu_selectDrawMode);
		/* 將此選單與滑鼠中鍵綁定 */
			glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Enter main loop */
		std::cout << "請於 OpenGL 視窗於作用中狀態時按 q 鍵離開程式。" << std::endl;
		glutMainLoop();

	/* 暫停程式運行
	 * 　由於在 GLUT 程式中使用 exit() 結束所以不會執行到...
	if(pauseProgram() == 1){
		goto restart_program;
	} */
	return EXIT_SUCCESS;
}

void cbKeyboard(unsigned char key, int mouse_x, int mouse_y){
#ifndef NDEBUG
	printf(DEBUG_TAG "您於滑鼠位在(%d, %d)座標時按下了 keycode 為 '%c' 的按鍵。\n", mouse_x, mouse_y, key);
#endif
	/* 偵測結束程式運行的觸發按鍵 */
		if(key == 'q' || key == 'Q'){
			std::cout << "程式結束運行。" << std::endl;
			exit(EXIT_SUCCESS);
		}

	return;
}

void cbMouse(int button, int state, int x, int y){
	/* workaround : avoid mouse event when menu event just happened */
		if(workaround_menu_clicked == 'y'){
			workaround_menu_clicked = 'n';
			return;
		}

#ifndef NDEBUG
	std::cout << DEBUG_TAG "您於(" << x << ',' <<  y << ")位置"
		<< (state == GLUT_DOWN ? MESSAGE_MOUSE_BUTTON_PRESSED : MESSAGE_MOUSE_BUTTON_RELEASED);
#endif

	switch(button){
	case GLUT_LEFT_BUTTON:
#ifndef NDEBUG
		std::cout << MESSAGE_MOUSE_BUTTON_LEFT << CJK_CHINESE_PERIOD << std::endl;
#endif
		switch(draw_mode){
		case DRAW_POINT:
			drawPoint3f(x, y, 1, 0, 0, draw_size);
			break;
		case DRAW_LINE:

			break;
		case DRAW_SQUARE:
			drawSqare3f(x, y, 0, 0, 1, draw_size);
			break;
		case DRAW_POLYGON:
			glBegin(GL_POLYGON);
				glColor3f(1,0,1);
				glVertex2f(x, window_height - y);
				glColor3f(1,1,1);
				glVertex2f(x + 10, window_height - y -10);
				glColor3f(0,1,1);
				glVertex2f(x - 10, window_height - y -10);
			glEnd();
			glFlush();
		default:
			break;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
#ifndef NDEBUG
		std::cout << MESSAGE_MOUSE_BUTTON_MIDDLE << CJK_CHINESE_PERIOD << std::endl;
#endif
		break;
	case GLUT_RIGHT_BUTTON:
#ifndef NDEBUG
		std::cout << MESSAGE_MOUSE_BUTTON_RIGHT << CJK_CHINESE_PERIOD << std::endl;
#endif
		break;
	default:
#ifndef NDEBUG
		std::cout << MESSAGE_MOUSE_BUTTON_UNKNOWN << CJK_CHINESE_PERIOD << std::endl;
#endif
		break;
	}
	return;
}

void cbMouseMotion(int x, int y){
#ifndef NDEBUG
	std::cout << DEBUG_TAG "您正在於(" << x << ',' << y << ")進行滑鼠拖曳操作。" << std::endl;
#endif

	return;
}

void cbDisplay(void){
#ifndef NDEBUG
	std::cout << DEBUG_TAG "cbDisplay()被呼叫。" << std::endl;
		/* TODO : 弄懂何時會呼叫此 callback 函式
		 * 　目前視窗的露出並不會造成此函式的 callback 與預期不符。 */
#endif

	/* 將背景 buffer 的顏色設為黑色 */
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

	/* 將背景 buffer 置換出去
		glutSwapBuffers(); */

	return;
}

void cbReshape(int new_width, int new_height){
#ifndef NDEBUG
	std::cout << DEBUG_TAG "cbReshape()被呼叫。" << std::endl;
#endif
	/* set current width/height to global variable */
		window_width = new_width;
		window_height = new_height;

	/* TODO : 這裡有一些還沒看懂的範例函式呼叫 */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, window_width, 0.0, window_height, -1.0, 1.0);
		glViewport(0,0,window_width,window_height);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	/* 畫面被更動了，讓 display callback 函式再被呼叫 */
		glutPostRedisplay();

	return;
}

void cbMenuMain(int selection){
	/* do nothing now due to no clickable events */

	/* workaround */
	workaround_menu_clicked = 'y';
	return;
}

void cbMenuSelectOperation(int selection){
#ifndef NDEBUG
	std::cout << DEBUG_TAG "您選擇了：";
#endif
	switch(selection){
	case SELECTOPERATION_EXIT:
#ifndef NDEBUG
		std::cout << "結束程式";
#endif
		exit(EXIT_SUCCESS);
		break;
	default:
#ifndef NDEBUG
		std::cout << "未知的操作";
#endif
		break;
	}
#ifndef NDEBUG
	std::cout << "。" << std::endl;
#endif
	/* workaround */
	workaround_menu_clicked = 'y';
	return;
}

void cbMenuSelectDrawMode(int selection){
	draw_mode = selection;
	/* workaround */
	workaround_menu_clicked = 'y';
	return;
}

void drawPoint3f(int x, int y, float red, float green, float blue, float size){
	glColor3f(red, green, blue);
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex2i(x, window_height - y);
	glEnd();
	glFlush();
	return;
}

void drawSqare3f(int x, int y, float red, float green, float blue, unsigned edge_length){
	glColor3f(red, green, blue);
	glBegin(GL_POLYGON);
		glVertex2i(x - edge_length / 2, window_height - (y - edge_length / 2));
		glVertex2i(x + edge_length / 2, window_height - (y - edge_length / 2));
		glVertex2i(x + edge_length / 2, window_height - (y + edge_length / 2));
		glVertex2i(x - edge_length / 2, window_height - (y + edge_length / 2));
	glEnd();
	glFlush();
	return;
}

