#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <la/config.h>

#define INIFILE "config.ini"

int main(void) {
    int rc = 0;
    FILE *file;
    config_t *cfg;
    char *tmp;

    file = fopen(INIFILE, "w");
    if (!file) return 1;
    fprintf(file, "# config_1\n");
    fprintf(file, "\n");
    fprintf(file, "key=value\n");
    fprintf(file, "\n");
    fprintf(file, "[test]\n");
    fprintf(file, "key=value\n");
    fprintf(file, "entry=value\n");
    fprintf(file, "item=value\n");
    fprintf(file, "\n");
    fprintf(file, "[test1]\n");
    fprintf(file, "key1=value1\n");
    fprintf(file, "entry1=val1\n");
    fprintf(file, "item1=v1\n");
    fclose(file);

    cfg = config_init();

    config_load(cfg, INIFILE);
    if (errno) {
        rc = errno;
        perror("open config");
        fflush(stdout);
        fflush(stderr);
        goto quit;
    }

    puts("\n[NONE]");
    tmp = config_get(cfg, "", "key");
    printf("key=%s\n", tmp);
    free(tmp);

    puts("\n[test]");
    tmp = config_get(cfg, "test", "key");
    printf("key=%s\n", tmp);
    free(tmp);

    puts("\n[test1]");
    tmp = config_get(cfg, "test1", "key1");
    printf("key1=%s\n", tmp);
    free(tmp);

    /* manipulate data */
    config_set(cfg, "", "key", "123");
    config_set(cfg, "", "new", "456");
    config_remove(cfg, "test1", "key1");
    config_remove(cfg, "test1", "entry1");
    config_remove(cfg, "test1", "item1");

    config_save(cfg, "test.ini");

    config_free(cfg);

quit:

#if 0
    unlink(INIFILE);
#endif
    return rc;
}
