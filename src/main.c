#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern int errno;

int main(void)
{
	DIR *proc_dir = opendir("/proc");
	if (proc_dir == NULL) {
		perror("opendir failed\n");
		return -1;
	}

	errno = 0;
	struct dirent *proc_dirent;
	do {
		proc_dirent = readdir(proc_dir);
		if (proc_dirent == NULL && errno != 0) {
			perror("readdir failed\n");
			return -1;
		} else if (proc_dirent == NULL)
			continue;

		bool pid_entry = true;
		for (size_t i = 0; i < strlen(proc_dirent->d_name) && pid_entry; i++) {
				if (!isdigit(proc_dirent->d_name[i]))
					pid_entry = false;
		}

		if (!pid_entry)
			continue;

		char path[6+strlen(proc_dirent->d_name)+6];
		strcpy(path, "/proc/");
		strcat(path, proc_dirent->d_name);
		strcat(path, "/stat");
		FILE *proc = fopen(path, "r");
		if (proc == NULL) {
			perror("fopen failed\n");
			return -1;
		}
		printf("%-10s", proc_dirent->d_name);

		for (int c; (c = fgetc(proc)) != EOF && c != '(';);
		int open_bracket_cnt = 1;
		for (int c; (c = fgetc(proc)) != EOF && open_bracket_cnt > 0;) {
			if (c == ')' && open_bracket_cnt == 1)
				break;
			putc(c, stdout);
			if (c == '(')
				open_bracket_cnt++;
			else if (c == ')')
				open_bracket_cnt--;
		}
		putc('\n', stdout);

		if (fclose(proc) == EOF) {
			perror("fclose failed\n");
			return -1;
		}
	} while (proc_dirent != NULL);

	if (closedir(proc_dir) == -1) {
		perror("closedir failed\n");
		return -1;
	}

	return 0;
}
