#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <raylib.h>

#include <assert.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_custom.h"

#include "mf.h"
#include "resultlist.h"
#include "music.h"

extern void update_app (mf_state_t * mf_state, char * search_text);

mf_state_t * mf_state = NULL;

pthread_t threadId = { 0 };

char search_text[64] = "MF...";
int flag_selected = 0;

static void handle_input(void);
static void * LoadDataThread(void * arg);
static char * get_location(char * string);

int main (void)
{
	mf_state = (mf_state_t * ) calloc (1, sizeof(mf_state_t));
	assert (NULL != mf_state && "Out of memory\n");

	if (NULL == mf_state) exit(1);

	InitWindow (520, 80, "My Finder");
	SetWindowState (FLAG_WINDOW_UNDECORATED); 
	SetTargetFPS (30);
    GuiLoadStyleCustom ();
    GuiSetStyle (DEFAULT, TEXT_SIZE, 30);
	SetWindowOpacity (0.95); 
	
	InitAudioDevice ();
	Music music = LoadMusicStreamFromMemory (fileType, data, mp3_len);
	PlayMusicStream (music);

	while (!mf_state->should_exit)
	{
		UpdateMusicStream (music);

		if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
			
			mf_state->should_exit = true;
		
		} 

		if (mf_state->app_state == LOADING || mf_state->app_state == LOADED ) {

			update_app(mf_state, search_text);	
		
		} else if (mf_state->app_state == INIT) {

			handle_input();
			BeginDrawing();
				ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
				GuiTextBox((Rectangle){0, 0, 400, 80 }, search_text, sizeof(search_text), true);
				GuiComboBox((Rectangle){401, 0, 119, 80}, "file;text", &flag_selected);
			EndDrawing();
		
		} else if (mf_state->app_state == STARTED) {
			
			BeginDrawing();
				ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
				DrawText("Loading.....", GetScreenWidth()/2, GetScreenHeight()/2, FONT_SIZE*2, BLACK);
			EndDrawing();
		
		}
	}
	UnloadMusicStream (music);
	CloseWindow();
	free_resources(); //free all allocated memory
	return 0;
}


static void handle_input ()
{
	if (IsKeyDown(KEY_ENTER)) {
		mf_state->app_state = STARTED;
		int error = pthread_create (&threadId, NULL, &LoadDataThread, NULL);
		if (error != 0) { 
			printf("Error creating loading thread\n");
		}
		else {
			printf("Loading thread initialized successfully\n");
		}
		
		ClearWindowState (FLAG_WINDOW_UNDECORATED);
		SetWindowState (FLAG_WINDOW_RESIZABLE);
		SetWindowSize (SCREEN_WIDTH, SCREEN_HEIGHT);
		SetWindowPosition (GetMonitorWidth (GetCurrentMonitor())/4, GetMonitorHeight (GetCurrentMonitor())/4);
    }
}

static char * get_location (char * string)
{
	char * start =  strchr(string, '/');
	if (NULL == start) return NULL;
	char * end = strchr(start, ':');
	if (NULL == end) return NULL;
	
	int length = end - start;
	char * buf = (char * ) malloc ((length) * sizeof(char));
	if (NULL == buf) return NULL;
	strncpy(buf, start+1, length-1);
	buf[length] = '\0';
	return buf;
}

static void * LoadDataThread (void * arg)
{
    FILE * out;
	char buf[512];	
	char path[1024];

	out = popen("cd", "r");
	
	if (NULL == out) {
		printf("Could not get the current directory\n");
		return NULL;
	}

	fgets (mf_state->cwd, sizeof(mf_state->cwd), out);
	mf_state->cwd[strlen(mf_state->cwd)-1] = '\0';

	pclose (out);
	
	if (flag_selected == 0) {

		mf_state->is_file_finder = true;
		snprintf(buf, sizeof(buf), "dir /b/s *%s*", search_text);
	
	} else {
		
		mf_state->is_file_finder = false;
		snprintf(buf, sizeof(buf), "findstr /spi /c:%s ./*.*", search_text);
	
	}
	
	out = popen (buf, "r");
	
	if (NULL == out) {
		printf("Could not perform the requested operation\n");
		return NULL;
	}
	int index = 0;
	
	while ((fgets(path, sizeof(path), out) != NULL) && !(mf_state->should_exit)) {
		result_t * result  = (result_t * ) malloc(sizeof(result_t));
		assert (NULL != result && "Out of memory");
		if (NULL == result) return NULL;

		int length = strlen (path);
		if (length > WINDOWS_MAX_PATH_CHARACTERS * 2) length = WINDOWS_MAX_PATH_CHARACTERS * 2;

		result->string = (char * ) malloc (length * sizeof(char));
		assert (NULL != result->string && "Out of memory");
		
		strncpy(result->string, path, length);
		result->string[length-1] = '\0'; 
		
		if (mf_state->is_file_finder)
		{

			result->location = result->string;
		
		} else {

			char * location = get_location(result->string);
			if (NULL == location) {
				free (result->string);
				result->string = NULL;
				free (result);
				result = NULL;
				continue;
			}
			length = strlen(location);	
			result->location = (char * ) malloc ((length+1) * sizeof(char));
			assert (NULL != result->location && "Out of memory");
			strncpy (result->location, location, length);
			result->location[length] = '\0';

		}
			
		result->index = index;	
		mf_state->total_count = ++index;
		add_result(result);
		if (mf_state->app_state == STARTED) {	
			mf_state->app_state = LOADING;
		}
	}
	pclose (out);
	printf ("Thread Exited\n");
	mf_state->app_state = LOADED;
    return NULL;
}
