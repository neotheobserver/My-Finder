#ifndef __MF_H__
#define __MF_H__

#define WINDOWS_MAX_PATH_CHARACTERS		256*2 //256 is max but could have more?
#define SCREEN_WIDTH	1100
#define SCREEN_HEIGHT	700
#define FONT_SIZE 		20
#define BOX_SIZE  		30
#define START_POS_X 	10
#define START_POS_Y 	10
#define PADDING 		4
#define COUNT 		((SCREEN_HEIGHT/BOX_SIZE)-2)
#define COUNT_X		((SCREEN_WIDTH/(FONT_SIZE-START_POS_X))-START_POS_X)

typedef enum app_state
{
    INIT,
    STARTED,
	LOADING,
    LOADED
} app_state_t;

typedef struct
{
	bool is_file_finder;
	bool should_exit;
	app_state_t app_state;
	char cwd[WINDOWS_MAX_PATH_CHARACTERS];
	int current_index;
	int total_count;
}mf_state_t;

#endif