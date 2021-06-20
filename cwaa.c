#include <z_/types/base.h>
#include <z_/types/string.h>
#include <z_/types/enum.h>

#include <z_/imp/time.h>
#include <z_/imp/fio.h>
#include <z_/imp/ansi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tgmath.h>


/**
 * Base Config:
 * speed.min => Minimum Speed.
 * speed.max => Maximum Speed.
 * write_mode => Set Presentation mode. 
 */
struct config_glob {
    struct { z__f32 min, max; } speed;
    z__u8 write_mode; 
};


/**
 * Get a random number between min and max;
 */
z__f32 frandom(z__f32 min, z__f32 max)
{
	z__f32 diff = fabs(max - min);
	return fmod(random(), diff + 0.00000000000000001f) + min;
}

void write_text(z__String s, z__f32 speedmin, z__f32 speedmax)
{
    z__auto ch = s.str;
    z__bool sameSpeed = 1;
    z__f32 waittime = frandom(speedmin, speedmax);

    for(z__size i = 0; i < s.len; i++, ch++)
    {
        fputc(*ch, stdout);
        fflush(stdout);

        // Ignore White-spaces
        if(*ch == ' '
        || *ch == '\t'
        || *ch == '\n'){
            sameSpeed = 0;
        } else {
            z__time_msleep(waittime);
            if (!sameSpeed){
                waittime = frandom(speedmin, speedmax);
                sameSpeed = 1;
            }
        }
    }
}

void init_file(char const *path, struct config_glob *conf)
{
    z__String s = z__String_newFromFile((char *)path);
    if (s.str) {
        printf(
            z__ansi_scr((clear), (jump))
        );
        switch (conf->write_mode) {
            case 1: write_text(s, conf->speed.min, conf->speed.max); ;break;
            default: fputs(s.str, stdout); fputc('\n', stdout); fflush(stdout); break;

        }
    } else {
        printf("Cannot open File: %s\n", path);
    }
    z__String_delete(&s);
}

void start(int count, char * args[])
{
    struct config_glob conf = {
        .speed.min = 0.0f,
        .speed.max = 1.0f,
        .write_mode = 0
    };

    z__EnumDef(String_check, (Got, char *), (Not)) str_check;
    z__Enum_chip(&str_check, Not);

    for(int i = 1; i < count; i++) {
        if(args[i][0] == '-') {
            switch(args[i][1]){
                case 'm': if(count >= i+1)sscanf(args[i+1], "%f", &conf.speed.min); break;
                case 'x': if(count >= i+1)sscanf(args[i+1], "%f", &conf.speed.max); break;
                case 'd': if(count >= i+1)sscanf(args[i+1], "%hhd", &conf.write_mode); break;
            }
            i++;
        }
        else {
            ifSlot(str_check, Not)
                z__Enum_chip(&str_check, Got , args[i]);  
            unslot
        }
    }

    match(str_check) {
        slot(Got, path)
            if (*(strlen(*path) + *path - 1) == '/') {

                z__StringList dir = z__io_getfnames(*path);

                for (int i = 2; i < dir.ll_used; i++) {
                    printf(z__ansi_fmt((bold)) "%s: %dbytes" z__ansi_fmt((reset)) , dir.str_list[i], dir.str_lens[i]);
                    init_file(dir.str_list[i], &conf);
                }
                z__StringList_delete(&dir);

            } else {
                init_file(*path, &conf);
            }
        unslot;
        slot(Not) printf("No File Found!\n"); unslot;
    }

}

int main(int argc, char const * argv[])
{
    srandom(clock());
    start(argc, (char**)argv);
    return 0;
}
