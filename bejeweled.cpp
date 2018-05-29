#include <stdio.h>
#include <allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include<iostream>
#include<ctime>
using namespace std;
const int PACSIZE = 20;
const float FPS = 60;
const int SCREEN_W = 500;
const int SCREEN_H = 500;
const int BOUNCER_SIZE = 60;

bool CheckForMatches(int matrix[20][20]);
void MoveDown(int matrix[20][20]);
void Replenish(int matrix[20][20]);


int main(int argc, char **argv) {
	srand(time(NULL));
	
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	ALLEGRO_BITMAP *bouncer2 = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP*wall = NULL;
	//ALLEGRO_SAMPLE *sample = NULL;
	ALLEGRO_BITMAP*dots = NULL;
	
	

	cout << "FLEGIN" << endl;
	float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_dx = -4.0, bouncer_dy = 4.0;

	float bouncer2_x = SCREEN_W / 4.0 - BOUNCER_SIZE / 2.0;
	float bouncer2_y = SCREEN_H / 4.0 - BOUNCER_SIZE / 2.0;
	float bouncer2_dx = -2.0, bouncer2_dy = 4.0;
	cout << "FLEGIN2" << endl;

	bool redraw = true;
	bool doexit = false;
	al_init();
	al_init_primitives_addon();
	al_install_mouse();

	int boxX = 0;
	int boxY = 0;
	bool clicked = false;
	//these variables handle the mouse selection of grid slots
	int highlightX = 0;
	int highlightY = 0;
	bool highlight = false;
	int leftovers = 0;
	bool snapped = false;

	int highlight2X = 0;
	int highlight2Y = 0;
	bool highlight2 = false;
	int leftovers2 = 0;
	bool snapped2 = false;
	int temp;
	int board[20][20];
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = rand() % 4 + 1; //populate each slot with a #1-4
//check for and erase matches, move spots down, replenish
//repeat until no more matches found
	while (CheckForMatches(board)) {
		MoveDown(board);
		Replenish(board);
	}

	//print board for testing
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++)
			cout << board[i][j];
		cout << endl;
	}
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);
	//sample = al_load_sample("oof.wav");
	cout << "FLEGIN3" << endl;



	dots = al_create_bitmap(5, 5);
	al_set_target_bitmap(dots);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	//////////////////////
	wall = al_create_bitmap(30, 30);////////////////////wall size
	al_set_target_bitmap(wall);
	al_clear_to_color(al_map_rgb(128, 0, 0));
	/////////////////////////////////
	bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
	bouncer2 = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
	cout << "FLEGIN4" << endl;

	al_set_target_bitmap(bouncer);
	al_clear_to_color(al_map_rgb(255, 255, 0));
	al_set_target_bitmap(bouncer2);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_set_target_bitmap(al_get_backbuffer(display));


	event_queue = al_create_event_queue();
	cout << "FLEGIN5" << endl;

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);
	cout << "FLEGIN6" << endl;

	int level[10][10];
	for (int i = 0; i < 10; i++) {//rows
		for (int j = 0; j < 10; j++) {//collumns

			level[i][j] = rand() % 4 + 1;//print out each grid slit
		}
	}

		//prints to console
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				cout << level[i][j] <<" ";
			}
			cout << endl;
		}
		//draw to game wwindow
		
		while (1)
		{
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);

			if (ev.type == ALLEGRO_EVENT_TIMER) {
				//snap the highlighted box to grid
				if (!snapped) {
					leftovers = highlightX % 40;
					highlightX -= leftovers;
					leftovers = highlightY % 40;
					highlightY -= leftovers;
					snapped = true;
				}
				if (!snapped2) {
					leftovers2 = highlight2X % 40;
					highlight2X -= leftovers2;
					leftovers2 = highlight2Y % 40;
					highlight2Y -= leftovers2;
					snapped = true;
				}
				redraw = true;
			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				break;
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
				ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {

				bouncer_x = ev.mouse.x;
				bouncer_y = ev.mouse.y;
				
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				clicked = true;
				boxX = ev.mouse.x;
				boxY = ev.mouse.y;
				boxX -= boxX % 50;
				boxY -= boxY % 50;
				//if you already have a slot selected, and you're clicking next to it, select a second box
				if (highlight == true && ev.mouse.x > highlightX - 40 && ev.mouse.x < highlightX + 80 && ev.mouse.y > highlightY - 40 && ev.mouse.y < highlightY + 80) {
					//cout << "selected next slot!";
					highlight2 = true;
					highlight2X = ev.mouse.x;
					highlight2Y = ev.mouse.y;
					snapped2 = false;
					//swap the two boxes 
					temp = board[highlight2Y / 40][highlight2X / 40];
					board[highlight2Y / 40][highlight2X / 40] = board[highlightY / 40][highlightX / 40];
					board[highlightY / 40][highlightX / 40] = temp;
					while (CheckForMatches(board)) {
						MoveDown(board);
						Replenish(board);//may be premature
						highlight = false;
						highlight2 = false;
					}
				}
				//otherwise, select a new box
				else
				{
					highlight = true;
					highlight2 = false;
					highlightX = ev.mouse.x;
					highlightY = ev.mouse.y;
					snapped = false;
			}
				//render section
			if (redraw && al_is_event_queue_empty(event_queue)) {
				redraw = false;

				al_clear_to_color(al_map_rgb(0, 0, 0));



				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						if (level[i][j] == 1)
							al_draw_filled_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(128, 0, 0));
						else if (level[i][j] == 2)
							al_draw_filled_circle(i * 50 + 25, j * 50 + 25, 25, al_map_rgb(0, 155, 255));
						else if (level[i][j] == 3)
							al_draw_filled_triangle(i * 50, j * 50, i * 50, j * 50 + 50, i * 50 + 50, j * 50, al_map_rgb(0, 255, 0));
						else if (level[i][j] == 4)
							al_draw_rectangle(i * 50, j * 50, i * 50 + 45, j * 50 + 45, al_map_rgb(240, 246, 155), 5);
					}
					if (clicked==true)
					al_draw_rectangle(boxX, boxY, boxX + 50, boxY + 50, al_map_rgb(0, 0, 255),5);
				}

				if (highlight) {
					al_draw_rectangle(highlightX, highlightY, highlightX + 40, highlightY + 40, al_map_rgb(255, 255, 255), 5);
				}
				if (highlight2) {
					al_draw_rectangle(highlight2X, highlight2Y, highlight2X + 40, highlight2Y + 40, al_map_rgb(0, 255, 255), 5);
				}
				al_flip_display();

				cout << "checking for matches..." << endl;
				system("pause");
				CheckForMatches(board);
				al_flip_display();
				system("pause");


				

				cout << "moving columns down" << endl;
				system("pause");
				MoveDown(board);
				al_flip_display();

				cout << "replenishing board" << endl;
				system("pause");
				Replenish(board);
				al_flip_display();
				
			}
		}
		
		al_destroy_bitmap(bouncer);
		al_destroy_timer(timer);
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		system("pause");
		




		

	return 0;
}
//checks for matches of 3
//then, goes through and changes everything in that row/column that are 3+ matches into 0s
bool CheckForMatches(int matrix[20][20]) {
	int count = 1;
	bool foundMatch = false;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if ((matrix[i][j] == matrix[i][j + 1]) && (matrix[i][j] == matrix[i][j + 2])) {
				//cout << "horizontal match found at " << i << " , " << j << endl;
				foundMatch = true;
				while (matrix[i][j] == matrix[i][j + count]) {
					matrix[i][j + count] = 0;
					count++;
				}
				matrix[i][j] = 0;
				count = 1;
			}
		}
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if ((matrix[i][j] == matrix[i + 1][j]) && (matrix[i][j] == matrix[i + 2][j])) {
				//cout << "vertical match found at " << i << " , " << j << endl;
				foundMatch = true;
				while (matrix[i][j] == matrix[i + count][j]) {
					matrix[i + count][j] = 0;
					count++;
				}
				matrix[i][j] = 0;
				count = 1;
			}
		}
	}
	return foundMatch;
}

void MoveDown(int matrix[20][20]) {
	int counter = 1;
	for (int i = 19; i > 0; i--) {
		for (int j = 0; j < 20; j++) {
			if (matrix[i][j] == 0) {
				//counts how many spaces we're going to move the 0 down
				while (matrix[i - counter][j] == 0 && i - counter > 0) {
					counter++;
				}
				//move the slot above my current slot down
				matrix[i][j] = matrix[i - counter][j];
				//moves the 0 that used to be there up
				matrix[i - counter][j] = 0;
				counter = 1;
				//cout << "moving down" << endl;
			}
		}
	}
}

void Replenish(int matrix[20][20]) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (matrix[i][j] == 0) {
				matrix[i][j] = rand() % 4 + 1;
			}
		}
	}
}