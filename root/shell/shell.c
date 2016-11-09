#include "shell.h"
#include <path.h>
#include <disk.h>
#include <readline/readline.h>

char *getPrompt() 
{
	if ( !is_mounted() ) 
		return strdup("➜ ");

	char *cwd = get_cwd();
	char *prompt = malloc(strlen(cwd) + 4);
	strcpy(prompt, cwd);
	strcpy(prompt + strlen(cwd), "➜ ");

	free(cwd);
	return prompt;
}

int main(int argc, char** argv) 
{
	const char *diskPath = NULL;
	const char *cmdPath = NULL;
	if (argc > 1) 
		diskPath = argv[1];

	init_mount();
	bool exiting = false;
	unsigned int i;
	char *line = NULL;
	while(! is_mounted() )
	{
	    char *disk = readline("Target disk: ");
	
		char tmp[512];
		memset(tmp, 0, 512);
		
		if (diskPath) 
			strcat(tmp, diskPath);
			
		strcat(tmp, disk);
		mount(tmp);
		free(disk);
	}
	
	for(;;) 
	{
		char *prompt = getPrompt();
		line = readline(prompt);
		free(prompt);

		if (line == NULL || strlen(line) == 0) 
		{
			free(line);
			line = NULL;
			continue;
		}
		
		unsigned int cmdOffset = 0;
		for (i = 0; i < strlen(line); ++i) 
		{
			if (line[i] == ' ')
				++cmdOffset;
			else
				break;
		}

		unsigned int sub_argc = 1;
		for (i = cmdOffset; i < strlen(line); ++i)
			if (line[i] == ' ')
				++sub_argc;

		char ** sub_argv = malloc((sub_argc + 1) * sizeof(char*));
		for (i = 0; i < sub_argc + 1; ++i) 
			sub_argv[i] = NULL;

		char *pch = NULL;
		pch = strtok(line + cmdOffset, " ");
		char *cmd = strdup(pch);
		if (cmd[strlen(cmd) - 1] == '\n') cmd[strlen(cmd) - 1] = '\0';

		if (sub_argc > 1) 
		{
			i = 1;
			while (pch != NULL) 
			{
				pch = strtok(NULL, " ");
				if (pch == NULL)
					break;
				sub_argv[i] = strdup(pch);
				++i;
			}
			
			char *last_arg = sub_argv[sub_argc - 1];
			if (last_arg[strlen(last_arg) - 1] == '\n') last_arg[strlen(last_arg) - 1] = '\0';
		}

		if (strcmp(cmd, "exitShell") == 0)
			exiting = true;
		
		else 
		{
			char tmp[512];
			memset(tmp, 0, 512);
			if (cmdPath) 
				strcat(tmp, cmdPath);

			strcat(tmp, cmd);
			sub_argv[0] = strdup(tmp);

			int result = run(sub_argv);
			if (result > 0) 
				fprintf(stderr, "Command '%s' failed\n", cmd);
		}

		free(cmd);
		for (i = 0; i < sub_argc; ++i) 
			free(sub_argv[i]);
		free(sub_argv);
		
		if (exiting) 
		    break;
		free(line);
	}
	umount();
	free(line);
	return 0;
}

int run(char ** argv) 
{
    pid_t pid;
    int status;
    if (0 == (pid = fork())) 
    {
        if (-1 == execve(argv[0], (char **)argv, NULL)) 
        {
            perror(NULL);
            return -1;
        }
    }
    while (0 == waitpid(pid, &status, WNOHANG)) 
        usleep(100);
    return status;
}
