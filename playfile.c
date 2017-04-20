
	BOOL ret=FALSE;
	regex_t fregex;
	GtkWidget *filesel; // file selector
	GtkFileFilter *filter;
	filesel=gtk_file_chooser_dialog_new("Open File",GTK_WINDOW(win),GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
	filter=gtk_file_filter_new();
	gtk_file_filter_set_name(filter,"Playable files");
	regcomp(&fregex,"\\.(mo3|xm|mod|s3m|it|umx|mp[1-3]|ogg|wav|aif)$",REG_ICASE|REG_NOSUB|REG_EXTENDED);
	gtk_file_filter_add_custom(filter,GTK_FILE_FILTER_FILENAME,FileExtensionFilter,&fregex,NULL);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filesel),filter);
	filter=gtk_file_filter_new();
	gtk_file_filter_set_name(filter,"All files");
	gtk_file_filter_add_pattern(filter,"*");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filesel),filter);
//	if (gtk_dialog_run(GTK_DIALOG(filesel))==GTK_RESPONSE_ACCEPT) {
		//char *file=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filesel));
		char * file = "test.wav";		
//		gtk_widget_hide(filesel);
		if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP))
			&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP,1))) {
			Error("Can't play file");
		} else {
			BASS_ChannelPlay(chan,FALSE);
			ret=TRUE;
		}
		//g_free(file);
//	}
	gtk_widget_destroy(filesel);
	return ret;

