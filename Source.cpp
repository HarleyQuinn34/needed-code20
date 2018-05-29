#include <allegro5/allegro.h>
#include <allegro5\allegro_primitives.h>
#include <stdio.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<iostream>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include <fstream>
//function declaration
bool PixelCollision(int x1, int y1, int w1, int hl, int x2, int y2, int w2, int h2);

using namespace std;

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BOUNCER_SIZE = 32;
///////////////////////////////////////////////////////////I need the score
enum MYKEYS {
	KEY_UP, KEY_DOWN
};
enum MYKEYS2 {
	KEY_W, KEY_S
};
int main() {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	ALLEGRO_BITMAP *bouncer2 = NULL;
	ALLEGRO_BITMAP *bouncer3 = NULL;
	ALLEGRO_SAMPLE *sample = NULL;
	ALLEGRO_SAMPLE *sample2 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *sample2Instance = NULL;

	float bouncer_x = SCREEN_W / 1.1 - BOUNCER_SIZE / 9.0;
	float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_dx = -4.0, bouncer_dy = 4.0;

	float bouncer2_x = SCREEN_W / 11.0 - BOUNCER_SIZE / 2.0;
	float bouncer2_y = SCREEN_H / 11.0 - BOUNCER_SIZE / 2.0;
	float bouncer2_dx = -4.0, bouncer2_dy = 4.0;

	float bouncer3_x = 200;
	float bouncer3_y = 200;
	float bouncer3_dx = -40.0, bouncer3_dy = 4.0;

	bool key[4] = { false, false, false, false };
	bool key2[4] = { false, false, false, false };
	bool redraw = true;
	bool doexit = false;
	int score = -1;
	int score2 = -1;
	al_init();
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);

	font = al_load_ttf_font("cubic.ttf", 20, 0);
	al_reserve_samples(2);
	sample2 = al_load_sample("8bit.wav");
	sample = al_load_sample("oof.wav");

	sample2Instance = al_create_sample_instance(sample2);
	al_set_sample_instance_playmode(sample2Instance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(sample2Instance, al_get_default_mixer());
	al_play_sample_instance(sample2Instance);

	sample2Instance = al_create_sample_instance(sample);
	al_set_sample_instance_playmode(sample2Instance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(sample2Instance, al_get_default_mixer());


	bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
	bouncer2 = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
	bouncer3 = al_create_bitmap(15, 15);

	al_set_target_bitmap(bouncer);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(bouncer2);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(bouncer3);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);

	int topscores[5];
	int highscores[5];
	ifstream infile;
	ifstream scoreread;
	int num;
	infile.open("save.txt");
	int temp;
	//reads in from file
	for (int i = 0; i < 5; i++)
		infile >> topscores[i];

	//print to console for testing
	for (int i = 0; i < 5; i++)
		cout << topscores[i] << endl;

	al_draw_text(font, al_map_rgb(250, 250, 250), 150, 20, ALLEGRO_ALIGN_CENTER, "Top Scores");
	//print from screen
	for (int i = 0; i < 5; i++)
		al_draw_textf(font, al_map_rgb(250, 250, 250), 500, 50 + i * 40, ALLEGRO_ALIGN_CENTER, "%d", topscores[i]);

	al_flip_display();
	al_rest(5);
	while (!doexit) {

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {//note from mo: this was in the wrong place... all physics should be *inside* the timer section
			if (bouncer3_x < 0) {
				bouncer3_dx = -bouncer3_dx;
				score++;
			}

			if (bouncer3_x > 640 - 10) {
				bouncer3_dx = -bouncer3_dx;
				score2++;
			}


			if (bouncer3_y < 0 || bouncer3_y > 480 - 10) {
				bouncer3_dy = -bouncer3_dy;
			}
			//add the x velocity to the x position, 
			//and the y velocity to the y position
			bouncer3_x += bouncer3_dx;
			bouncer3_y += bouncer3_dy;

			redraw = true;

			//note from mo: timer section was here, moved up above

			if (key2[KEY_W]) {
				bouncer2_y -= 4.0;
			}

			if (key2[KEY_S]) {
				bouncer2_y += 4.0;
			}

			if (key[KEY_UP]) {
				bouncer_y -= 4.0;
			}

			if (key[KEY_DOWN]) {
				bouncer_y += 4.0;
			}

			redraw = true;


			if (PixelCollision(bouncer3_x, bouncer3_y, 15, 15, bouncer_x, bouncer_y, 32, 100)) {
				cout << "collision!";
				al_play_sample_instance(sample2Instance);
				//al_play_sample(sample2, 2, 5.0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				bouncer3_dx = -bouncer3_dx;//reflection

										   //system("pause");

			}
			if (PixelCollision(bouncer3_x, bouncer3_y, 15, 15, bouncer2_x, bouncer2_y, 32, 100)) {
				cout << "collision!";
				//sample = true;
				al_play_sample_instance(sample2Instance);
				bouncer3_dx = -bouncer3_dx;//reflection

										   //system("pause");
			}

		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_W:
				key2[KEY_W] = true;

				break;

			case ALLEGRO_KEY_S:
				key2[KEY_S] = true;
				break;
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_W:
				key2[KEY_W] = false;
				break;

			case ALLEGRO_KEY_S:
				key2[KEY_S] = false;
				break;


			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;

			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;
			}
		}
		//render section//////////////////////////////////////////////////////////////////////////////
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));


			al_draw_filled_rectangle(bouncer_x, bouncer_y, bouncer_x + 30, bouncer_y + 100, al_map_rgb(255, 255, 255));
			al_draw_filled_rectangle(bouncer2_x, bouncer2_y, bouncer2_x + 30, bouncer2_y + 100, al_map_rgb(255, 255, 255));
			al_draw_bitmap(bouncer3, bouncer3_x, bouncer3_y, 0);
			al_draw_textf(font, al_map_rgb(250, 250, 250), 150, 50, ALLEGRO_ALIGN_CENTER, "%d", score);
			al_draw_textf(font, al_map_rgb(250, 250, 250), 500, 50, ALLEGRO_ALIGN_CENTER, "%d", score2);

			al_flip_display();
		}
	}


	ofstream write;
	write.open("save.txt");

	for (int i = 0; i < 5; i++) {
		cout << "highscore is " << topscores[i];
		if (score > topscores[i]) {
			temp = topscores[i];
			topscores[i] = score;
			score = temp;
			cout << "replacing score" << endl;
		}
	}
	//this part actually writes to the file
	for (int i = 0; i < 5; i++) {
		write << topscores[i] << endl;
	}
	al_rest(2);
	write.close();

	al_destroy_bitmap(bouncer);
	al_destroy_bitmap(bouncer2);
	al_destroy_bitmap(bouncer3);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);


	return 0;
}
//function definition
bool PixelCollision(int x1, int y1, int w1, int hl, int x2, int y2, int w2, int h2) {

	if ((x1 + w1 > x2) && (x2 + w2 > x1) && (y2 + h2 > y1) && (y1 + hl > y2))

		return true;

	else
		return false;
}