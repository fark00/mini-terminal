#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include<direct.h>
#pragma warning(disable:4996)
struct user
{
	char name[50];
	char username[50];
	char password[101];
	int power = 0;
	char powwer[4];
	char date[11];
	char access_level[2];
	int access__level;
	int wrong__orders = 0;
	char wrong_orders[4];
	struct user * next;
};
struct file
{
	char filename[20];
	char filemaker[20];
	char construction_time[50];
	char lastseen[50];
	char fileaddress[_MAX_DIR];
	char file_access_level[2];
	struct file* nextfile;
};
struct dir
{
	char dirname[20];
	char dirmaker[20];
	char diraddress[_MAX_DIR];
	char dir_access_level[2];
	struct dir *nextdir;
};
FILE *fileptr;
char today[11];
struct user *user_ptr, *head, *tail, *updating_ptr;
struct file *file_ptr, *file_temp, *file_head = NULL, *file_tail = NULL;
struct dir *dir_ptr, *dir_temp, *dir_head = NULL, *dir_tail = NULL;
char x[100], y[100], root_address[160];
void SetColor(int ForgC)
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}
void wrong_orders(void)
{
	if (user_ptr->wrong__orders < 10)
	{
		user_ptr->wrong_orders[0] = (user_ptr->wrong__orders + 48);
		user_ptr->wrong_orders[1] = '\0';
	}
	else if (user_ptr->wrong__orders < 100)
	{
		user_ptr->wrong_orders[0] = ((user_ptr->wrong__orders) / 10 + 48);
		user_ptr->wrong_orders[1] = ((user_ptr->wrong__orders) % 10 + 48);
		user_ptr->wrong_orders[2] = '\0';
	}
	else if (user_ptr->wrong__orders < 1000)
	{
		user_ptr->wrong_orders[0] = ((user_ptr->wrong__orders) / 100 + 48);
		user_ptr->wrong_orders[1] = (((user_ptr->wrong__orders) / 10) % 10 + 48);
		user_ptr->wrong_orders[2] = ((user_ptr->wrong__orders) % 10 + 48);
		user_ptr->wrong_orders[3] = '\0';
	}
}
int chartoint(char* ch)
{
	int x = strlen(ch);
	int y = x - 1;
	int integer = 0;
	for (int i = 0; i < x; i++)
	{
		integer += (ch[i] - '0')*(int)pow((double)10, (y));
		y--;
	}
	return integer;
}
int review_deadline(char *str)
{
	char today_[11];
	strcpy(today_, today);
	int review = 0;
	char *year_t, *month_t, *day_t;
	year_t = strtok(today_, "/");
	int y_t = chartoint(year_t);
	month_t = strtok(NULL, "/");
	int m_t = chartoint(month_t);
	day_t = strtok(NULL, "/");
	int d_t = chartoint(day_t);

	char t[11];
	strcpy(t, user_ptr->date);
	char *year_d, *month_d, *day_d;
	year_d = strtok(t, "/");
	int y_d = chartoint(year_d);
	month_d = strtok(NULL, "/");
	int m_d = chartoint(month_d);
	day_d = strtok(NULL, "/");
	int d_d = chartoint(day_d);

	if (y_t < y_d)
		review = 1;
	else if (y_d == y_t && m_d > m_t)
		review = 1;
	else if (y_t == y_d && m_d == m_t && d_d >= d_t)
		review = 1;
	return review;
}
void firstuser(void)
{
	SetColor(13);
	char _username[50];
	char _password[101];
	printf("Please enter your usrename:");
	SetColor(15);
	scanf("%s", _username);
	int r = 0;
	user_ptr = head;
	while (user_ptr != NULL)
	{
		if (strcmp(user_ptr->username, _username) == 0)
		{
			SetColor(13);
			printf("Please enter your password:");
			SetColor(15);
			scanf("%s", _password);
			if (strcmp(_password, user_ptr->password) == 0)
			{
				r = 1;
				chdir(user_ptr->username);
				break;
			}
			if (strcmp(_password, user_ptr->password) != 0)
			{
				r = 2;
				break;
			}
		}
		if (strcmp(user_ptr->username, _username) != 0)
			user_ptr = user_ptr->next;
	}
	if (r == 0)
	{
		printf("User login failed!\n");
		firstuser();
	}
	else if (r == 2)
	{
		printf("User login failed!\n");
		firstuser();
	}
	if (r == 1 && review_deadline(user_ptr->date) == 0)
	{
		printf("The deadline for system work for this user has expired!\n");
		firstuser();
	}
}
void create_user(void)
{
	struct user*temp;
	temp = (struct user*)malloc(sizeof(struct user));
	temp->next = NULL;
	printf("Please enter the new user name : ");
	scanf("%s", temp->name);
	printf("Please enter the new username : ");
	scanf("%s", temp->username);
	printf("Please enter the user password : ");
	scanf("%s", temp->password);
	char _password[101];
	strcpy(_password, temp->password);
	while (true)
	{
		strcpy(_password, temp->password);
		temp->power = 0;
		for (int i = 0; _password[i] != '\0'; i++)
		{
			if (_password[i] >= 48 && _password[i] <= 57)
				temp->power += 1;
			else if (_password[i] >= 65 && _password[i] <= 90)
				temp->power += 4;
			else if (_password[i] >= 97 && _password[i] <= 122)
				temp->power += 2;
			else if (_password[i] == 32)
				temp->power += 1;
			else if (_password[i] == 33 && _password[i] == 34 && _password[i] == 38 && _password[i] == 39 && _password[i] == 40 && _password[i] == 41 && _password[i] == 42 && _password[i] == 44 && _password[i] == 46 && _password[i] == 47 && _password[i] == 58 && _password[i] == 59 && _password[i] == 60 && _password[i] == 62 && _password[i] == 63 && _password[i] == 64 && _password[i] == 91 && _password[i] == 92 && _password[i] == 93 && _password[i] == 95 && _password[i] == 123 && _password[i] == 125)
				temp->power += 8;
		}
		if (temp->power > 33)
		{
			if (temp->power < 10)
			{
				temp->powwer[0] = temp->power + '0';
				temp->powwer[1] = '\0';
			}
			if (temp->power < 100)
			{
				temp->powwer[0] = temp->power / 10 + '0';
				temp->powwer[1] = temp->power % 10 + '0';
				temp->powwer[2] = '\0';
			}
			if (temp->power == 100)
			{
				temp->powwer[0] = 1 + '0';
				temp->powwer[1] = 0 + '0';
				temp->powwer[2] = 0 + '0';
				temp->powwer[3] = '\0';
			}
			break;
		}
		if (temp->power <= 33)
		{
			printf("Password is invalid !\n");
			user_ptr->wrong__orders++;
			wrong_orders();
			printf("Please enter a new password:");
			scanf("%s", temp->password);
		}
	}
	chdir(root_address);
	printf("Please enter the deadline with the simulator : ");
	scanf("%s", &temp->date);
	printf("Please enter the access level(For user enter '0' and for admin enter'1'):");
	scanf("%s", &temp->access_level);//gets(temp->access_level);
	temp->access__level = chartoint(temp->access_level);
	temp->wrong__orders = 0;
	strcpy(temp->wrong_orders, "0");
	mkdir(temp->username);
	chdir(user_ptr->username);
	//chdir(user_ptr->username);
	dir_temp = (struct dir*)malloc(sizeof(struct dir));
	dir_temp->nextdir = NULL;
	strcpy(dir_temp->dirname, temp->username);
	strcpy(dir_temp->dirmaker, temp->username);
	char path[_MAX_DIR];
	_getcwd(path, _MAX_DIR);
	strcpy(dir_temp->diraddress, path);
	SetColor(15);
	printf("Please enter the directory access level named %s(for only directory maker enter '1' or for directory maker and admins enter'2' or for all members enter '3'):", temp->username);
	char a[2];
	SetColor(15);
	scanf("%s", a);
	strcpy(dir_temp->dir_access_level, a);
	if (dir_head == NULL)
	{
		dir_head = dir_temp;
		dir_tail = dir_temp;
	}
	else
	{
		dir_tail->nextdir = dir_temp;
		dir_tail = dir_temp;
	}
	tail->next = temp;
	tail = temp;
}
void passwd(char *string)
{
	if (strcmp(user_ptr->access_level, "0") == 0)
	{
		//printf("Enter your new password : ");
		char password[101];
		//gets(password);
		strcpy(password, string);
		int power = 0;
		for (int i = 0; password[i] != '\0'; i++)
		{
			if (password[i] >= 48 && password[i] <= 57)
				power += 1;
			if (password[i] >= 65 && password[i] <= 90)
				power += 4;
			if (password[i] >= 97 && password[i] <= 122)
				power += 2;
			if (password[i] == 32)
				power += 1;
			if (password[i] == 33 && password[i] == 34 && password[i] == 38 && password[i] == 39 && password[i] == 40 && password[i] == 41 && password[i] == 42 && password[i] == 44 && password[i] == 46 && password[i] == 47 && password[i] == 58 && password[i] == 59 && password[i] == 60 && password[i] == 62 && password[i] == 63 && password[i] == 64 && password[i] == 91 && password[i] == 92 && password[i] == 93 && password[i] == 95 && password[i] == 123 && password[i] == 125)
				power += 8;
		}
		if (power > 33)
		{
			strcpy(user_ptr->password, password);
			if (power < 100)
			{
				user_ptr->powwer[0] = power / 10 + '0';
				user_ptr->powwer[1] = power % 10 + '0';
				user_ptr->powwer[3] = '\0';
			}
			if (power == 100)
			{
				user_ptr->powwer[0] = 1 + '0';
				user_ptr->powwer[1] = 0 + '0';
				user_ptr->powwer[2] = 0 + '0';
				user_ptr->powwer[3] = '\0';
			}
		}
		if (power <= 33)
		{
			printf("Password is invalid !\n");
			user_ptr->wrong__orders++;
			wrong_orders();
		}
	}
	if (strcmp(user_ptr->access_level, "1") == 0)
	{
		printf("Enter the username you want to change the password:");
		char username_[50];
		gets(username_);
		struct user *p_ptr = head;
		int r = 0;
		for (p_ptr = head; p_ptr != NULL&&strcmp(p_ptr->username, username_) != 0; p_ptr = p_ptr->next);
		if (strcmp(p_ptr->username, username_) == 0)
		{
			printf("Please enter the new user password:");
			char password[100];
			gets(password);
			int power = 0;
			for (int i = 0; password[i] != '\0'; i++)
			{
				if (password[i] >= 48 && password[i] <= 57)
					power += 1;
				if (password[i] >= 65 && password[i] <= 90)
					power += 4;
				if (password[i] >= 97 && password[i] <= 122)
					power += 2;
				if (password[i] == 32)
					power += 1;
				if (password[i] == 33 && password[i] == 34 && password[i] == 38 && password[i] == 39 && password[i] == 40 && password[i] == 41 && password[i] == 42 && password[i] == 44 && password[i] == 46 && password[i] == 47 && password[i] == 58 && password[i] == 59 && password[i] == 60 && password[i] == 62 && password[i] == 63 && password[i] == 64 && password[i] == 91 && password[i] == 92 && password[i] == 93 && password[i] == 95 && password[i] == 123 && password[i] == 125)
					power += 8;
			}
			if (power > 33)
			{
				strcpy(p_ptr->password, password);
				if (power < 100)
				{
					user_ptr->powwer[0] = power / 10 + '0';
					user_ptr->powwer[1] = power % 10 + '0';
					user_ptr->powwer[3] = '\0';
				}
				if (power == 100)
				{
					user_ptr->powwer[0] = 1 + '0';
					user_ptr->powwer[1] = 0 + '0';
					user_ptr->powwer[2] = 0 + '0';
					user_ptr->powwer[3] = '\0';
				}
			}
			if (power <= 33)
			{
				printf("Password is invalid !\n");
				user_ptr->wrong__orders++;
				wrong_orders();
			}
		}
		if (p_ptr == NULL)
		{
			printf("This user could not be found!\n");
			user_ptr->wrong__orders++;
			wrong_orders();
		}
	}
}
void passwd_l(void)
{
	if (strcmp(user_ptr->access_level, "1") == 0)
	{
		char date[11], username[50];
		scanf("%s", date);
		scanf("%s", username);
		struct user*i_ptr;
		for (i_ptr = head; i_ptr != NULL && strcmp(i_ptr->username, username) != 0; i_ptr = i_ptr->next);
		if (i_ptr == NULL)
		{
			printf("This user could not be found!\n");
			user_ptr->wrong__orders++;
			wrong_orders();
		}
		else if (strcmp(i_ptr->username, username) == 0)
		{
			updating_ptr = i_ptr;
			strcpy(updating_ptr->date, date);
		}
	}
	else
	{
		printf("You are not an administrator and You do not have permission to use this command!\n");
		user_ptr->wrong__orders++;
		wrong_orders();
	}
}
void su_username(char * username)
{
	char password_[101];
	struct user *i_ptr;
	for (i_ptr = head; i_ptr != NULL && strcmp(i_ptr->username, username) != 0; i_ptr = i_ptr->next);
	if (strcmp(i_ptr->username, username) == 0)
	{
		if (strcmp(user_ptr->access_level, "0") == 0)
		{
			printf("Please enter the user password:");
			//gets(password_);
			scanf("%s", password_);
			if (strcmp(i_ptr->password, password_) == 0)
				if (review_deadline(i_ptr->date) == 1)
				{
					user_ptr = i_ptr;
					chdir(root_address);
					chdir(user_ptr->username);
				}
				else
				{
					printf("The deadline for system work for this user has expired!\n");
					user_ptr->wrong__orders++;
					wrong_orders();
				}
			if (strcmp(i_ptr->password, password_) != 0)
			{
				printf("Password is invalid !\n");
				user_ptr->wrong__orders++;
				wrong_orders();
			}
		}
		if (strcmp(user_ptr->access_level, "1") == 0)
		{
			user_ptr = i_ptr;
			chdir(root_address);
			chdir(user_ptr->username);
		}
	}//user found
	else if (i_ptr == NULL)
	{
		printf("This user could not be found!\n");
		user_ptr->wrong__orders++;
		wrong_orders();
	}
}
void chgr_username(char* username)
{
	if (strcmp(user_ptr->access_level, "0") == 0)
	{
		printf("You are not an administrator and You do not have permission to use this command!\n");
		user_ptr->wrong__orders++;
		wrong_orders();
	}
	else if (strcmp(user_ptr->access_level, "1") == 0)
	{
		for (updating_ptr = head; updating_ptr != NULL && strcmp(updating_ptr->username, username) != 0; updating_ptr = updating_ptr->next);
		if (strcmp(updating_ptr->username, username) == 0)
		{
			if (strcmp(updating_ptr->access_level, "0") == 0)
			{
				updating_ptr->power = chartoint(updating_ptr->powwer);
				if (updating_ptr->power > 75)
				{
					if (updating_ptr->wrong__orders <= 10)
					{
						updating_ptr->access__level = 1;
						strcpy(updating_ptr->access_level, "1");
					}
					else if (updating_ptr->wrong__orders > 10)
					{
						printf("User's wrong orders exceeded!\n");
						user_ptr->wrong__orders++;
						wrong_orders();
					}
				}
				else if (updating_ptr->power <= 75)
				{
					printf("The user's password strength is lower than the limit!\n");
					user_ptr->wrong__orders++;
					wrong_orders();
				}
			}
			else if (strcmp(updating_ptr->access_level, "0") != 0)
			{
				printf("This user is the administrator!\n");
				user_ptr->wrong__orders++;
				wrong_orders();
			}
		}
		else if (updating_ptr == NULL)
		{
			printf("This user could not be found!\n");
			user_ptr->wrong__orders++;
			wrong_orders();
		}
	}
}
void cd(void)
{
	int a = 0;
	char dirname[20];
	scanf("%s", dirname);
	struct dir*p;
	for (p = dir_head; strcmp(dirname, p->dirname)!= 0 && p != NULL; p = p->nextdir);
	if (p == NULL)
		printf("This directory is not available!\n");
	else if (strcmp(dirname, p->dirname) == 0)
	{
		if (strcmp(p->dir_access_level, "3") == 0)
			chdir(dirname);
		else if (strcmp(p->dir_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
				chdir(dirname);
			else if (strcmp(user_ptr->username, p->dirmaker) == 0)
				chdir(dirname);
		}
		else if (strcmp(p->dir_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->dirmaker) == 0)
				chdir(dirname);
		}
		else
			printf("You do not have permission to access this directory!\n");
	}
}
void mkdir(void)
{
	char dirname[20];
	scanf("%s", dirname);
	mkdir(dirname);
	dir_temp = (struct dir*)malloc(sizeof(struct dir));
	dir_temp->nextdir = NULL;
	strcpy(dir_temp->dirname, dirname);
	strcpy(dir_temp->dirmaker, user_ptr->username);
	char path[_MAX_DIR];
	_getcwd(path, _MAX_DIR);
	strcpy(dir_temp->diraddress, path);
	SetColor(15);
	printf("Please enter the directory access level named %s(for only directory maker enter '1' or for directory maker and admins enter'2' or for all members enter '3'):", dirname);
	char a[2];
	SetColor(15);
	scanf("%s", a);
	strcpy(dir_temp->dir_access_level, a);
	if (dir_head == NULL)
	{
		dir_head = dir_temp;
		dir_tail = dir_temp;
	}
	else
	{
		dir_tail->nextdir = dir_temp;
		dir_tail = dir_temp;
	}
}
void pwd(void)
{
	char path[_MAX_DIR];
	_getcwd(path, _MAX_DIR);
	printf("%s\n", path);
}
void rm_r(void)
{
	char dirname[_MAX_PATH];
	gets(dirname);
	char ch[20];
	strcpy(ch, "rmdir/s");
	strcat(ch, dirname);
	struct dir*p;
	for (p = dir_head; strcmp(dirname, p->dirname) != 0 && p != NULL; p = p->nextdir);
	if (p == NULL)
		printf("This directory is not available!\n");
	else if (strcmp(dirname, p->dirname) == 0)
	{
		if (strcmp(p->dir_access_level, "3") == 0)
		{
			system(ch);
			return;
		}
		else if (strcmp(p->dir_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
			{
				system(ch);
				return;
			}
			else if (strcmp(user_ptr->username, p->dirmaker) == 0)
			{
				system(ch);
				return;
			}
		}
		else if (strcmp(p->dir_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->dirmaker) == 0)
			{
				system(ch);
				return;
			}
		}
		printf("You do not have permission to access this directory!\n");
	}
}
void rm(void)
{
	char file_name[50];
	scanf("%s", file_name);
	strcat(file_name, ".txt");
	struct file*p;
	for (p = file_head; strcmp(file_name, p->filename) != 0 && p != NULL; p = p->nextfile);
	if (p == NULL)
		printf("This file is not available!\n");
	else if (strcmp(file_name, p->filename) == 0)
	{
		int b = 0;
		if (strcmp(p->file_access_level, "3") == 0)
		{
			remove(file_name);
			b = 1;
		}
		else if (strcmp(p->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
			{
				remove(file_name);
				b = 1;
			}
			else if (strcmp(user_ptr->username, p->filemaker) == 0)
			{
				remove(file_name);
				b = 1;
			}
			else
				printf("You do not have permission to access this file!\n");
		}
		else if (strcmp(p->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->filemaker) == 0)
			{
				remove(file_name);
				b = 1;
			}
			else
				printf("You do not have permission to access this file!\n");
		}
		else
			printf("You do not have permission to access this file!\n");
		if (b == 1)
		{
			struct file*lp;
			for (p = file_head, lp = NULL; strcmp(file_name, p->filename) == 0 && p != NULL; lp = p, p = p->nextfile);
			if (lp == NULL)
				p = p->nextfile;
			else
				lp->nextfile = p->nextfile;
		}
	}
}
void cp(void)
{
	char ch, source_file[20], target_file[20];
	FILE *source, *target;
	scanf("%s", source_file);
	scanf("%s", target_file);
	//char s[50], t[50];
	strcat(source_file, ".txt");
	strcat(target_file, ".txt");
	int r = 0;
	struct file*p;
	for (p = file_head; strcmp(source_file, p->filename) != 0 && p != NULL; p = p->nextfile);
	if (p == NULL)
		printf("This file is not available!\n");
	else if (strcmp(source_file, p->filename) != 0)
	{
		if (strcmp(p->file_access_level, "3") == 0)
		{
			r = 1;
			//return;
		}
		else if (strcmp(p->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
			{
				r = 1;
				//return;
			}
			else if (strcmp(user_ptr->username, p->filemaker) == 0)
			{
				r = 1;
				//return;
			}
		}
		else if (strcmp(p->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->filemaker) == 0)
			{
				r = 1;
				//return;
			}
		}
		else if (r == 0)
			printf("You do not have permission to access this file!\n");
	}
	if (r == 1)
	{
		source = fopen(source_file, "r");
		if (source == NULL)
		{
			printf("There is no such file\n");
			return;
		}
		target = fopen(target_file, "w");
		while ((ch = fgetc(source)) != EOF)
			fputc(ch, target);
		fcloseall();
	}
}
void mv(void)
{
	char ch, source_file[20], target_file[20];
	FILE *source, *target;
	scanf("%s", source_file);
	scanf("%s", target_file);
	//char s[50], t[50];
	strcat(source_file, ".txt");
	strcat(target_file, ".txt");
	int r = 0;
	struct file*p;
	for (p = file_head; strcmp(source_file, p->filename) != 0 && p != NULL; p = p->nextfile);
	if (p == NULL)
		printf("This file is not available!\n");
	else if (strcmp(source_file, p->filename) != 0)
	{
		if (strcmp(p->file_access_level, "3") == 0)
			r = 1;
		else if (strcmp(p->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
				r = 1;
			else if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else if (strcmp(p->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else
			printf("You do not have permission to access this file!\n");
	}
	if (r == 1)
	{
		source = fopen(source_file, "r");
		if (source == NULL)
		{
			printf("There is no such file\n");
			return;
		}
		target = fopen(target_file, "w");
		while ((ch = fgetc(source)) != EOF)
			fputc(ch, target);
		fcloseall();
		remove(source_file);
	}
}
void cat(void)
{
	FILE *ptr;
	char filename[20];
	scanf("%s", filename);
	strcat(filename, ".txt");
	int r = 0;
	struct file*p;
	for (p = file_head; strcmp(filename, p->filename) != 0 && p != NULL; p = p->nextfile);
	if (p == NULL)
		printf("This file is not available!\n");
	else if (strcmp(filename, p->filename) != 0)
	{
		if (strcmp(p->file_access_level, "3") == 0)
			r = 1;
		else if (strcmp(p->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
				r = 1;
			else if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else if (strcmp(p->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else
			printf("You do not have permission to access file!\n");
	}
	if (r == 1)
	{
		ptr = fopen(filename, "r");
		char str[100];
		while (!feof(ptr))
		{
			fgets(str, 100, ptr);
			printf("%s", str);
		}
		printf("\n");
	}
}
void shift(char *str)
{
	char path[_MAX_DIR];
	_getcwd(path, _MAX_DIR);
	//chdir(user_ptr->username);
	char * token_l, *token_r;
	char l_[50], r_[50];
	token_l = strtok(str, ">");
	strcpy(l_, token_l);
	token_r = strtok(NULL, ">");
	strcpy(r_, token_r);
	strcat(l_, ".txt");
	strcat(r_, ".txt");
	FILE *ptr, *r, *l;
	char s[100];
	int t = 0;
	int f = 0;
	FILE *k, *g;
	if ((k = fopen(l_, "r")) != NULL)
	{
		struct file*p;
		for (p = file_head; strcmp(l_, p->filename) != 0 && p != NULL; p = p->nextfile);
		if (p == NULL)
			printf("This file is not available!\n");
		else if (strcmp(l_, p->filename) != 0)
		{
			if (strcmp(p->file_access_level, "3") == 0)
				f = 1;
			else if (strcmp(p->file_access_level, "2") == 0)
			{
				if (strcmp(user_ptr->access_level, "1") == 0)
					f = 1;
				else if (strcmp(user_ptr->username, p->filemaker) == 0)
					f = 1;
			}
			else if (strcmp(p->file_access_level, "1") == 0)
			{
				if (strcmp(user_ptr->username, p->filemaker) == 0)
					f = 1;
			}
			else
				printf("You do not have permission to access file(%s)!\n", token_l);
		}
		fclose(k);
	}
	if (f == 1 && (g = fopen(r_, "r")) != NULL)
	{
		struct file*p;
		for (p = file_head; strcmp(r_, p->filename) != 0 && p != NULL; p = p->nextfile);
		if (p == NULL)
			printf("This file is not available!\n");
		else if (strcmp(r_, p->filename) == 0)
		{
			if (strcmp(p->file_access_level, "3") == 0)
				t = 1;
			else if (strcmp(p->file_access_level, "2") == 0)
			{
				if (strcmp(user_ptr->access_level, "1") == 0)
					t = 1;
				else if (strcmp(user_ptr->username, p->filemaker) == 0)
					t = 1;
			}
			else if (strcmp(p->file_access_level, "1") == 0)
			{
				if (strcmp(user_ptr->username, p->filemaker) == 0)
					t = 1;
			}
			else
				printf("You do not have permission to access file(%s)!\n", token_r);
		}
		fclose(g);
	}
	if (((l = fopen(l_, "r")) != NULL && (r = fopen(r_, "r")) != NULL &&  f == 1 && t == 1) || ((l = fopen(l_, "r")) == NULL && (r = fopen(r_, "r")) == NULL &&  f == 0 && t == 0))
	{
		if ((r = fopen(r_, "r")) == NULL)
		{
			r = fopen(r_, "w");
			file_temp = (struct file*)malloc(sizeof(struct file));
			file_temp->nextfile = NULL;
			strcpy(file_temp->filename, r_);
			strcpy(file_temp->filemaker, user_ptr->username);
			strcpy(file_temp->fileaddress, path);
			time_t rawtime;
			struct tm *info;
			char buffer[80];
			time(&rawtime);
			info = localtime(&rawtime);
			strftime(buffer, 80, "%x - %I:%M%p", info);
			strcpy(file_temp->construction_time, buffer);
			strcpy(file_temp->lastseen, buffer);
			printf("Please enter the file access level named %s(for only file maker enter '1' or for file maker and admins enter'2' or for all members enter '3'):", file_temp->filename);
			char a[2];
			scanf("%s", a);
			strcpy(file_temp->file_access_level, a);
			if (file_head == NULL)
			{
				file_head = file_temp;
				file_tail = file_temp;
			}
			else if (file_head != NULL)
			{
				file_tail->nextfile = file_temp;
				file_tail = file_temp;
			}
			fcloseall();
		}
		else if ((r = fopen(r_, "r")) != NULL)
		{
			r = fopen(r_, "w");
			time_t rawtime;
			struct tm *info;
			char buffer[80];
			time(&rawtime);
			info = localtime(&rawtime);
			strftime(buffer, 80, "%x - %I:%M%p", info);
			for (file_ptr = file_head; strcmp(file_ptr->filename, r_) != 0; file_ptr = file_ptr->nextfile);
			strcpy(file_ptr->lastseen, buffer);
		}
		if ((l = fopen(l_, "r")) != NULL)
		{
			while (!feof(l))
			{
				fgets(s, 50, l);
				fprintf(r, "%s\n", s);
			}
		}
		else if ((l = fopen(l_, "r")) == NULL)
		{
			ptr = fopen(l_, "w");
			fprintf(r, "%s", token_l);
			file_temp = (struct file*)malloc(sizeof(struct file));
			file_temp->nextfile = NULL;
			strcpy(file_temp->filename, l_);
			strcpy(file_temp->filemaker, user_ptr->username);
			strcpy(file_temp->fileaddress, path);
			time_t rawtime;
			struct tm *info;
			char buffer[80];
			time(&rawtime);
			info = localtime(&rawtime);
			strftime(buffer, 80, "%x - %I:%M%p", info);
			strcpy(file_temp->construction_time, buffer);
			strcpy(file_temp->lastseen, buffer);
			printf("Please enter the file access level named %s(for only file maker enter '1' or for file maker and admins enter'2' or for all members enter '3'):", file_temp->filename);
			char a[2];
			scanf("%s", a);
			strcpy(file_temp->file_access_level, a);
			if (file_head == NULL)
			{
				file_head = file_temp;
				file_tail = file_temp;
			}
			else if (file_head != NULL)
			{
				file_tail->nextfile = file_temp;
				file_tail = file_temp;
			}
			fcloseall();
		}
		fcloseall();
	}
}
void double_shift(char * str)
{
	char path[_MAX_DIR];
	_getcwd(path, _MAX_DIR);
	//chdir(user_ptr->username);
	char * token_l, *token_r;
	char l_[50], r_[50];
	token_l = strtok(str, ">");
	strcpy(l_, token_l);
	token_r = strtok(NULL, ">");
	strcpy(r_, token_r);
	strcat(l_, ".txt");
	strcat(r_, ".txt");
	FILE *ptr, *r, *l;
	FILE *k, *g;
	int f = 0, t = 0;
	if ((k = fopen(l_, "r")) != NULL)
	{
		struct file*p;
		for (p = file_head; strcmp(l_, p->filename) != 0 && p != NULL; p = p->nextfile);
		if (p == NULL)
			printf("This file is not available!\n");
		else if (strcmp(l_, p->filename) == 0)
		{
			if (strcmp(p->file_access_level, "3") == 0)
				f = 1;
			else if (strcmp(p->file_access_level, "2") == 0)
			{
				if (strcmp(user_ptr->access_level, "1") == 0)
					f = 1;
				else if (strcmp(user_ptr->username, p->filemaker) == 0)
					f = 1;
			}
			else if (strcmp(p->file_access_level, "1") == 0)
			{
				if (strcmp(user_ptr->username, p->filemaker) == 0)
					f = 1;
			}
			else
				printf("You do not have permission to access file(%s)!\n", token_l);
		}
		fclose(k);
	}
	if (f == 1 && (g = fopen(r_, "r")) != NULL)
	{
		struct file*p;
		for (p = file_head; strcmp(r_, p->filename) == 0 && p != NULL; p = p->nextfile);
		if (p == NULL)
			printf("This file is not available!\n");
		else
		{
			if (strcmp(p->file_access_level, "3") == 0)
				t = 1;
			else if (strcmp(p->file_access_level, "2") == 0)
			{
				if (strcmp(user_ptr->access_level, "1") == 0)
					t = 1;
				else if (strcmp(user_ptr->username, p->filemaker) == 0)
					t = 1;
			}
			else if (strcmp(p->file_access_level, "1") == 0)
			{
				if (strcmp(user_ptr->username, p->filemaker) == 0)
					t = 1;
			}
			else
				printf("You do not have permission to access file(%s)!\n", token_r);
		}
		fclose(g);
	}
	if (((l = fopen(l_, "r")) != NULL && (r = fopen(r_, "r")) != NULL &&  f == 1 && t == 1) || ((l = fopen(l_, "r")) == NULL && (r = fopen(r_, "r")) == NULL &&  f == 0 && t == 0))
	{
		if ((r = fopen(r_, "r")) == NULL)
		{
			file_temp = (struct file*)malloc(sizeof(struct file));
			file_temp->nextfile = NULL;
			strcpy(file_temp->filename, r_);
			strcpy(file_temp->filemaker, user_ptr->username);
			strcpy(file_temp->fileaddress, path);
			time_t rawtime;
			struct tm *info;
			char buffer[80];
			time(&rawtime);
			info = localtime(&rawtime);
			strftime(buffer, 80, "%x - %I:%M%p", info);
			strcpy(file_temp->construction_time, buffer);
			strcpy(file_temp->lastseen, buffer);
			printf("Please enter the file access level named %s(for only file maker enter '1' or for file maker and admins enter'2' or for all members enter '3'):", file_temp->filename);
			char a[2];
			scanf("%s", a);
			strcpy(file_temp->file_access_level, a);
			if (file_head == NULL)
			{
				file_head = file_temp;
				file_tail = file_temp;
			}
			else if (file_head != NULL)
			{
				file_tail->nextfile = file_temp;
				file_tail = file_temp;
			}
		}
		if ((r = fopen(r_, "r")) != NULL)
		{
			time_t rawtime;
			struct tm *info;
			char buffer[80];
			time(&rawtime);
			info = localtime(&rawtime);
			strftime(buffer, 80, "%x - %I:%M%p", info);
			for (file_ptr = file_head; strcmp(file_ptr->filename, r_) != 0; file_ptr = file_ptr->nextfile);
			strcpy(file_ptr->lastseen, buffer);
			fclose(r);
		}
		if ((l = fopen(l_, "r")) == NULL)
		{
			file_temp = (struct file*)malloc(sizeof(struct file));
			file_temp->nextfile = NULL;
			strcpy(file_temp->filename, l_);
			strcpy(file_temp->filemaker, user_ptr->username);
			strcpy(file_temp->fileaddress, path);
			time_t rawtime;
			struct tm *info;
			char buffer[80];
			time(&rawtime);
			info = localtime(&rawtime);
			strftime(buffer, 80, "%x - %I:%M%p", info);
			strcpy(file_temp->construction_time, buffer);
			strcpy(file_temp->lastseen, buffer);
			printf("Please enter the file access level named %s(for only file maker enter '1' or for file maker and admins enter'2' or for all members enter '3'):", file_temp->filename);
			char a[2];
			scanf("%s", a);
			strcpy(file_temp->file_access_level, a);
			if (file_head == NULL)
			{
				file_head = file_temp;
				file_tail = file_temp;
			}
			else if (file_head != NULL)
			{
				file_tail->nextfile = file_temp;
				file_tail = file_temp;
			}
		}
		if ((l = fopen(l_, "r")) != NULL)
		{
			fclose(l);
		}
		if ((l = fopen(l_, "r")) == NULL)
		{
			ptr = fopen(l_, "w");
			if ((r = fopen(r_, "a")) == NULL)
			{
				//fclose(r);
				r = fopen(r_, "w");
				fprintf(r, "%s", token_l);
				fcloseall();
			}
			else if ((r = fopen(r_, "a")) != NULL)
			{
				fprintf(r, "%s", token_l);
				fclose(r);
			}
		}
		else if ((l = fopen(l_, "a")) != NULL)
		{
			char s[100];
			//l = fopen(l_, "w");
			while (!feof(l))
			{
				fgets(s, 50, l);
				fprintf(r, "%s\n", s);
			}
		}
		fcloseall();
	}
}
void ls(void)
{
	system("dir");
}
void exif(void)
{
	char s[50];
	scanf("%s", s);
	strcat(s, ".txt");
	int r = 0;
	struct file*p;
	for (p = file_head; strcmp(s, p->filename) != 0 && p != NULL; p = p->nextfile);
	if (p == NULL)
		printf("This file is not available!\n");
	else if (strcmp(s, p->filename) == 0)
	{
		if (strcmp(p->file_access_level, "3") == 0)
			r = 1;
		else if (strcmp(p->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
				r = 1;
			else if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else if (strcmp(p->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else
			printf("You do not have permission to access this file!\n");
	}
	if (r == 1)
	{
		file_ptr = file_head;
		file_ptr = file_head;
		while (strcmp(file_ptr->filename, s) != 0)
		{
			file_ptr = file_ptr->nextfile;
		}
		printf("File name : %s\n", file_ptr->filename);
		printf("File maker : %s\n", file_ptr->filemaker);
		printf("File address : %s\n", file_ptr->fileaddress);
		printf("Construction time : %s\n", file_ptr->construction_time);
		printf("Last change time : %s\n", file_ptr->lastseen);
	}
}
void wc(void)
{
	char str[40];
	scanf("%s", str);
	strcat(str, ".txt");
	int r = 0;
	struct file*p;
	for (p = file_head; strcmp(str, p->filename) != 0 && p != NULL; p = p->nextfile);
	if (p == NULL)
		printf("This file is not available!\n");
	else if (strcmp(str, p->filename) == 0)
	{
		if (strcmp(p->file_access_level, "3") == 0)
			r = 1;
		else if (strcmp(p->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
				r = 1;
			else if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else if (strcmp(p->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else
			printf("You do not have permission to access this file!\n");
	}
	if (r == 1)
	{
		FILE *ptr;
		char s[200], *strtoken;
		int word_c = 1, line_c = 0;
		if ((ptr = fopen(str, "r")) != NULL)
		{
			while (!feof(ptr))
			{
				word_c++;
				line_c++;
				fgets(s, 200, ptr);
				strtoken = strtok(s, " ");
				while ((strtoken = strtok(NULL, " ")) != NULL)
					word_c++;
			}
			printf("Number of lines : %d\n", line_c);
			printf("Number of words : %d\n", (word_c - 1));
		}
		else
			printf("No such file or directy!\n");
		fcloseall();
	}
}
void diff(void)
{
	char name1[20], name2[20], s1[200], s2[200];
	int line = 0;
	scanf("%s", name1);
	scanf("%s", name2);
	strcat(name1, ".txt");
	strcat(name2, ".txt");
	int r = 0, t = 0;
	struct file*p;
	for (p = file_head; strcmp(name1, p->filename) != 0 && p != NULL; p = p->nextfile);
	if (p == NULL)
		printf("This file is not available!\n");
	else if (strcmp(name1, p->filename) == 0)
	{
		if (strcmp(p->file_access_level, "3") == 0)
			r = 1;
		else if (strcmp(p->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
				r = 1;
			else if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else if (strcmp(p->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, p->filemaker) == 0)
				r = 1;
		}
		else
			printf("You do not have permission to access this file(%s)!\n", name1);
	}
	struct file*q;
	for (q = file_head; strcmp(name2, q->filename) != 0 && q != NULL; q = q->nextfile);
	if (q == NULL)
		printf("This directory is not available!\n");
	else if (strcmp(name2, q->filename) == 0)
	{
		if (strcmp(q->file_access_level, "3") == 0)
			t = 1;
		else if (strcmp(q->file_access_level, "2") == 0)
		{
			if (strcmp(user_ptr->access_level, "1") == 0)
				t = 1;
			else if (strcmp(user_ptr->username, q->filemaker) == 0)
				t = 1;
		}
		else if (strcmp(q->file_access_level, "1") == 0)
		{
			if (strcmp(user_ptr->username, q->filemaker) == 0)
				t = 1;
		}
		else
			printf("You do not have permission to access this file(%s)!\n", name2);
	}
	if (r == 1 && t == 1)
	{
		FILE *ptr1, *ptr2;
		ptr1 = fopen(name1, "r");
		ptr2 = fopen(name2, "r");
		while (!feof(ptr1) || !feof(ptr2))
		{
			line++;
			fgets(s1, 200, ptr1);
			fgets(s2, 200, ptr2);
			if (strcmp(s1, s2) == 0)
			{
				printf("Line %d is similar to each other in the two files\n", line);
			}
			else
				printf("Line %d is different in the two files\n", line);
		}
		fcloseall();
	}
}
void search(void)
{
	printf("if you want to search file enter \"file\" else enter \"directory\":");
	char s[10];
	scanf("%s", s);
	if (strcmp(s, "file") == 0)
	{
		printf("Enter the name of file:");
		char fs[20];
		scanf("%s", fs);
		strcat(fs, ".txt");
		struct file *p;
		//for (p = file_head; p != NULL && strcmp(p->filename, s) != 0; p = p->nextfile);
		p = file_head;
		while (strcmp(p->filename, fs) != 0 && p != NULL)
			p = p->nextfile;
		if (p == NULL)
		{
			printf("There is no such file!\n");
		}
		else
		{
			printf("The file was found.The address of file is in x\n");
			strcpy(x, p->fileaddress);
		}
	}
	else if (strcmp(s, "directory") == 0)
	{
		printf("Enter the name of directory:");
		char ds[20];
		scanf("%s", ds);
		struct dir *q;
		q = dir_head;
		while (q != NULL && strcmp(q->dirname, ds) != 0)
		{
			q = q->nextdir;
		}
		//for (q = dir_head; q != NULL && strcmp(q->dirname, s) != 0; q = q->nextdir);
		if (q == NULL)
		{
			printf("There is no such directory!\n");
		}
		else
		{
			printf("The directory was found.The address of directory is in y\n");
			strcpy(y, q->diraddress);
		}
	}
}
void help_create(void)
{
	printf("create user:This command is executed by the administrator and adds a new user.\n");
}
void help_su(void)
{
	printf("su username:This command is used to change the user.\n");
}
void help_passwd(void)
{
	printf("passwd:This command is used to change the password by the administrator or the user.\n");
}
void help_passwd_l(void)
{
	printf("passwd -l:This command is used by the administrator to change the working time of the program for the administrator or the user.\n");
}
void help_chgr(void)
{
	printf("chgr username:This command is used by the administrator to upgrade the user's access level to the administrator.\n");
}
void help_cd(void)
{
	printf("cd diname:This command will change the current location to a specific directory, and if the file name is used instead of the file name, the current location goes to a directory earlier.\n");
}
void help_pwd(void)
{
	printf("pwd:This command represents the current location of the system.\n");
}
void help_mkdir(void)
{
	printf("mkdir dirname:This command creates a new directory.\n");
}
void help_shift(void)
{
	printf("statemente1>statement2:The phrase or contents of the left file is written at the end of the right file.\n");
}
void help_double_shift(void)
{
	printf("statement1>>statement2:The phrase or contents of the leftmost file of the first file is written right and the previous contents of the file are cleared.\n");
}
void help_cat(void)
{
	printf("cat filename:Entering this command and the file name will display the contents of the file.\n");
}
void help_rm(void)
{
	printf("rm filename:This command is used to delete the file.\n");
}
void help_rm_r(void)
{
	printf("rm-r dirname:This command is used to delete the directory.\n");
}
void help_cp(void)
{
	printf("cp file1 file2:This command copies a file to another location.\n");
}
void help_mv(void)
{
	printf("mv file1 file2:This command transfers a file to another location.\n");
}
void help_wc(void)
{
	printf("wc filename:This command provides a report on the number of lines and the number of words in a file.\n");
}
void help_diff(void)
{
	printf("diff file1 file2:This command compares the two files together and displays a report.\n");
}
void help(void)
{
	printf("\n");
	help_create();
	printf("\n");
	help_su();
	printf("\n");
	help_passwd();
	printf("\n");
	help_passwd_l();
	printf("\n");
	help_chgr();
	printf("\n");
	help_cd();
	printf("\n");
	help_pwd();
	printf("\n");
	help_mkdir();
	printf("\n");
	help_shift();
	printf("\n");
	help_double_shift();
	printf("\n");
	help_cat();
	printf("\n");
	help_rm();
	printf("\n");
	help_rm_r();
	printf("\n");
	help_cp();
	printf("\n");
	help_mv();
	printf("\n");
	help_wc();
	printf("\n");
	help_diff();
}
void change_access_level(void)
{//chacc filename 3
	char sf[50], sd[50];
	scanf("%s", sd);
	strcpy(sf, sd);
	strcat(sf, ".txt");
	char a[2];
	scanf("%s", a);
	struct dir*d;
	for (d = dir_head; d != NULL && strcmp(d->dirname, sd) != 0; d = d->nextdir);
	if (d == NULL)
	{
		struct file*f;
		for (f = file_head; f != NULL && strcmp(f->filename, sf) != 0; f = f->nextfile);
		if (f == NULL)
		{
			printf("There is not such file or directory!\n");
		}
		else if (strcmp(f->filename, sf) == 0)
		{
			strcpy(f->file_access_level, a);
		}
	}
	if (strcmp(d->dirname, sd) == 0)
	{
		strcpy(d->dir_access_level, a);
	}
}
void userfile_to_linkedlist(void)
{
	struct user*temp;
	FILE * fileptr;
	char str[100];
	char *tokenptr;
	fileptr = fopen("userfile.txt", "r");
	while (!feof(fileptr))
	{
		temp = (struct user *)malloc(sizeof(struct user));
		temp->next = NULL;
		fgets(str, 100, fileptr);
		tokenptr = strtok(str, ",");
		strcpy(temp->name, tokenptr);
		tokenptr = strtok(NULL, ",");
		strcpy(temp->username, tokenptr);
		tokenptr = strtok(NULL, ",");
		strcpy(temp->password, tokenptr);
		tokenptr = strtok(NULL, ",");
		strcpy(temp->powwer, tokenptr);
		tokenptr = strtok(NULL, ",");
		strcpy(temp->date, tokenptr);
		tokenptr = strtok(NULL, ",");
		strcpy(temp->access_level, tokenptr);
		tokenptr = strtok(NULL, ",");
		temp->wrong__orders = 0;
		strcpy(temp->wrong_orders, tokenptr);
		if (head == NULL)
		{
			head = temp;
			tail = temp;
			temp->next = NULL;
		}
		else
		{
			tail->next = temp;
			tail = temp;
			temp->next = NULL;
		}
	}
	fclose(fileptr);
}
void print_to_file(void)
{
	fileptr = fopen("userfile.txt", "w");
	struct user *i_ptr;
	for (i_ptr = head; i_ptr->next != NULL; i_ptr = i_ptr->next)
	{
		fprintf(fileptr, "%s", i_ptr->name);
		fprintf(fileptr, ",");
		fprintf(fileptr, "%s", i_ptr->username);
		fprintf(fileptr, ",");
		fprintf(fileptr, "%s", i_ptr->password);
		fprintf(fileptr, ",");
		fprintf(fileptr, "%s", i_ptr->powwer);
		fprintf(fileptr, ",");
		fprintf(fileptr, "%s", i_ptr->date);
		fprintf(fileptr, ",");
		fprintf(fileptr, "%s", i_ptr->access_level);
		fprintf(fileptr, ",");
		fprintf(fileptr, "%s", i_ptr->wrong_orders);
		fprintf(fileptr, ",");
		fprintf(fileptr, "\n");
	}
	fprintf(fileptr, "%s", i_ptr->name);
	fprintf(fileptr, ",");
	fprintf(fileptr, "%s", i_ptr->username);
	fprintf(fileptr, ",");
	fprintf(fileptr, "%s", i_ptr->password);
	fprintf(fileptr, ",");
	fprintf(fileptr, "%s", i_ptr->powwer);
	fprintf(fileptr, ",");
	fprintf(fileptr, "%s", i_ptr->date);
	fprintf(fileptr, ",");
	fprintf(fileptr, "%s", i_ptr->access_level);
	fprintf(fileptr, ",");
	fprintf(fileptr, "%s", i_ptr->wrong_orders);
	fprintf(fileptr, ",");
	fclose(fileptr);
}
void struct_file_to_struct(void)
{
	FILE *p;
	char s[180];
	char *str;
	p = fopen("struct_file.txt", "r");
	while (!feof(p))
	{
		file_temp = (struct file*)malloc(sizeof(struct file));
		file_temp->nextfile = NULL;
		fgets(s, 180, p);
		str = strtok(s, ",");
		strcpy(file_temp->filename, str);
		str = strtok(NULL, ",");
		strcpy(file_temp->filemaker, str);
		str = strtok(NULL, ",");
		strcpy(file_temp->fileaddress, str);
		str = strtok(NULL, ",");
		strcpy(file_temp->construction_time, str);
		str = strtok(NULL, ",");
		strcpy(file_temp->lastseen, str);
		str = strtok(NULL, ",");
		strcpy(file_temp->file_access_level, str);
		if (file_head == NULL)
		{
			file_head = file_temp;
			file_tail = file_temp;
			file_temp->nextfile = NULL;
		}
		else
		{
			file_tail->nextfile = file_temp;
			file_tail = file_temp;
			file_temp->nextfile = NULL;
		}
	}
	fclose(p);
}
void struct_dir_to_struct(void)
{
	FILE *p;
	char s[180];
	char *str;
	p = fopen("struct_dir.txt", "r");
	while (!feof(p))
	{
		dir_temp = (struct dir*)malloc(sizeof(struct dir));
		dir_temp->nextdir = NULL;
		fgets(s, 180, p);
		str = strtok(s, ",");
		strcpy(dir_temp->dirname, str);
		str = strtok(NULL, ",");
		strcpy(dir_temp->dirmaker, str);
		str = strtok(NULL, ",");
		strcpy(dir_temp->diraddress, str);
		str = strtok(NULL, ",");
		strcpy(dir_temp->dir_access_level, str);
		if (dir_head == NULL)
		{
			dir_head = dir_temp;
			dir_tail = dir_temp;
			dir_temp->nextdir = NULL;
		}
		else
		{
			dir_tail->nextdir;
			dir_tail = dir_temp;
			dir_temp->nextdir = NULL;
		}
	}
}
void struct_dir_to_file(void)
{
	FILE *dptr;
	dptr = fopen("struct_dir.txt", "w");
	struct dir*p = dir_head;
	while (p->nextdir != NULL)
	{
		fprintf(dptr, "%s,", p->dirname);
		fprintf(dptr, "%s,", p->dirmaker);
		fprintf(dptr, "%s,", p->diraddress);
		fprintf(dptr, "%s,", p->dir_access_level);
		fprintf(dptr, "\n");
		p = p->nextdir;
	}
	fprintf(dptr, "%s,", p->dirname);
	fprintf(dptr, "%s,", p->dirmaker);
	fprintf(dptr, "%s,", p->diraddress);
	fprintf(dptr, "%s,", p->dir_access_level);
	fclose(dptr);
}
void struct_file_to_file(void)
{
	FILE *fptr;
	fptr = fopen("struct_file.txt", "w");
	struct file *p = file_head;
	while (p->nextfile != NULL)
	{
		fprintf(fptr, "%s,", p->filename);
		fprintf(fptr, "%s,", p->filemaker);
		fprintf(fptr, "%s,", p->fileaddress);
		fprintf(fptr, "%s,", p->construction_time);
		fprintf(fptr, "%s,", p->lastseen);
		fprintf(fptr, "%s,", p->file_access_level);
		fprintf(fptr, "\n");
		p = p->nextfile;
	}
	fprintf(fptr, "%s,", p->filename);
	fprintf(fptr, "%s,", p->filemaker);
	fprintf(fptr, "%s,", p->fileaddress);
	fprintf(fptr, "%s,", p->construction_time);
	fprintf(fptr, "%s,", p->lastseen);
	fprintf(fptr, "%s,", p->file_access_level);
	fclose(fptr);
}
int main()
{
	chdir("root");
	strcpy(root_address, "C:\\Users\\farzaneh\\Documents\\Visual Studio 2013\\Projects\\Project6\\Project6\\root");
	SetColor(12);
	printf("\n");
	printf("\t\t\t\t\t\t\t\"Welcome to the Linux Terminal Simulator\"\n");
	printf("\t\t\t\t\t\t\t  \"programming by Farzaneh Koohestani\"\n");
	printf("\t\t\t\t\t\t\t  \"Isfahan University of Thechnology\"\n");
	printf("________________________________________________________________________________________________________________________________________________________________________\n");
	SetColor(11);
	printf("\n");
	printf("Enter today's date:");
	SetColor(15);//blue
	gets(today);
	userfile_to_linkedlist();
	struct_dir_to_struct();
	struct_file_to_struct();
	firstuser();
	while (true)
	{
		SetColor(14);
		printf("**");
		SetColor(10);
		printf("%s", user_ptr->username);
		SetColor(14);
		printf("**");
		SetColor(5);
		printf(":");
		SetColor(15);
		char s1[10];
		int c = 0;
		getch();
		scanf("%s", s1);
		char * t1;
		t1 = s1;
		char s[50], ss[50];
		strcpy(s, s1);
		strcpy(ss, s1);
		char sss[30], *st;
		strcpy(sss, ss);
		st = strtok(ss, " ");
		if (strcmp(s1, "create") == 0)
		{
			char s2[10];
			scanf("%s", s2);
			if (strcmp(s2, "user") == 0)
			{
				if (strcmp(user_ptr->access_level, "1") == 0)
					create_user();
				if (strcmp(user_ptr->access_level, "0") == 0)
				{
					printf("You are not an administrator and You do not have permission to use this command!\n");
					user_ptr->wrong__orders++;
					wrong_orders();
				}
			}
			else
			{
				printf("You have entered incorrect command!\n");
				user_ptr->wrong__orders++;
				wrong_orders();
			}
		}
		else if (strcmp(s1, "passwd") == 0)
		{
			char t3[10];
			scanf("%s", t3);
			if (strcmp(t3, "-l") == 0)
				passwd_l();
			else
				passwd(t3);
		}
		else if (strcmp(strtok(s1, " "), "su") == 0)
		{
			char u[50];
			scanf("%s", u);
			su_username(u);
		}
		else if (strcmp(strtok(s1, " "), "chgr") == 0)
		{
			char s2[50];
			scanf("%s", s2);
			chgr_username(s2);
		}
		else if (strcmp(s1, "cd") == 0)
			cd();
		else if (strcmp(s1, "mkdir") == 0)
			mkdir();
		else if (strcmp(s1, "pwd") == 0)
			pwd();
		else if (strcmp(s1, "rm-r") == 0)
			rm_r();
		else if (strcmp(s1, "rm") == 0)
			rm();
		else if (strcmp(s1, "cp") == 0)
			cp();
		else if (strcmp(s1, "mv") == 0)
			mv();
		else if (strcmp(s1, "cat") == 0)
			cat();
		else if (strstr(s1, ">>") != NULL)
			double_shift(ss);
		else if (strchr(s, '>') != NULL)
			shift(ss);
		else if (strcmp(ss, "ls") == 0)
			ls();
		else if (strcmp(ss, "exif") == 0)
			exif();
		else if (strcmp(ss, "wc") == 0)
			wc();
		else if (strcmp(ss, "diff") == 0)
			diff();
		else if (strcmp(ss, "search") == 0)
			search();
		else if (strcmp(ss, "chacc") == 0)
			change_access_level();
		else if (strcmp(ss, "help") == 0)
		{
			scanf("%s", st);
			if (strcmp(st, "create") == 0)
				help_create();
			else if (strcmp(st, "su") == 0)
				help_su();
			else if (strcmp(st, "passwd") == 0)
				help_passwd();
			else if (strcmp(st, "passwd -l") == 0)
				help_passwd_l();
			else if (strcmp(st, "chgr") == 0)
				help_chgr();
			else if (strcmp(st, "cd") == 0)
				help_cd();
			else if (strcmp(st, "pwd") == 0)
				help_pwd();
			else if (strcmp(st, "mkdir") == 0)
				help_mkdir();
			else if (strcmp(st, ">") == 0)
				help_shift();
			else if (strcmp(st, ">>") == 0)
				help_double_shift();
			else if (strcmp(st, "cat") == 0)
				help_cat();
			else if (strcmp(st, "rm") == 0)
				help_rm();
			else if (strcmp(st, "rm-r") == 0)
				help_rm_r();
			else if (strcmp(st, "cp") == 0)
				help_cp();
			else if (strcmp(st, "mv") == 0)
				help_mv();
			else if (strcmp(st, "wc") == 0)
				help_wc();
			else if (strcmp(st, "diff") == 0)
				help_diff();
			else if (strcmp(st, "all_commands") == 0)
				help();
		}
		else if (strcmp(s1, "exit") == 0)
		{
			chdir(root_address);
			print_to_file();
			struct_file_to_file();
			struct_dir_to_file();
		}
		else
		{
			printf("You have entered incorrect command!\n");
			user_ptr->wrong__orders++;
			wrong_orders();
		}
	}
	return 0;
}
