#include "other_utils.h"

void mypwd(void)
{
    char *pwd = NULL;
    pwd = getcwd(pwd, MAX_INPUT_SIZE);
    printf("%s\n", pwd);
    free(pwd);
}

int mycd(DynamicStringArray *args)
{
    static char *OLDPWD = NULL;
    // determine destination
    char *des = NULL;
    // HOME directory
    if (args->size > 3 && args->data[1] != NULL && args->data[2] != NULL)
    {
        cd_arg_error();
        return -1;
    }
    if (args->size == 2 || (args->size == 3 && strcmp(args->data[1], "~") == 0))
    {
        des = getenv("HOME");
    }
    // last directory
    else if (args->size == 3 && strcmp(args->data[1], "-") == 0)
    {
        if (OLDPWD == NULL)
        {
            oldpwd_error();
            return -1;
        }
        else
        {
            promptn(OLDPWD);
            des = stringCopy(OLDPWD);
        }
    }
    else
        des = args->data[1];
    OLDPWD = getcwd(OLDPWD, MAX_INPUT_SIZE);
    if (des != NULL)
    {
        if (chdir(des) == -1)
        {
            if (errno == ENOENT)
            {
                cd_error(des);
            }
            else if (errno == EACCES)
            {
                permission_error(des);
            }
            else
            {
                perror("chdir");
            }
        }
    }
    else
    {
        cd_null_error();
        return -1;
    }

    if (args->size == 3 && strcmp(args->data[1], "-") == 0 && des != NULL)
        free(des);
    return 0;
}
