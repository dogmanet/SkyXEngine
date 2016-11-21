
#pragma once

//чтение файла
size_t read_ogg(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	FILE* f = (FILE*)datasource;
	return fread(ptr, 1, size * nmemb, f);
}

//закртытие файла
int close_ogg(void* datasource)
{
	FILE* f = (FILE*) datasource;
	fclose(f);
	return 0;
}

//позиционирование
int seek_ogg(void *datasource, ogg_int64_t offset, int whence)
{
	FILE* f = (FILE*)datasource;
		switch (whence) 
		{
			case SEEK_SET: return fseek(f, offset, SEEK_SET);  break;
			case SEEK_CUR: return fseek(f, offset, SEEK_CUR);  break;
			case SEEK_END: return fseek(f, offset, SEEK_END);  break;
			default: return -1;
		}
	return 1;
}

//размер файла
long tell_ogg(void* datasource)
{
	FILE* f = (FILE*)datasource;
	return ftell(f);
}