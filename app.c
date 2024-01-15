#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>

#include "raygui.h"

#include "mf.h"
#include "resultlist.h"

static void handle_input(mf_state_t * mf_state);

int listViewExScrollIndex = 0;
int listViewExActive = 2;
int listViewExFocus = -1;

void update_app (mf_state_t * mf_state, char * search_text)
{
	char buf[WINDOWS_MAX_PATH_CHARACTERS+16];
    handle_input (mf_state);
    BeginDrawing ();
		ClearBackground (GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
	
		snprintf (buf, sizeof(buf), "Start Dir: %s", mf_state->cwd);
		GuiLabel ((Rectangle){0, 0, GetScreenWidth(), 40}, buf);
		snprintf (buf, sizeof(buf), "Search Tex: %s", search_text);
		GuiLabel ((Rectangle){0, 50, GetScreenWidth(), 40}, buf);
		
		GuiSetStyle (LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
		snprintf (buf, sizeof(buf), "Total Found: %d", mf_state->total_count);
		GuiLabel ((Rectangle){0, 50, GetScreenWidth(), 40}, buf);
		GuiSetStyle (LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
		
        GuiListViewCustom ((Rectangle){ 0, 100, GetScreenWidth(), GetScreenHeight()-100 }, mf_state->total_count, &listViewExScrollIndex, &listViewExActive, &listViewExFocus);
    EndDrawing ();
}

static void handle_input (mf_state_t * mf_state)
{
	if (IsKeyPressed (KEY_ENTER)) {
        result_t * result = find_result (listViewExActive);
        if (NULL != result) {
            char buf[WINDOWS_MAX_PATH_CHARACTERS];
            if (mf_state->is_file_finder) {

                snprintf (buf, sizeof(buf), "explorer \"%s\"", result->location);
				printf ("File: %s\n",buf);
                system (buf);

            } else {

                snprintf (buf, sizeof(buf), "explorer \"%s\\%s\"",mf_state->cwd, result->location);
				printf ("Text: %s\n",buf);
                system (buf);

            }
        }	     
    }
	
	if (IsKeyPressed (KEY_UP) || IsKeyPressed (KEY_K)) {
		if (listViewExActive <= 0) {
			listViewExActive = mf_state->total_count-1;
		} else {
			listViewExActive--;
		}
	}

	if (IsKeyPressed (KEY_DOWN) || IsKeyPressed (KEY_J)) {
		if (listViewExActive >= mf_state->total_count-1) {
			listViewExActive = 0;
		}
		else {
			listViewExActive++;
		}
	}

	if (IsKeyPressed (KEY_M)) {
		if (listViewExActive+COUNT >  mf_state->total_count-1)
		{
			listViewExActive = mf_state->total_count-1;
		}
		else {
			listViewExActive += COUNT;
		}
	}

	if (IsKeyPressed (KEY_N)) {
		if (listViewExActive-COUNT < 0) {
			listViewExActive  = 0;
		}
		else {
			listViewExActive -= COUNT;
		}
	}
}
