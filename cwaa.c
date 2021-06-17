#include <z_/types/base.h>
#include <z_/types/string.h>

#include <z_/imp/time.h>
#include <z_/imp/fio.h>
#include <z_/imp/ansi.h>

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

z__f32 update_waittime(z__f32 min, z__f32 max)
{
	z__f32 diff = fabs(max - min);
	return fmod(random(), diff) + min;
}

void write_text(z__String s, z__f32 speedmin, z__f32 speedmax)
{
    z__auto ch = s.str;
    z__bool sameSpeed = 1;
    z__f32 waittime = update_waittime(speedmin, speedmax);
    for(z__size i = 0; i < s.len; i++, ch++)
    {
        fputc(*ch, stdout);
        fflush(stdout);
        if(*ch == ' '
        || *ch == '\t'
        || *ch == '\n'){
            z__time_msleep(10.0f);
            sameSpeed = 0;
        } else {
            z__time_msleep(waittime);
            if (!sameSpeed){
                waittime = update_waittime(speedmin, speedmax);
                sameSpeed = 1;
            }
        }
    }
}

int main(int argc, char const * argv[])
{
    srandom(52);
    z__String s = z__String_newFromFile((char*)argv[1]);
    if (s.str) {
        printf(
            z__ansi_scr((clear), (jump))
        );
        write_text(s, 1, 10.0f);
    } else {
        printf("Cannot open File\n");
    }
}
