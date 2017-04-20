#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "bass.h"

DWORD chan;

// Recording callback - not doing anything with the data
BOOL CALLBACK DuffRecording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE; // continue recording
}


int traverse_files(const char * parent_dir,
		DIR * directory_stream_p, 
		void (*function_to_call)(const char*) )
{

	struct dirent * p ; 
	char full_path[2048];
	
	while((p = readdir(directory_stream_p)) != NULL){
		sprintf(full_path, "%s/%s", parent_dir, p->d_name);
		
		if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")){
			continue;
		}
		else if(p->d_type == DT_DIR){
			traverse_directory(full_path, function_to_call);
		}
		else{
			function_to_call(full_path);
		}
	}

}


/* Goes through a directory and calls the function provided supplying
 * it with the full path to each file.
 */

int traverse_directory(const char * path,
		void (*function_to_call)(const char*) )
{
	if(path == NULL || function_to_call == NULL){
		printf("Passed a null pointer\n");
		return 1;
	}

	DIR * dirp = opendir(path);
	int ret = 0;

	if(dirp == NULL){
		printf("Error opening directory\n");	
		ret = 1;
	}
	else {
		printf("traversing...");
		ret = traverse_files(path, dirp, function_to_call);
		closedir(dirp);
	}

	return ret;
}

void print_names(const char * path)
{
	printf("%s\n", path);
}


int init_bass(void)
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		printf("An incorrect version of BASS was loaded");
		return 0;
	}

	// initialize BASS
	
	if(!BASS_RecordInit(-1)){
		printf("Can't init recording");
		return -1;
	}

	if(!(chan=BASS_RecordStart(44100, 1, 0, DuffRecording, 0))){
		printf("Can't start recording");
		return -1;
	}

	return 1;
}


int PlayFile(const char * path)
{
	int ret = 0;
	char * file = "test.wav";		
	if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP))
		&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP,1))) {
		printf("Can't play file");
	} 
	else {
		BASS_ChannelPlay(chan,FALSE);
			ret=TRUE;
	}
	return ret;
}

int main(int argc, const char * argv[]) 
{
	if(argc != 2){
		printf("Usuage: musicpurge <path>\n");
		return 1;
	}
	if(init_bass() != 1){
		printf("Unable to initilize BASS\n");
		return 1;
	}


	traverse_directory(argv[1], print_names);

	return 0;
}


