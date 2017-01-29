
#ifndef __string_api
#define __string_api

#define def_str_validate(str) (str && str[0]!=0 && str[0]!='0')

inline bool StrValidate(const char* str)
{
	if (str && str[0] != 0 && str[0] != '0')
		return true;
	return false;
}

void StrCutMesh(const char* path, char* name)
{
	for (int i = 0; i<strlen(path); i++)
	{
		if (path[strlen(path) - i] == '\\')
		{
			if (i + 6 > 0 && i + 6 < strlen(path) &&
				(path[(strlen(path) - i) - 1] == 's' || path[(strlen(path) - i) - 1] == 'S') &&
				(path[(strlen(path) - i) - 2] == 'e' || path[(strlen(path) - i) - 2] == 'E') &&
				(path[(strlen(path) - i) - 3] == 'h' || path[(strlen(path) - i) - 3] == 'H') &&
				(path[(strlen(path) - i) - 4] == 's' || path[(strlen(path) - i) - 4] == 'S') &&
				(path[(strlen(path) - i) - 5] == 'e' || path[(strlen(path) - i) - 5] == 'E') &&
				(path[(strlen(path) - i) - 6] == 'm' || path[(strlen(path) - i) - 6] == 'M')
				)
			{
				memcpy(name, path + (strlen(path) - i) + 1, i - 1);
				name[(i - 1)] = 0;
				break;
			}
		}
	}
}

void StrCutSound(const char* path, char* name)
{
	for (int i = 0; i<strlen(path); i++)
	{
		if (path[strlen(path) - i] == '\\')
		{
			if (i + 6 > 0 && i + 6 < strlen(path) &&
				(path[(strlen(path) - i) - 1] == 's' || path[(strlen(path) - i) - 1] == 'S') &&
				(path[(strlen(path) - i) - 2] == 'd' || path[(strlen(path) - i) - 2] == 'D') &&
				(path[(strlen(path) - i) - 3] == 'n' || path[(strlen(path) - i) - 3] == 'N') &&
				(path[(strlen(path) - i) - 4] == 'u' || path[(strlen(path) - i) - 4] == 'U') &&
				(path[(strlen(path) - i) - 5] == 'o' || path[(strlen(path) - i) - 5] == 'O') &&
				(path[(strlen(path) - i) - 6] == 's' || path[(strlen(path) - i) - 6] == 'S')
				)
			{
				memcpy(name, path + (strlen(path) - i) + 1, i - 1);
				name[(i - 1)] = 0;
				break;
			}
		}
	}
}

void StrCutName(const char* path, char* name)
{
	name[0] = 0;
	for (int i = 0; i<strlen(path); i++)
	{
		if (path[strlen(path) - i] == '\\')
		{
			memcpy(name, path + (strlen(path) - i) + 1, i - 1);
			name[(i - 1)] = 0;
			break;
		}
	}

	if (name[0] == 0)
	{
		sprintf(name, "%s", path);
	}
}

void StrCutNameNEx(const char* path, char* name)
{
	name[0] = 0;
	int pointpoint = 0;
	for (int i = 0; i<strlen(path); i++)
	{
		if (path[strlen(path) - i] == '\\')
		{
			memcpy(name, path + (strlen(path) - i) + 1, (i - 1) - pointpoint);
			name[(i - 1) - pointpoint] = 0;
			break;
		}
		else if (path[strlen(path) - i] == '.')
			pointpoint = i;
	}

	if (name[0] == 0)
	{
		sprintf(name, "%s", path);
	}
}

#endif