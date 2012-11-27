/*
Testing_basic_operations_and_callback_functions.cpp
	請見本檔案的header檔案以得到更多關於本檔案的說明
*/
/*||||| 程式所include之函式庫的標頭檔 | Included Library Headers |||||*/
/* Standard C++ Library Definitions */
	#include <cstdio>
	#include <iostream>
		/* for I/O functions */
	#include <cstdlib>
		/* for EXIT_* macro */

/* GLUT Library Definitions */
	#include <GL/glut.h>

/* Vdragon's C Libraries */
	#include "Show_software_info/Show_software_info.h"
	#include "pauseProgram/Pause_program.h"
	#include "Project_specific_configurations/Software_info.h"
	#include "Messages_templates/zh_TW.h"

/*||||| 常數與巨集 | Constants & Macros |||||*/
	#define MESSAGE_MOUSE_BUTTON_PRESSED "按下了"
	#define MESSAGE_MOUSE_BUTTON_RELEASED "放開了"
	#define MESSAGE_MOUSE_BUTTON_LEFT "滑鼠左鍵"
	#define MESSAGE_MOUSE_BUTTON_RIGHT "滑鼠右鍵"
	#define MESSAGE_MOUSE_BUTTON_MIDDLE "滑鼠中鍵"
	#define MESSAGE_MOUSE_BUTTON_UNKNOWN "未知的滑鼠按鈕"

/*||||| Definition of data type, enumeration, data structure and class |||||*/
	/* 選色選單的項目 */
		typedef enum menu_selectColor{
			SELECTCOLOR_WHITE,
			SELECTCOLOR_RED,
			SELECTCOLOR_GREEN,
			SELECTCOLOR_BLUE
		}Menu_selectColor;

	/* 選操作選單的項目 */
		typedef enum menu_selectOperation{
			SELECTOPERATION_EXIT,
		}Menu_selectOperation;

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
	void cbMenuSelectColor(int selection);
	void cbMenuSelectOperation(int selection);

/*||||| 全域變數 | Global Variables |||||*/
	int window_current_width = 600, window_current_height = 360;
		/* current width and height of window */

/*||||| 主要程式碼 | Main Code |||||*/
int main(int argc, char *argv[]){
	int handle_submenu_selectColor, handle_submenu_selectOperation;
		/* a handle of submenus */

	show_software_info(__FILE__);
/* restart_program: */

	/* GLUT 初始化？ */
	glutInit(&argc, argv);
		/* make a session with the window system */
	glutInitWindowPosition(0, 0);
		/* set window position */
	glutInitWindowSize(window_current_width, window_current_height);
		/* set window size */
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
		/* set display mode */
	glutCreateWindow(SOFTWARE_NAME);
		/* create a window
		   因為我們沒有要建立多個視窗，我們不需要使用此函式傳回的視窗(window)ID */

	/* Register callback functions */
		glutKeyboardFunc(cbKeyboard);
		glutMouseFunc(cbMouse);
		glutMotionFunc(cbMouseMotion);
		glutDisplayFunc(cbDisplay);
		glutReshapeFunc(cbReshape);

	/* 註冊選單與選單的 callback 函式 */
		/* 註冊一個選單的 callback 函式並獲取一個 menu handle */
			handle_submenu_selectColor = glutCreateMenu(cbMenuSelectColor);
		/* 新增選單中的項目
		 * 　FIXME
		 * 　　* 選單無法正確的顯示雙倍寬度字元（這很悲劇） */
			glutAddMenuEntry("白色 | white", SELECTCOLOR_WHITE);
			glutAddMenuEntry("紅色 | red", SELECTCOLOR_RED);
			glutAddMenuEntry("綠色 | green", SELECTCOLOR_GREEN);
			glutAddMenuEntry("藍色 | blue", SELECTCOLOR_BLUE);

		/* 註冊一個選單的 callback 函式並獲取一個 menu handle */
			handle_submenu_selectOperation = glutCreateMenu(cbMenuSelectOperation);
		/* 新增選單中的項目 */
			glutAddMenuEntry("結束程式 | Exit program", SELECTOPERATION_EXIT);

		/* 註冊主選單 */
			glutCreateMenu(cbMenuMain);
		/* 新增子選單於主選單上 */
			glutAddSubMenu("選取顏色 | Select color", handle_submenu_selectColor);
			glutAddSubMenu("選取操作 | Select operation", handle_submenu_selectOperation);
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
	printf(DEBUG_TAG "您於滑鼠位在(%d, %d)座標時按下了 keycode 為 '%c' 的按鍵。\n", mouse_x, mouse_y, key);

	/* 偵測結束程式運行的觸發按鍵 */
		if(key == 'q' || key == 'Q'){
			std::cout << "程式結束運行。" << std::endl;
			exit(EXIT_SUCCESS);
		}

	return;
}

void cbMouse(int button, int state, int x, int y){
	std::cout << DEBUG_TAG "您於(" << x << ',' <<  y << ")位置"
		<< (state == GLUT_DOWN ? MESSAGE_MOUSE_BUTTON_PRESSED : MESSAGE_MOUSE_BUTTON_RELEASED);
	switch(button){
	case GLUT_LEFT_BUTTON:
		std::cout << MESSAGE_MOUSE_BUTTON_LEFT;
		break;
	case GLUT_MIDDLE_BUTTON:
		std::cout << MESSAGE_MOUSE_BUTTON_MIDDLE;
		break;
	case GLUT_RIGHT_BUTTON:
		std::cout << MESSAGE_MOUSE_BUTTON_RIGHT;
		break;
	default:
		std::cout << MESSAGE_MOUSE_BUTTON_UNKNOWN;
		break;
	}
	std::cout << "。" << std::endl;

	return;
}

void cbMouseMotion(int x, int y){
	std::cout << DEBUG_TAG "您正在於(" << x << ',' << y << ")進行滑鼠拖曳操作。" << std::endl;

	return;
}

void cbDisplay(void){
	std::cout << DEBUG_TAG "cbDisplay()被呼叫。" << std::endl;
		/* TODO : 弄懂何時會呼叫此 callback 函式
		 * 　目前視窗的露出並不會造成此函式的 callback 與預期不符。 */

	/* 將背景 buffer 的顏色設為黑色 */
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

	/* 將背景 buffer 置換出去 */
		glutSwapBuffers();

	return;
}

void cbReshape(int new_width, int new_height){
	std::cout << DEBUG_TAG "cbReshape()被呼叫。" << std::endl;
	/* set current width/height to global variable */
		window_current_height = new_height;
		window_current_width = new_width;

	/* 畫面被更動了，讓 display callback 函式再被呼叫 */
		glutPostRedisplay();

	return;
}

void cbMenuMain(int selection){
	/* do nothing now due to no clickable events */

	return;
}

void cbMenuSelectColor(int selection){
	std::cout << DEBUG_TAG "您選擇了：";
	switch(selection){
	case SELECTCOLOR_WHITE:
		std::cout << "白色";
		break;
	case SELECTCOLOR_RED:
		std::cout << "紅色";
		break;
	case SELECTCOLOR_GREEN:
		std::cout << "綠色";
		break;
	case SELECTCOLOR_BLUE:
		std::cout << "藍色";
		break;
	default:
		std::cout << "未知的顏色";
	}

	std::cout << "。" << std::endl;

	return;
}

void cbMenuSelectOperation(int selection){
	std::cout << DEBUG_TAG "您選擇了：";
	switch(selection){
	case SELECTOPERATION_EXIT:
		std::cout << "結束程式";
		exit(EXIT_SUCCESS);
		break;
	default:
		std::cout << "未知的操作";
	}
	std::cout << "。" << std::endl;

	return;
}
